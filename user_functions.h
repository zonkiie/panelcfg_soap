#ifndef __user_functions__
#define __user_functions__

bool infoUser(int & error_status, string& username, userinfo& info);
soap_quotadata getUserQuotaData(string username, string filesystem = "");
bool setUserQuotaData(soap_quotadata qd);

#endif
