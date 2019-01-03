#ifndef __apache_functions__
#define __apache_functions__

vector<string> get_all_vhosts();
vector<vhost> get_all_vhost_data();
string get_vhost_entry_string(string vhost_name);
vector<string> get_all_sites();
bool add_site(string sitename);
bool set_site_status(string sitename, bool status);
bool remove_site(string sitename);
bool add_vhost(string sitename, string vhostname);

#endif
