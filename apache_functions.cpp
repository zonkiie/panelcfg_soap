#include <includes.h>

/// @see https://stackoverflow.com/questions/13172158/c-split-string-by-line
vector<string> get_all_vhosts()
{
	vector<string> vhosts;
	vector<string> args{"-t", "-D", "DUMP_VHOSTS"};
	string result = pexec_read("/usr/sbin/apache2ctl", args);
	regex rex_vhost("\\s*port \\d+ namevhost ([^\\s]+) .+");
	regex rex_alias("\\s*alias ([^\\s]+).*");
	smatch sm;
	stringstream strs(result);
	string line;
	if(!result.empty())
	{
		while(getline(strs, line, '\n'))
		{
			if("VirtualHost configuration:" == line) continue;
			if(regex_match(line, sm, rex_vhost)) vhosts.push_back(sm[1]);
			if(regex_match(line, sm, rex_alias)) vhosts.push_back(sm[1]);
		}
	}
	sort (vhosts.begin(), vhosts.end());
	auto last = unique(vhosts.begin(), vhosts.end());
	vhosts.erase(last, vhosts.end());
	return vhosts;
}

/// @see https://stackoverflow.com/questions/1085083/regular-expressions-in-c-examples/1085120
/// @see https://www.lemoda.net/c/unix-regex/
char ** get_all_vhosts_c()
{
    char ** vhosts = (char**)calloc(sizeof(char*), 2), * str_ret = NULL, *saveptr1, *line = NULL, *name, **vhosts_sorted_unique = NULL;
    regex_t regex_vhost, regex_alias;
    const int nr_max_matches = 3;
    regmatch_t m_vhost[nr_max_matches], m_alias[nr_max_matches];
    int regex_result, regex_match_result, exec_result, exec_length;
    const char * apachectl_args[] = {"-t", "-D", "DUMP_VHOSTS", NULL};
    if((exec_result = pexec_to_carr(&str_ret, &exec_length, "/usr/sbin/apache2ctl", apachectl_args)) != 0) 
    {
        free(vhosts);
        return NULL;
    }
    if((regex_result = regcomp(&regex_vhost, "namevhost\\s+([\\w.-_]+)", REGEX_FLAGS))) return NULL;
    if((regex_result = regcomp(&regex_alias, "\\s*alias\\s+([\\w.-_]+).*", REGEX_FLAGS))) return NULL;
    
    for(line = strtok_r(str_ret, "\n", &saveptr1); line != NULL; line = strtok_r(NULL, "\n", &saveptr1))
    {
        if(line == NULL) break;
        if(!strcmp("VirtualHost configuration:", line)) continue;
        if((regex_match_result = regexec(&regex_vhost, line, nr_max_matches, m_vhost, 0)) == 0) 
        {
            if(m_vhost[1].rm_so != -1 && m_vhost[1].rm_eo != -1)
            {
                name = strndupa(line + m_vhost[1].rm_so, m_vhost[1].rm_eo - m_vhost[1].rm_so);
                if(name != NULL) array_push(&vhosts, name);
            }
        }
        if((regex_match_result = regexec(&regex_alias, line, nr_max_matches, m_alias, 0)) == 0)
        {
            if(m_alias[1].rm_so != -1 && m_alias[1].rm_eo != -1)
            {
                name = strndupa(line + m_alias[1].rm_so, m_alias[1].rm_eo - m_alias[1].rm_so);
                if(name != NULL) array_push(&vhosts, name);
            }
        }
    }
    free(str_ret);
    regfree(&regex_vhost);
    regfree(&regex_alias);
    qsort(vhosts, get_carr_size(vhosts), sizeof(char *), cmpstringp);
    make_cstr_array_unique(&vhosts_sorted_unique, vhosts);
    qsort(vhosts_sorted_unique, get_carr_size(vhosts_sorted_unique), sizeof(char *), cmpstringp);
    free_carr(&vhosts);
    return vhosts_sorted_unique;
}

/**
 * find the site file for an alias or vhost
 */
