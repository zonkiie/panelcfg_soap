#include <includes.h>

/// @see https://stackoverflow.com/questions/13172158/c-split-string-by-line
vector<string> get_all_vhosts()
{
	vector<string> vhosts;
	vector<string> args{"-t", "-D", "DUMP_VHOSTS"};
	string result = pexec_read("/usr/sbin/apache2ctl", args);
	regex rex("\\s*port \\d+ namevhost ([^\\s]+) .+");
	smatch sm;
	stringstream strs(result);
	string line;
	if(!result.empty())
	{
		while(getline(strs, line, '\n'))
		{
			if("VirtualHost configuration:" == line) continue;
			if(regex_match(line, sm, rex)) vhosts.push_back(sm[1]);
		}
	}
	return vhosts;
}
