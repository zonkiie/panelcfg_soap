// hello.h
#ifndef __hello__
#define __hello__

#import "stlvector.h"

//gsoap ns2 schema namespace:   urn:ns
//gsoap ns2 schema form:        unqualified
//gsoap ns2 service name:       panelcfg_soap
//gsoap ns2 service type:       panelcfg_soapPortType
//gsoap ns2 service port:       http://localhost/~rainer/panelcfg_soap/panelsoap.cgi 
//gsoap ns2 service namespace:  urn:panelsoap
//gsoap ns2 service transport:  http://schemas.xmlsoap.org/soap/http

//gsoap ns2 service method-protocol:    getusers SOAP 
//gsoap ns2 service method-style:       getusers rpc 
//gsoap ns2 service method-encoding:    getusers http://schemas.xmlsoap.org/soap/encoding/ 
//gsoap ns2 service method-action:      getusers "" 
int ns__getusers(std::vector<std::string>& userlist);

#endif
