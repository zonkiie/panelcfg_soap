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

string get_vhost_entry_string(string vhost_name)
{
	vector<vhost> vh = get_all_vhost_data();
	string source("");
	string entry("");
	smatch sm;
	regex vhost_close_tag("\\s*<\\s*/VirtualHost\\s*>");
	for(vector<vhost>::iterator it = vh.begin() ; it != vh.end(); it++)
	{
		if((*it).vhost_name == vhost_name || std::find((*it).vhost_aliases.begin(), (*it).vhost_aliases.end(), vhost_name) != (*it).vhost_aliases.end())
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
	outfile << HEADER_TEXT;
	outfile.close();
	return set_site_status(sitename, true) == 0;
}

bool set_site_status(string sitename, bool status)
{
	int res = 0;
	if(status == true)
	{
		vector<string> args{"a2ensize", sitename};
		res = execvp_fork("/usr/sbin/a2ensize", args);
	}
	else
	{
		vector<string> args{"a2dissize", sitename};
		res = execvp_fork("/usr/sbin/a2dissize", args);
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
	rstr << "\tServerAlias " << vhostname << "www." << vhostname << endl;
	rstr << "\tDocumentRoot " << documentroot << endl;
	rstr << "</VirtualHost>\n";
	rstr << endl;
	return rstr.str();
}

bool add_vhost(string sitename, string vhostname, string documentroot)
{
	string vhstr = create_vhost_string(vhostname, documentroot);
	set_vhost_string(sitename, vhostname, vhstr);
	return true;
}

bool add_vhost(string sitename, vhost vh)
{
	string vhstr = create_vhost_string(vh);
	set_vhost_string(sitename, vh.vhost_name, vhstr);
	return true;
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
		ofstream ofs(get_site_file(sitename));
		ofs << site_string;
		ofs.close();
	}
	else
	{
		ofstream ofs(get_site_file(sitename), ostream::app);
		ofs << vhost_string;
		ofs.close();
	}
	return true;
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
	set_vhost_string(sitename, vhostname, "");
	return false;
}
