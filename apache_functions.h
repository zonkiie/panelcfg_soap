#ifndef __apache_functions__
#define __apache_functions__

#define SITEDIR "/etc/apache2/sites-available"
#define HEADER_TEXT "### Warning: File is generated automatically.\n###If you edit this file, your settings could be loosed!\n\n"

vector<string> get_all_vhosts();
vector<vhost> get_all_vhost_data();
string get_vhost_entry_string(string vhost_name);
vector<string> get_all_sites();
bool add_site(string sitename);
bool set_site_status(string sitename, bool status);
bool remove_site(string sitename);
bool add_vhost(string sitename, string vhostname);

#endif
