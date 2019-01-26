#include <includes.h>

char *progpath;
char *progdir;

/// @see https://stackoverflow.com/questions/35390912/proper-way-to-get-file-size-in-c
int get_filesize(char * filename)
{
	int fsize = 0;
	if(access(filename, F_OK)) // file not found
		return -1;
	struct stat buffer;
	int         status;

	status = stat(filename, &buffer);
	if(status == 0) {
	// size of file is in member buffer.st_size;
		fsize = buffer.st_size;
	}
	return fsize;
}

// From: libzbcl
/// @see http://www.cs.ecu.edu/karl/4630/sum01/example1.html
int execvp_fork(string file, vector<string> argv)
{
	char** cargv;
	int pid = fork();
	int pid_child;
	int state = -1;
	if(pid == 0)
	{
		close(1);
		close(2);
		cargv = vector2carr(argv);
		execvp(file.c_str(), cargv);
	}
	else
	{
		do {
			pid_child = wait(&state);
		} while(pid != pid_child);
	}
	//free_carr(&cargv);
	return state;
}

/**
 * @brief executes an external program. you don't need to insert the basename of the command on the argv vector, it's done by this command.
 * @param file the file to execute
 * @param argv an vector of arguments
 * @return the exit code of the executed program.
 */
int execvp_fork2(string file, vector<string> argv)
{
	argv.insert(argv.begin(), basename(file));
	char** cargv;
	int pid = fork();
	int pid_child;
	int state = -1;
	if(pid == 0)
	{
		close(1);
		close(2);
		cargv = vector2carr(argv);
		execvp(file.c_str(), cargv);
	}
	else
	{
		do {
			pid_child = wait(&state);
		} while(pid != pid_child);
	}
	//free_carr(&cargv);
	return state;
}
 
/**
*  Fork an Process, create a Pipe and read the Answer of the Child from the Pipe
*  returns a c++ string
*  command: Program to execute
*  args: Vector of args (argv[0] MUST NOT be the Program name)
*/
string pexec_read(string command, vector<string> args)
{
	args.insert(args.begin(), basename(command));
	//fprintf(stderr, "Command: %s\n", command.c_str());
	//for(uint i = 0; i < args.size(); i++) fprintf(stderr, "args[%d]: %s\n", i, args[i].c_str());
	string retstr = "";
	int inpipe[2];
	char inbuffer[256];
	strcpy(inbuffer, "");
	pipe(inpipe);
	int pid;
	int size = 0;
	pid = fork();
	if(pid == 0)
	{
		close(1);
		dup(inpipe[1]);
		close(inpipe[0]);
		char **cargs = vector2carr(args);
		execvp(command.c_str(), cargs);
		free_carr(&cargs);
		exit(0);
	}
	else
	{
		dup(inpipe[0]);
		close(inpipe[1]);
		while((size = read(inpipe[0], &inbuffer, sizeof(inbuffer))) > 0)
		{
			inbuffer[size] = '\0';
			retstr += inbuffer;
			strcpy(inbuffer, "");
		}
	}
	return(retstr);
}

/**
*  C++ Vector<string> to C char** Array
* @param v The vector<string> to be converted
* @returns a string array 
*/
char **vector2carr(vector<string> &v)
{
	char **cargs = (char**)malloc((v.size()+1)*sizeof(char**));
	vector<string>::iterator it = v.begin();
	while(it != v.end())
	{
		cargs[it - v.begin()] = strdup((*it).c_str());
		it++;
	}
	cargs[v.size()] = NULL;
	return(cargs);
}

/**
*  Iterate through C array and free all Elements
*/
void free_carr(char ***carr)
{
	int i = 0;
	while((*carr)[i] != NULL)
	{
		free((*carr)[i]);
		i++;
	}
	free(*carr);
}

mode_t get_type_for_path(string path)
{
	struct stat st;
	int res;
	res = lstat(path.c_str(), &st);
	if(res == -1) return (mode_t)-1;
	else return st.st_mode & S_IFMT;
}

int access(string path, int mode) { return access(path.c_str(), mode); }

bool is_special_node(string path)
{
	if(access(path, F_OK)) return false;
	if(get_type_for_path(path) & (S_IFBLK | S_IFCHR | S_IFIFO | S_IFSOCK)) return true;
	else return false;
}

bool is_symlink(string path)
{
	if(access(path, F_OK)) return false;
	if(get_type_for_path(path) & S_IFLNK) return true;
	else return false;
}

bool is_file(string path)
{
	if(access(path, F_OK)) return false;
	if(get_type_for_path(path) & S_IFREG) return true;
	else return false;
}

bool is_dir(string path)
{
	if(access(path, F_OK)) return false;
	if(get_type_for_path(path) & S_IFDIR) return true;
	else return false;
}

string basename(string path)
{
	return(string(basename((char*)path.c_str())));
}

string dirname(string path)
{
	return(string(dirname((char*)path.c_str())));
}

char* strdup(string str)
{
	return (char*)str.c_str();
}

string s_crypt(string password, string salt)
{
	return(string(crypt(password.c_str(), salt.c_str())));
}

char get_rand_char()
{
	string zeichen = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
	int nr = zeichen.length();
	char c;
	srand((unsigned int)time((time_t *)NULL));
	c = zeichen[(rand()%nr) + 1];
	return(c);
}

string get_rand_string(int number)
{
	string zeichen = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
	int nr = zeichen.length();
	string randstr = "";
	char c;
	for(int i = 0; i < number; i++)
	{
		srand((unsigned int)time((time_t *)NULL) + i);
		c = zeichen[(rand()%nr) + 1];
		randstr += c;
		c = 0;
	}
	return(randstr);
}

string make_salt()
{
	string salt = "";
	salt+=get_rand_string(2);
	return(salt);
}

