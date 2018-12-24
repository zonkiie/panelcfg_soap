// panelsoap.h
#ifndef __panelsoap__
#define __panelsoap__

#import "stlvector.h"

//gsoap ns2 schema namespace:   urn:ns
//gsoap ns2 schema form:        unqualified
//gsoap ns2 service name:       panelcfg_soap
//gsoap ns2 service type:       panelcfg_soapPortType
/*gsoap ns2 service port2:       http://localhost/~rainer/panelcfg_soap/panelsoap.cgi */
//gsoap ns2 service namespace:  urn:panelsoap
//gsoap ns2 service transport:  http://schemas.xmlsoap.org/soap/http

//gsoap ns2 service method-protocol:    getusers SOAP 
//gsoap ns2 service method-style:       getusers rpc 
//gsoap ns2 service method-encoding:    getusers literal
//gsoap ns2 service method-action:      getusers "" 
int ns__getusers(std::vector<std::string>& userlist);

//gsoap ns2 service method-protocol:    getusers SOAP 
//gsoap ns2 service method-style:       getusers rpc 
//gsoap ns2 service method-encoding:    getusers literal 
//gsoap ns2 service method-action:      getusers "" 
int ns__soapinfo(std::string& response);

//gsoap ns2 service method-protocol:    user_exists SOAP 
//gsoap ns2 service method-style:       user_exists rpc 
//gsoap ns2 service method-encoding:    user_exists literal 
//gsoap ns2 service method-action:      user_exists "" 
int ns__userExists(std::string username, bool& response);

int ns__checkPassword(std::string username, std::string password, bool& response);

#endif