char * get_site_for_vhost(const char * vhost_name)
{
    char * site = NULL, *tmp_site = NULL, * str_ret = NULL, *saveptr1 = NULL, *line = NULL, **matches = NULL;
    int exec_result, exec_length, extract_result;
    const char * apachectl_args[] = {"-t", "-D", "DUMP_VHOSTS", NULL};
    if((exec_result = pexec_to_carr(&str_ret, &exec_length, "/usr/sbin/apache2ctl", apachectl_args)) != 0) return NULL;
    for(line = strtok_r(str_ret, "\n", &saveptr1); line != NULL; line = strtok_r(NULL, "\n", &saveptr1))
    {
        if(line == NULL) break;
        if(!strcmp("VirtualHost configuration:", line)) continue;
        //"namevhost\\s+([\\w.-_]+)\\s+\\(([\\w\\d.-_:/]+)\\)"
        if((extract_result = cstring_extract_from_regex(&matches, "namevhost\\s+([\\w.-_]+)\\s+\\((.+)\\)", line, 3, REGEX_FLAGS)) > 1)
        {
            //fprintf(stderr, "Zeile %d: Matches[0]: %s, Matches[1]: %s\n", __LINE__, matches[0], matches[1]);
            tmp_site = strdupa(matches[1]);
            if(!strcmp(matches[0], vhost_name)) 
            {
                free_carr(&matches);
                break;
            }
            free_carr(&matches);
        } else free_carr(&matches);
        if((extract_result = cstring_extract_from_regex(&matches, "\\s*alias\\s+([\\w.-_]+).*", line, 2, REGEX_FLAGS)) > 0)
        {
            //fprintf(stderr, "Zeile %d: Matches[0]: %s\n", __LINE__, matches[0]);
            if(!strcmp(matches[0], vhost_name)) 
            {
                free_carr(&matches);
                break;
            }
            free_carr(&matches);
        } else free_carr(&matches);
    }
    free(str_ret);
    if(tmp_site) site = strdup(tmp_site);
    return site;
}

vector<vhost> get_all_vhost_data()
{
	vector<vhost> vhosts;
	vector<string> args{"-t", "-D", "DUMP_VHOSTS"};
	string result = pexec_read("/usr/sbin/apache2ctl", args);
	regex rex_vhost("\\s*port (\\d+) namevhost ([^\\s]+) \\((.+)\\)");
	regex rex_alias("\\s*alias ([^\\s]+).*");
	smatch sm;
	stringstream strs(result);
	string line;
	if(!result.empty())
	{
		while(getline(strs, line, '\n'))
		{
			if("VirtualHost configuration:" == line) continue;
			if(regex_match(line, sm, rex_vhost))
			{
				vhost vh;
				vhosts.push_back(vh);
				if(sm[1] != "") vhosts.back().port = atoi(sm[1].str().c_str());
				vhosts.back().vhost_name = sm[2];
				vhosts.back().source_file = sm[3];
			}
			if(regex_match(line, sm, rex_alias)) vhosts.back().vhost_aliases.push_back(sm[1]);
		}
	}
	return vhosts;
}

string get_vhost_entry_string(string vhostName)
{
	vector<vhost> vh = get_all_vhost_data();
	string source("");
	string entry("");
	smatch sm;
	regex vhost_close_tag("\\s*<\\s*/VirtualHost\\s*>");
	for(vector<vhost>::iterator it = vh.begin() ; it != vh.end(); it++)
	{
		if((*it).vhost_name == vhostName || std::find((*it).vhost_aliases.begin(), (*it).vhost_aliases.end(), vhostName) != (*it).vhost_aliases.end())
		{
			source = (*it).source_file;
			break;
		}
	}
	if(!source.empty())
	{
		vector<string> res;
		boost::split(res, source, boost::is_any_of(":"));
		string filename = res[0];
		if(res[0] == "" || res[1] == "") goto get_vhost_entry_string_return;
		int lineno = atoi(res[1].c_str());
		ifstream ifs(res[0]);
		int linecnt = 0;
		string line;
		// Jump to right line
		if(lineno > 1) while(getline(ifs, line, '\n') && ++linecnt < lineno - 1);
		//entry += line + '\n';
		while(getline(ifs, line, '\n'))
		{
			if(regex_match(line, sm, vhost_close_tag))
			{
				entry += line + '\n';
				break;
			}
			entry += line + '\n';
		}
		ifs.close();
		
	}
get_vhost_entry_string_return:
	return entry;
}

string get_site_file(string sitename)
{
	return string(SITEDIR) + "/" + sitename + ".conf";
}

vector<string> get_all_sites()
{
	vector<string> sites = getFileList(SITEDIR);
	return sites;
}

