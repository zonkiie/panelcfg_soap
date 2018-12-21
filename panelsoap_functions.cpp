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

int ns__authinfo(struct soap* soap, string & response)
{
	response = "Username:" + (soap->userid != NULL?string(soap->userid):"(NULL)") + ", Password:" + (soap->passwd != NULL?string(soap->passwd):"(NULL)");
	return SOAP_OK;
}
