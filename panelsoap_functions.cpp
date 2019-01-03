#include <includes.h>

using namespace std;


/// @see https://www.fluentcpp.com/2017/04/21/how-to-split-a-string-in-c/
/// @see http://www.martinbroadhurst.com/how-to-split-a-string-in-c.html
int ns__getusers(struct soap* soap, vector<string>& userlist)
{
	ifstream ifs("/etc/passwd");
	
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
	response = false;
	struct spwd *spw = getspnam(username.c_str());
	if(spw == NULL) return 401;
	if(s_crypt(password, string(spw->sp_pwdp)) == string(spw->sp_pwdp)) response = true;
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
	if(password.empty()) return 401;
	string salt = make_sha512_salt();
	string enc_password = s_crypt(password, salt);
	if(enc_password.empty()) return 401;
	vector<string> args{"useradd", "-p", enc_password, "-m", username};
	if(!homedir.empty()) args.insert(args.end(), {"-d", homedir});
	if(!shell.empty()) args.insert(args.end(), {"-s", shell});
	if(!groupname.empty()) args.insert(args.end(), {"-g", groupname});
	response = execvp_fork("/usr/sbin/useradd", args);
	return SOAP_OK;
}

int ns__changePassword(struct soap* soap, string username, string password, bool& response)
{
	if(!check_auth(soap)) return 403;
	string enc_password = s_crypt(password, make_sha512_salt());
	vector<string> args{"usermod", "-p", enc_password, username};
	response = execvp_fork("/usr/sbin/usermod", args);
	return SOAP_OK;
}

int ns__changeMyPassword(struct soap* soap, string password, bool& response)
{
	if(!check_credentials(soap)) return 403;
	string enc_password = s_crypt(password, make_sha512_salt());
	vector<string> args{"usermod", "-p", enc_password, soap->userid};
	response = execvp_fork("/usr/sbin/usermod", args);
	return SOAP_OK;
}

int ns__delUser(struct soap* soap, string username, bool& response)
{
	if(!check_auth(soap)) return 403;
	vector<string> args{"userdel", "-r", username};
	response = execvp_fork("/usr/sbin/userdel", args);
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
	response = get_all_vhost_data();
	return SOAP_OK;
}

int ns__getVhostEntryString(struct soap* soap, string vhost_name, string& response)
{
	response = get_vhost_entry_string(vhost_name);
	return SOAP_OK;
}
