#ifndef __common_functions__
#define __common_functions__

using namespace std;

extern char * progpath;
extern char * progdir;
bool check_credentials(struct soap* );
bool check_credentials(struct soap* , struct passwd** );
bool check_password(string , string);
bool check_auth(struct soap*);
vector<string> getFileList(string);
int parse_configstring(const char *, char *, char *);

#endif