bool site_exists(string sitename)
{
	string filename = get_site_file(sitename);
	if(filesys::exists(filename) && filesys::is_regular_file(filename)) return true;
	return false;
}

bool add_site(string sitename)
{
	string filename = get_site_file(sitename);
	ofstream outfile(filename, ios_base::out|ios_base::app);
	outfile << HEADER_TEXT << endl;
	outfile.close();
	return set_site_status(sitename, true) == 0;
}

bool set_site_status(string sitename, bool status)
{
	int res = 0;
	if(status == true)
	{
		vector<string> args{"a2ensize", sitename};
		res = execvp_fork("a2ensize", args);
	}
	else
	{
		vector<string> args{"a2dissize", sitename};
		res = execvp_fork("a2dissize", args);
	}
	return res == 0;
}

bool del_site(string sitename)
{
	string filename = get_site_file(sitename);
	return set_site_status(sitename, false) == true && unlink(filename.c_str()) == 0;
}

string create_vhost_string(vhost vh)
{
	stringstream rstr;
	rstr << "<VirtualHost *:" << vh.port << ">\n";
	rstr << "\tServerName " << vh.vhost_name << endl;
	rstr << "\tServerAlias";
	for(vector<string>::iterator it = vh.vhost_aliases.begin() ; it != vh.vhost_aliases.end(); it++)
		rstr << " " << (*it);
	rstr << endl;
	if(vh.type == localvhost) rstr << "\tDocumentRoot " << vh.document_root << endl;
	else if(vh.type == redirect_permanent) rstr << "\tRedirectPermanent / " << vh.redirect_target << endl;
	else if(vh.type == redirect_temporary) rstr << "\tRedirectTemp / " << vh.redirect_target << endl;
	rstr << "</VirtualHost>\n";
	return rstr.str();
}

string create_vhost_string(string vhostname, string documentroot)
{
	stringstream rstr;
	rstr << "<VirtualHost *:80>\n";
	rstr << "\tServerName " << vhostname << endl;
	rstr << "\tServerAlias " << vhostname << " www." << vhostname << endl;
	rstr << "\tDocumentRoot " << documentroot << endl;
	rstr << "</VirtualHost>\n";
	rstr << endl;
	return rstr.str();
}

bool add_vhost(string sitename, string vhostname, string documentroot)
{
	string vhstr = create_vhost_string(vhostname, documentroot);
	return set_vhost_string(sitename, vhostname, vhstr);
}

bool add_vhost(string sitename, vhost vh)
{
	string vhstr = create_vhost_string(vh);
	return set_vhost_string(sitename, vh.vhost_name, vhstr);
}

/// @see https://stackoverflow.com/questions/116038/what-is-the-best-way-to-read-an-entire-file-into-a-stdstring-in-c
bool set_vhost_string(string sitename, string vhostname, string vhost_string)
{
	string current_string = get_vhost_entry_string(vhostname);
	stringstream site_str("");
	ifstream ifs(get_site_file(sitename));
	site_str << ifs.rdbuf();
	ifs.close();
	string site_string = site_str.str();
	site_str.str("");
	if(current_string != "")
	{
		boost::replace_all(site_string, current_string, vhost_string);
		ba::trim(site_string);
		site_string += '\n';
		ofstream ofs(get_site_file(sitename));
		ofs << site_string;
		ofs.close();
		return ofs.good();
	}
	else
	{
		if(!site_exists(sitename))
		{
			add_site(sitename);
			set_site_status(sitename, true);
		}
		ofstream ofs(get_site_file(sitename), ostream::app);
		ofs << vhost_string;
		ofs.close();
		return ofs.good();
	}
}

bool change_vhost(string sitename, string vhostname, string documentroot)
{
	string vhstr = create_vhost_string(vhostname, documentroot);
	set_vhost_string(sitename, vhostname, vhstr);
	return true;
}

bool change_vhost(string sitename, vhost vh)
{
	string vhstr = create_vhost_string(vh);
	set_vhost_string(sitename, vh.vhost_name, vhstr);
	return true;
}

bool del_vhost(string sitename, string vhostname)
{
	return set_vhost_string(sitename, vhostname, "");
}

bool restart_apache()
{
	vector<string> args{"apache2ctl", "restart"};
	return execvp_fork("apache2ctl", args);
}

bool reload_apache()
{
	vector<string> args{"apache2ctl", "reload"};
	return execvp_fork("apache2ctl", args);
}
