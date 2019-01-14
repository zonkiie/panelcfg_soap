#include <includes.h> 

vector<string> getUserList()
{
	vector<string> userlist;
	struct passwd *pwd;
	while((pwd = getpwent()) != NULL) userlist.push_back(string(pwd->pw_name));
	endpwent();
	return userlist;
}

bool delUser(string username)
{
	vector<string> args{"userdel", "-r", username};
	return execvp_fork("userdel", args) == 0;
}

bool changePassword(string username, string password)
{
	string enc_password = s_crypt(password, make_sha512_salt());
	vector<string> args{"usermod", "-p", enc_password, username};
	return execvp_fork("usermod", args) == 0;
}

bool checkPassword(int & error_status, string username, string password)
{
	error_status = 0;
	struct spwd *spw = getspnam(username.c_str());
	if(spw == NULL)
	{
		error_status = 404;
		return false;
	}
	return (s_crypt(password, string(spw->sp_pwdp)) == string(spw->sp_pwdp));
}

bool infoUser(int & error_status, string& username, string& homedir, string& shell, string& groupname, int & uid)
{
	struct passwd *pwd = getpwnam(username.c_str());
	if(pwd == 0)
	{
		error_status = 404;
		return false;
	}
	homedir = string(pwd->pw_dir == NULL?"":pwd->pw_dir);
	shell = string(pwd->pw_shell == NULL?"":pwd->pw_shell);
	struct group * grp = getgrgid(pwd->pw_gid);
	groupname = string(grp->gr_name == NULL?"":grp->gr_name);
	uid = pwd->pw_uid;
	return true;
}

bool addUser(int & error_status, string username, string password, string homedir, string shell, string groupname)
{
	error_status = 0;
	if(password.empty())
	{
		error_status = 420;
		return false;
	}
	string enc_password = s_crypt(password, make_sha512_salt());
	if(enc_password.empty())
	{
		error_status = 420;
		return false;
	}
	vector<string> args{"useradd", "-p", enc_password, "-m", username};
	if(!homedir.empty()) args.insert(args.end(), {"-d", homedir});
	if(!shell.empty()) args.insert(args.end(), {"-s", shell});
	if(!groupname.empty()) args.insert(args.end(), {"-g", groupname});
	return execvp_fork("useradd", args) == 0;
}

bool changeShell(string username, string shell)
{
	vector<string> args{"usermod", "-s", shell, username};
	return execvp_fork("usermod", args);
}

vector<string> getUserGroupMembership(string username)
{
	vector<string> result;
	int ngroups = 1;
	struct passwd *pwd = getpwnam(username.c_str());
	gid_t* groups = (gid_t*)malloc(ngroups * sizeof (gid_t));
	int res = getgrouplist(username.c_str(), pwd->pw_gid, groups, &ngroups);
	if(res == -1)
	{
		groups = (gid_t*)realloc(groups, ngroups * sizeof (gid_t));
		res = getgrouplist(username.c_str(), pwd->pw_gid, groups, &ngroups);
		if(res == -1) fprintf(stderr, "Fail on line %d in file %s\n", __LINE__, __FILE__);
	}
	for(int i = 0; i < ngroups; i++)
	{
		struct group *grp = getgrgid(groups[i]);
		result.push_back(grp->gr_name);
	}
	free(groups);
	return result;
}

bool addGroup(string groupname)
{
	vector<string> args{"groupadd", groupname};
	return execvp_fork("groupadd", args) == 0;
}

bool delGroup(string groupname)
{
	vector<string> args{"groupdel", groupname};
	return execvp_fork("groupdel", args) == 0;
}

bool setGroupMembers(string groupname, vector<string> members)
{
	string str_members = boost::algorithm::join(members, ",");
	vector<string> args{"gpasswd", "-M", str_members, groupname};
	return execvp_fork("gpasswd", args) == 0;
}

vector<string> getGroupMembers(string groupname)
{
	vector<string> result;
	int i = 0;
	struct group *grp = getgrnam(groupname.c_str());
	if(grp->gr_mem != NULL)
	{
		while(grp->gr_mem[i] != NULL)
		{
			result.push_back(grp->gr_mem[i]);
			i++;
		}
	}
	struct passwd *pwd;
	while((pwd = getpwent()) != NULL)
	{
		if(pwd->pw_gid == grp->gr_gid && (std::find(result.begin(), result.end(), string(pwd->pw_name)) != result.end()) == false) result.push_back(string(pwd->pw_name));
	}
	endpwent();
	return result;
}

bool addUserToGroup(string username, string groupname)
{
	vector<string> args{"usermod", "-a", "-G", groupname, username};
	return execvp_fork("groupdel", args) == 0;
}

bool delUserFromGroup(string username, string groupname)
{
	vector<string> args{"gpasswd", "-d", username, groupname};
	return execvp_fork("gpasswd", args) == 0;
}
