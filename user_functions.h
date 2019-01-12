#ifndef __user_functions__
#define __user_functions__

vector<string> getUserList();
bool userDel(string username);
bool changePassword(string username, string password);
bool checkPassword(int & error_status, string username, string password);
bool addUser(int & error_status, string username, string password, string homedir, string shell, string groupname);

#endif