string make_ext_salt()
{
	string salt = "_";
	salt+=get_rand_string(8);
	return(salt);
}

string make_md5_salt()
{
	string salt = "$1$";
	salt+=get_rand_string(5);
	return(salt);
}

string make_blowfish_salt()
{
	string salt = "$2a$09$";
	salt+=get_rand_string(22);
	return(salt);
}

string make_sha256_salt()
{
	string salt = "$5$rounds=5000$";
	salt+=get_rand_string(26);
	salt+="$";
	return(salt);
}

string make_sha512_salt()
{
	string salt = "$6$rounds=5000$";
	salt+=get_rand_string(26);
	salt+="$";
	return(salt);
}

int cr_check_pass(const char *plainpw, const char *cryptpw)
{
	return strcmp(crypt(plainpw,cryptpw), cryptpw) == 0;
}

int cr_check_pass(string plain, string crypted)
{
	return(cr_check_pass(plain.c_str(), crypted.c_str()));
}

bool check_password(string username, string password)
{
	struct spwd *spw = getspnam(username.c_str());
	if(spw == NULL) return false;
	return(s_crypt(password, string(spw->sp_pwdp)) == string(spw->sp_pwdp));
}

bool check_credentials(struct soap* soap)
{
	struct passwd * pwd = getpwnam(soap->userid);
	return check_credentials(soap, &pwd);
}

bool check_credentials(struct soap* soap, struct passwd** pwd)
{
	if(*pwd == NULL) *pwd = getpwnam(soap->userid);
	// Check User: username must not be null
	if(soap->userid == NULL || !strcmp(soap->userid, "")) return false;
	// Check Password: don't allow empty or null passwords
	if(soap->passwd == NULL || !strcmp(soap->passwd, "")) return false;
	// Check if user exists
	if(*pwd == NULL) return false;
	// Check for correct password
	if(!check_password(string(soap->userid), string(soap->passwd))) return false;
	return true;
}

bool check_auth(struct soap* soap)
{
	bool state = false;
	struct passwd *pwd = getpwnam(soap->userid);
	if(!check_credentials(soap, &pwd))
	{
		fprintf(stderr, "Invalid Credentials!\n");
		return false;
	}
	// Check correct group
	int ngroups = 1;
	gid_t* groups = (gid_t*)malloc(ngroups * sizeof (gid_t));
	int res = getgrouplist(soap->userid, pwd->pw_gid, groups, &ngroups);
	if(res == -1)
	{
		groups = (gid_t*)realloc(groups, ngroups * sizeof (gid_t));
		res = getgrouplist(soap->userid, pwd->pw_gid, groups, &ngroups);
		if(res == -1) fprintf(stderr, "Fail on line %d in file %s\n", __LINE__, __FILE__);
	}
	for(int i = 0; i < ngroups; i++)
	{
		struct group *grp = getgrgid(groups[i]);
		//fprintf(stderr, "Group: %s[%d]\n", grp->gr_name, groups[i]);
		// Hard wired for the begin or as default if no configfile exists
		if(!strcasecmp(grp->gr_name, "admin")) state = true;
	}
	free(groups);
	
	// Check correct user
	
	if(state == false)
	{
		fprintf(stderr, "No Permission!\n");
	}
	return state;
}

vector<string> getFileList(string path)
{
	vector<string> entries;
	string entry;
	DIR           *dir_p = NULL;
	struct dirent *dir_entry_p = NULL;
	
	if(NULL == (dir_p = opendir(path.c_str()))) return entries;

	while(NULL != (dir_entry_p = readdir(dir_p)))
	{
		entry = string(dir_entry_p->d_name);
		if((entry != ".") && (entry != "..") && (is_file(path + string("/") + entry))) entries.push_back(entry);
	}
	closedir(dir_p);
	return(entries);
}

char* devname_from_procpartitions(int major, int minor)
{
	char* procfilename = "/proc/partitions";
	char* content = NULL;
	if(access(procfilename, F_OK)) return NULL;
	FILE *file = fopen(procfilename, "r");
	size_t readbytes = 1;
	if(file == NULL) return NULL;
	size_t size;
	int linecount = 0;
	while((size = getline(&content, &readbytes, file)) > 0)
	{
		// Copy content from heap variable content to stack variable cc to get rid of allocated memory
		char cc[size];
		strcpy(cc, content);
		free(content);
		content = NULL;
		// now lets skip the first two lines of the file
		linecount++;
		if(linecount < 2) continue;
		int pmajor = -1, pminor = -1;
		long long blocks;
		char name[1024];
		bzero(name, sizeof(name));
		sscanf(cc, "%d %d %lld %1024s", &pmajor, &pminor, &blocks, name);
		if(pmajor == major && pminor == minor)
		{
			fclose(file);
			return strdup(name);
		}
	}
	fclose(file);
	return NULL;
}


/// @see https://stackoverflow.com/questions/26046949/find-the-device-or-mountpoint-of-arbitrary-files-on-linux-using-c
char* getdev(const char *path)
{
	int major = -1, minor = -1;
	struct stat st;
	if(stat(path, &st) != 0) return NULL;
	major = major(st.st_dev);
	minor = minor(st.st_dev);
	const char* devdir = "/dev/";
	char* devname = devname_from_procpartitions(major, minor);
	size_t slen = strlen(devname) + strlen(devdir) + 1;
	char* result = (char*)malloc(slen);
	strcpy(result, devdir);
	strcat(result, devname);
	free(devname);
	return result;
}

int parse_configstring(const char * line, char * key, char * value)
{
	size_t s;
	s = sscanf(line, "%[^#^=]=%[^\n]", key, value);
	if(s <= 0) return -1;
	else return s;
}
