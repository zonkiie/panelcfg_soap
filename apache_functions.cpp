#include <includes.h>

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

