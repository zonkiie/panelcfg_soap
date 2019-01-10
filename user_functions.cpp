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
