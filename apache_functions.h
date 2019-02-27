#ifndef __apache_functions__
#define __apache_functions__

vector<vhost> get_all_vhost_data();
vector<string> get_all_sites();
string create_vhost_string(vhost virtualhost);
string create_vhost_string(string vhostname, string documentroot);
bool set_vhost_string(string sitename, string vhostname, string vhost_string);
bool add_vhost(string sitename, string vhostname, string documentroot);
bool add_vhost(string sitename, vhost vh);
bool change_vhost(string sitename, string vhostname, string documentroot);
bool change_vhost(string sitename, vhost vh);
bool del_vhost(string sitename, string vhostname);


#endif
