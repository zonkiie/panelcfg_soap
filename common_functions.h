#ifndef __common_functions__
#define __common_functions__

using namespace std;

extern char * progpath;
extern char * progdir;
int get_filesize(char * filename);
int execvp_fork(string file, vector<string> argv);
string pexec_read(string command, vector<string> args);
char **vector2carr(vector<string> &v);
void free_carr(char ***carr);
string basename(string);
string dirname(string);
char* strdup(string);
string s_crypt(string, string);
string make_md5_salt();
string make_salt();
string make_ext_salt();
string make_blowfish_salt();
string make_sha256_salt();
string make_sha512_salt();
char get_rand_char();
string get_rand_string(int);
int cr_check_pass(const char *, const char *);
int cr_check_pass(string, string);
bool check_credentials(struct soap* soap);
bool check_credentials(struct soap* soap, struct passwd** pwd);
bool check_password(string username, string password);
bool check_auth(struct soap* soap);
vector<string> getFileList(string dirPath);

#endif
