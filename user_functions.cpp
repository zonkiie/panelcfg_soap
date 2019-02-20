#include <includes.h> 

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

bool infoUser(int & error_status, string& username, userinfo& info)
{
	struct passwd *pwd = getpwnam(username.c_str());
	if(pwd == 0)
	{
		error_status = 404;
		return false;
	}
	info.homedir = string(pwd->pw_dir == NULL?"":pwd->pw_dir);
	info.shell = string(pwd->pw_shell == NULL?"":pwd->pw_shell);
	struct group * grp = getgrgid(pwd->pw_gid);
	info.groupname = string(grp->gr_name == NULL?"":grp->gr_name);
	info.uid = pwd->pw_uid;
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

bool addUserToGroup(string username, string groupname)
{
	vector<string> args{"usermod", "-a", "-G", groupname, username};
	return execvp_fork("usermod", args) == 0;
}

bool delUserFromGroup(string username, string groupname)
{
	vector<string> args{"gpasswd", "-d", username, groupname};
	return execvp_fork("gpasswd", args) == 0;
}

/// reads the quota from user username.
/// if filesystem is not given, the device of user's homedir will be used.
soap_quotadata getUserQuotaData(string username, string filesystem)
{
	soap_quotadata result;
	int error_status = 0;
	userinfo uinfo;
	if(!infoUser(error_status, username, uinfo)) return result;
	char* c_mountpoint = NULL;
	string answer, line, path_to_read, mountpoint;
	if(filesystem != "") path_to_read = filesystem;
	else path_to_read = uinfo.homedir;
	if(NULL == (c_mountpoint = getdev(path_to_read.c_str())))
	{
		free(c_mountpoint);
		return result;
	}
	if(c_mountpoint != NULL) mountpoint = string(c_mountpoint);
	free(c_mountpoint);
	if(mountpoint == "") return result;
	vector<string> args{"-d", "-u", to_string(uinfo.uid), mountpoint};
	answer = pexec_read("quotatool", args);
	regex rex_quotaline("(\\d+) ([\\w/]+) (\\d+) (\\d+) (\\d+) (\\d+) (\\d+) (\\d+) (\\d+)");
	smatch sm;
	stringstream strs(answer);
	if(!answer.empty())
	{
		while(getline(strs, line, '\n'))
		{
			if(regex_match(line, sm, rex_quotaline)) result.username = username, result.device = sm.str(2), result.blocks_used = stoull(sm.str(3)), result.block_softlimit = stoull(sm.str(4)), result.block_hardlimit = stoull(sm.str(5)), result.inodes_used = stoull(sm.str(7)), result.inode_softlimit = stoull(sm.str(8)), result.inode_hardlimit = stoull(sm.str(9));
		}
	}
	return result;
}

bool setUserQuotaData(soap_quotadata qd)
{
	userinfo uinfo;
	int error_status;
	if(!infoUser(error_status, qd.username, uinfo)) return false;
	char* c_mountpoint = NULL;
	string answer, line, path_to_read, mountpoint;
	if(qd.device != "") path_to_read = qd.device;
	else path_to_read = uinfo.homedir;
	if(NULL == (c_mountpoint = getdev(path_to_read.c_str())))
	{
		free(c_mountpoint);
		return false;
	}
	if(c_mountpoint != NULL) mountpoint = string(c_mountpoint);
	free(c_mountpoint);
	if(mountpoint == "") return false;
	vector<string> args{"quotatool", "-u", to_string(uinfo.uid), "-b", "-l", to_string(qd.block_hardlimit), "-q", to_string(qd.block_softlimit), mountpoint};
	if(execvp_fork("quotatool", args) != 0) return false;
	args = {"quotatool", "-u", to_string(uinfo.uid), "-i", "-l", to_string(qd.inode_hardlimit), "-q", to_string(qd.inode_softlimit), mountpoint};
	if(execvp_fork("quotatool", args) != 0) return false;
	return true;
}
