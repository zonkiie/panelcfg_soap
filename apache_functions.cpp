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
	regex rex_vhost("\\s*port \\d+ namevhost ([^\\s]+) \\((.+)\\)");
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
				vhosts.back().vhost_name = sm[1];
				vhosts.back().source_file = sm[2];
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
vector<string> get_all_sites()
{
	vector<string> sites = getFileList(SITEDIR);
	return sites;
}

bool add_site(string sitename)
{
	string filename = string(SITEDIR) + "/" + sitename + ".conf";
	ofstream outfile(filename, ios_base::out|ios_base::app);
	outfile << HEADER_TEXT;
	outfile.close();
	return false;
}

bool set_site_status(string sitename, bool status)
{
	return false;
}

bool remove_site(string sitename)
{
	return false;
}

bool add_vhost(string sitename, string vhostname)
{
	return false;
}
