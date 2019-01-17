#include <includes.h>

using namespace std;


int ns__getusers(struct soap* soap, vector<string>& userlist)
{
	userlist = getUserList();
	return SOAP_OK;
}

/// Authentication info seems to only work in standalone mode.
int ns__soapinfo(struct soap* soap, string & response)
{
	//response = "Username:" + (soap->userid != NULL?string(soap->userid):"(NULL)") + ", Password:" + (soap->passwd != NULL?string(soap->passwd):"(NULL)");
	std::stringstream rstr;
	rstr << "Soap_Info:";
	//if(soap->userid == NULL || soap->passwd == NULL) { soap->authrealm = "panelsoap"; return 401; }
	
	if(soap->userid != NULL) rstr << " Username:" << soap->userid;
	if(soap->passwd != NULL) rstr << " Password:" << soap->passwd;
	if(soap->proxy_userid != NULL) rstr << " proxy_userid:" << soap->proxy_userid;
	if(soap->proxy_passwd != NULL) rstr << " proxy_passwd:" << soap->proxy_passwd;
	if(soap->http_content != NULL) rstr << " http_content:" << soap->http_content;
	if(soap->authrealm != NULL) rstr << " authrealm:" << soap->authrealm;
	//if(soap->bearer != NULL) rstr << " bearer:" << soap->bearer;
	
	rstr << " Soap Info End.";
	response = rstr.str();
	return SOAP_OK;
}

int ns__userExists(struct soap* soap, string username, bool& response)
{
	struct passwd* pw = getpwnam(username.c_str());
	response = (pw != NULL);
	return SOAP_OK;
}

int ns__checkPassword(struct soap* soap, string username, string password, bool& response)
{
	int error_status = 0;
	response = checkPassword(error_status, username, password);
	if(response == false && error_status == 401) return 401;
	if(response == false) sleep(3);
	return SOAP_OK;
}

int ns__listSysRoot(struct soap* soap, string& response)
{
	vector<string> els{"-la", "/"};
	response = pexec_read("/bin/ls", els);
	return SOAP_OK;
}

int ns__addUser(struct soap* soap, string username, string password, string homedir, string shell, string groupname, bool& response)
{
	if(!check_auth(soap)) return 403;
	int error_status = 0;
	response = addUser(error_status, username, password, homedir, shell, groupname);
	if(error_status != 0) return error_status;
	return SOAP_OK;
}

int ns__changePassword(struct soap* soap, string username, string password, bool& response)
{
	if(!check_auth(soap)) return 403;
	response = changePassword(username, password);
	return SOAP_OK;
}

int ns__changeMyPassword(struct soap* soap, string password, bool& response)
{
	if(!check_credentials(soap)) return 403;
	response = changePassword(soap->userid, password);
	return SOAP_OK;
}

int ns__delUser(struct soap* soap, string username, bool& response)
{
	if(!check_auth(soap)) return 403;
	response = delUser(username);
	return SOAP_OK;
}

int ns__infoUser(struct soap* soap, string username, userinfo& uinfo)
{
	if(!check_auth(soap)) return 403;
	int error_status;
	if(infoUser(error_status, username, uinfo) == false) return error_status;
	return SOAP_OK;
}

int ns__changeShell(struct soap* soap, string username, string shell, bool& response)
{
	if(!check_auth(soap)) return 403;
	response = changeShell(username, shell);
	return SOAP_OK;
}

int ns__addGroup(struct soap* soap, string groupname, bool& response)
{
	if(!check_auth(soap)) return 403;
	response = addGroup(groupname);
	return SOAP_OK;
}

int ns__delGroup(struct soap* soap, string groupname, bool& response)
{
	if(!check_auth(soap)) return 403;
	response = delGroup(groupname);
	return SOAP_OK;
}

int ns__getUserGroupMembership(struct soap* soap, string username, vector<string>& response)
{
	if(!check_auth(soap)) return 403;
	response = getUserGroupMembership(username);
	return SOAP_OK;
}

int ns__getGroupMembers(struct soap* soap, string groupname, vector<string>& response)
{
	if(!check_auth(soap)) return 403;
	response = getGroupMembers(groupname);
	return SOAP_OK;
}

int ns__setGroupMembers(struct soap* soap, string groupname, vector<string> members, bool& response)
{
	if(!check_auth(soap)) return 403;
	response = setGroupMembers(groupname, members);
	return SOAP_OK;
}

int ns__getUserQuotaData(struct soap* soap, string username, string filesystem, quotadata& response)
{
	if(!check_auth(soap)) return 403;
	response = getUserQuotaData(username, filesystem);
	return SOAP_OK;
}

int ns__setUserQuotaData(struct soap* soap, quotadata qd, bool& response)
{
	if(!check_auth(soap)) return 403;
	response = setUserQuotaData(qd);
	return SOAP_OK;
}

int ns__getAllVhosts(struct soap* soap, vector<string>& response)
{
	if(!check_auth(soap)) return 403;
	response = get_all_vhosts();
	return SOAP_OK;
}

int ns__getAllVhostData(struct soap* soap, vector<vhost>& response)
{
	if(!check_auth(soap)) return 403;
	response = get_all_vhost_data();
	return SOAP_OK;
}

int ns__getVhostEntryString(struct soap* soap, string vhostName, string& response)
{
	if(!check_auth(soap)) return 403;
	response = get_vhost_entry_string(vhostName);
	return SOAP_OK;
}

int ns__addVhost(struct soap* soap, string siteName, string vhostName, string documentRoot, bool& response)
{
	if(!check_auth(soap)) return 403;
	response = add_vhost(siteName, vhostName, documentRoot);
	reload_apache();
	return SOAP_OK;
}

int ns__changeVhost(struct soap* soap, string siteName, string vhostName, string documentRoot, bool& response)
{
	if(!check_auth(soap)) return 403;
	response = change_vhost(siteName, vhostName, documentRoot);
	reload_apache();
	return SOAP_OK;
}

int ns__delVhost(struct soap* soap, string siteName, string vhostName, bool& response)
{
	if(!check_auth(soap)) return 403;
	response = del_vhost(siteName, vhostName);
	reload_apache();
	return SOAP_OK;
}

int ns__setVhostEntryString(struct soap* soap, string siteName, string vhostName, string vhostString, bool& response)
{
	if(!check_auth(soap)) return 403;
	response = set_vhost_string(siteName, vhostName, vhostString);
	reload_apache();
	return SOAP_OK;
}
