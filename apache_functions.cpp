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

vector<string> get_all_sites()
{
	vector<string> sites;
	return sites;
}

bool add_site(string sitename)
{
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
