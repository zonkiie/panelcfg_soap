#ifndef __apache_functions__
#define __apache_functions__

#define SITEDIR "/etc/apache2/sites-available"
#define HEADER_TEXT "### Warning: File is generated automatically.\n###If you edit this file, your settings could be loosed!\n\n"

vector<string> get_all_vhosts();
char ** get_all_vhosts_c();
vector<vhost> get_all_vhost_data();
string get_vhost_entry_string(string vhostName);
vector<string> get_all_sites();
string get_site_file(string sitename);
bool site_exists(string sitename);
bool add_site(string sitename);
bool set_site_status(string sitename, bool status);
bool del_site(string sitename);
string create_vhost_string(vhost virtualhost);
string create_vhost_string(string vhostname, string documentroot);
bool set_vhost_string(string sitename, string vhostname, string vhost_string);
bool add_vhost(string sitename, string vhostname, string documentroot);
bool add_vhost(string sitename, vhost vh);
bool change_vhost(string sitename, string vhostname, string documentroot);
bool change_vhost(string sitename, vhost vh);
bool del_vhost(string sitename, string vhostname);
bool restart_apache();
bool reload_apache();

#endif
