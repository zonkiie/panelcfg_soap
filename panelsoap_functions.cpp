#include <includes.h>

using namespace std;

// https://www.fluentcpp.com/2017/04/21/how-to-split-a-string-in-c/
int ns__getusers(struct soap* soap, vector<string>& userlist)
{
	ifstream ifs("/etc/passwd");
	
	string line;
	while(getline(ifs, line))
	{
		vector<string> entries;
		boost::split(entries, line, [](char c){return c == ':';});
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

int ns__soapinfo(struct soap* soap, string & response)
{
	//response = "Username:" + (soap->userid != NULL?string(soap->userid):"(NULL)") + ", Password:" + (soap->passwd != NULL?string(soap->passwd):"(NULL)");
	std::stringstream rstr;
	rstr << "Soap_Info:";
	if(soap->userid == NULL || soap->passwd == NULL)
	{
		soap->authrealm = "panelsoap";
		return 401;
	}
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
