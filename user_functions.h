#ifndef __user_functions__
#define __user_functions__

vector<string> getUserList();
bool infoUser(int & error_status, string& username, string& homedir, string& shell, string& groupname, int & uid);
bool delUser(string username);
bool changePassword(string username, string password);
bool checkPassword(int & error_status, string username, string password);
bool addUser(int & error_status, string username, string password, string homedir, string shell, string groupname);
bool changeShell(string username, string shell);
vector<string> getUserGroupMembership(string username);
bool addGroup(string groupname);
bool delGroup(string groupname);
bool setGroupMembers(string groupname, vector<string> members);
vector<string> getGroupMembers(string groupname);
bool addUserToGroup(string username, string groupname);
bool delUserFromGroup(string username, string groupname);

#endif
