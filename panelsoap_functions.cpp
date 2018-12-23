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

/// @see https://stackoverflow.com/questions/35390912/proper-way-to-get-file-size-in-c
int get_filesize(char * filename)
{
	int fsize = 0;
	if(access(filename, F_OK)) // file not found
		return -1;
	struct stat buffer;
	int         status;

	status = stat("path to file", &buffer);
	if(status == 0) {
	// size of file is in member buffer.st_size;
		fsize = buffer.st_size;
	}
	return fsize;
}

/// @see https://stackoverflow.com/questions/9318642/publishing-wsdl-with-gsoap-stand-alone-server
int http_get(struct soap* soap)
{
	char *file = strdup("ns.wsdl");
	char *wsdl = NULL;
	FILE *f = NULL;
	int fsize = get_filesize(file);
	int state = SOAP_OK;
	if(fsize < 0) // file not found
	{
		free(file);
		file = (char*)malloc(2048);
		sprintf(file, "%s/%s", dirname(progpath), "ns.wsdl");
		if((fsize = get_filesize(file)) < 0)
		{
			state = 401;
			goto http_get_free;
		}
	}
	f = fopen(file, "r");
	if(!f)
	{
		state = 401;
		goto http_get_free;
	}
	wsdl = (char*)malloc(fsize + 2);
	if(!fgets(wsdl, fsize, f))
	{
		state = 403;
		goto http_get_free;
	}
	
	soap_response(soap, SOAP_XML_TREE); // HTTP response header with text/xml
	soap_send(soap, (const char*)wsdl);
	soap_end_send(soap);
http_get_free:
	if(file) free(file);
	if(f) fclose(f);
	if(wsdl) free(wsdl);
	return state;
}
