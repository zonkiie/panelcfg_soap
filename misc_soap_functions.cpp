#include <includes.h>

/// @see https://stackoverflow.com/questions/9318642/publishing-wsdl-with-gsoap-stand-alone-server
int http_get2(struct soap* soap)
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

/// @see https://www.genivia.com/doc/guide/html/index.html#get
int http_get(struct soap *soap) 
{
	FILE *fd = NULL;
	char *s = strchr(soap->path, '?'); 
	if (!s || strcmp(s, "?wsdl")) 
		return SOAP_GET_METHOD;
	char file[2048];
	int fsize = get_filesize(file);
	strcpy(file, "ns.wsdl");
	
	
	if(fsize < 0) // file not found
	{
		sprintf(file, "%s/%s", dirname(progpath), "ns.wsdl");
		if((fsize = get_filesize(file)) < 0) return 404;
	}
	
	
	fd = fopen(file, "rb"); // open WSDL file to copy 
	if (!fd) 
		return 404; // return HTTP not found error 
	soap->http_content = "text/xml"; // HTTP header with text/xml content 
	if (soap_response(soap, SOAP_FILE) == SOAP_OK)
	{
		while (1)
		{
			size_t r = fread(soap->tmpbuf, 1, sizeof(soap->tmpbuf), fd); 
			if (!r || soap_send_raw(soap, soap->tmpbuf, r)) 
				break;
		} 
	}
	fclose(fd); 
	soap_end_send(soap);
	return soap_closesock(soap);
}

 