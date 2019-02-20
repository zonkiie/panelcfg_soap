#ifndef __user_functions__
#define __user_functions__

bool infoUser(int & error_status, string& username, userinfo& info);
bool delUser(string username);
bool changePassword(string username, string password);
bool addUser(int & error_status, string username, string password, string homedir, string shell, string groupname);
bool changeShell(string username, string shell);
bool addGroup(string groupname);
bool delGroup(string groupname);
bool setGroupMembers(string groupname, vector<string> members);
bool addUserToGroup(string username, string groupname);
bool delUserFromGroup(string username, string groupname);
soap_quotadata getUserQuotaData(string username, string filesystem = "");
bool setUserQuotaData(soap_quotadata qd);

#endif
