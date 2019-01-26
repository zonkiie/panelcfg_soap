#ifndef __common_functions__
#define __common_functions__

using namespace std;

extern char * progpath;
extern char * progdir;
int get_filesize(char *);
int execvp_fork(string, vector<string>);
int execvp_fork2(string, vector<string>);
string pexec_read(string, vector<string>);
char **vector2carr(vector<string> &);
void free_carr(char ***);
mode_t get_type_for_path(string);
int access(string, int);
bool is_special_node(string);
bool is_symlink(string);
bool is_file(string);
bool is_dir(string);
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
bool check_credentials(struct soap* );
bool check_credentials(struct soap* , struct passwd** );
bool check_password(string , string);
bool check_auth(struct soap*);
vector<string> getFileList(string);
char* devname_from_procpartitions(int, int);
char* getdev(const char *);
int parse_configstring(const char *, char *, char *);

#endif
