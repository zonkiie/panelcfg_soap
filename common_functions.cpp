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

int execvp(string file, vector<string> argv)
{
	argv.insert(argv.begin(), basename(file));
	char** cargv = (char**)malloc(argv.size());
	vector<string>::iterator it = argv.begin();
	int i = 0;
	while(it != argv.end())
	{
		cargv[i++] = strdup(*it);
		it++;
	}
	cargv[argv.size()] = NULL;
	int state = execvp(file.c_str(), cargv);
	for(int i = 0; cargv[i] != NULL; i++) free(cargv[i]);
	free(cargv);
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
		args.insert(args.begin(), basename(command));
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
	for(uint i = 0; i < v.size(); i++)
	{
		cargs[i] = strdup(v.at(i).c_str());
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
/*	char* enc_pw_c = crypt (password.c_str(), salt.c_str());
	string enc_pw = enc_pw_c;*/
	string enc_pw = crypt(password.c_str(), salt.c_str());
	return(enc_pw);
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
	char* enc_pass = spw->sp_pwdp;
	if(s_crypt(password, string(enc_pass)) == string(enc_pass)) return true;
	return false;
}
