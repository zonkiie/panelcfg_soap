#include <includes.h> 

/// @see https://www.fluentcpp.com/2017/04/21/how-to-split-a-string-in-c/
/// @see http://www.martinbroadhurst.com/how-to-split-a-string-in-c.html
vector<string> getUserList()
{
	ifstream ifs("/etc/passwd");
	vector<string> userlist;
	
	string line;
	while(getline(ifs, line))
	{
		vector<string> entries;
		//boost::split(entries, line, [](char c){return c == ':';});
		boost::split(entries, line, boost::is_any_of(":"));
		for(vector<string>::const_iterator i = entries.begin(); i != entries.end(); i++) {
			//cout << *j << endl;
			userlist.push_back((*i));
			break;
		}
		//cout << *i << " "; // this will print all the contents of *features*
	}
	ifs.close();
	return userlist;
}

bool userDel(string username)
{
	vector<string> args{"userdel", "-r", username};
	return execvp_fork("/usr/sbin/userdel", args);
}

bool changePassword(string username, string password)
{
	string enc_password = s_crypt(password, make_sha512_salt());
	vector<string> args{"usermod", "-p", enc_password, username};
	return execvp_fork("/usr/sbin/usermod", args);
}

bool checkPassword(int & error_status, string username, string password)
{
	error_status = 0;
	struct spwd *spw = getspnam(username.c_str());
	if(spw == NULL)
	{
		error_status = 401;
		return false;
	}
	return (s_crypt(password, string(spw->sp_pwdp)) == string(spw->sp_pwdp));
}

bool addUser(int & error_status, string username, string password, string homedir, string shell, string groupname)
{
	error_status = 0;
	if(password.empty())
	{
		error_status = 401;
		return false;
	}
	string enc_password = s_crypt(password, make_sha512_salt());
	if(enc_password.empty())
	{
		error_status = 401;
		return false;
	}
	vector<string> args{"useradd", "-p", enc_password, "-m", username};
	if(!homedir.empty()) args.insert(args.end(), {"-d", homedir});
	if(!shell.empty()) args.insert(args.end(), {"-s", shell});
	if(!groupname.empty()) args.insert(args.end(), {"-g", groupname});
	return execvp_fork("/usr/sbin/useradd", args);
}
