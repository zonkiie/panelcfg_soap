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

//gsoap ns2 service method-protocol:    userExists SOAP 
//gsoap ns2 service method-style:       userExists rpc 
//gsoap ns2 service method-encoding:    userExists literal 
//gsoap ns2 service method-action:      userExists "" 
int ns__userExists(std::string username, bool& response);

//gsoap ns2 service method-protocol:    checkPassword SOAP 
//gsoap ns2 service method-style:       checkPassword rpc 
//gsoap ns2 service method-encoding:    checkPassword literal 
//gsoap ns2 service method-action:      checkPassword "" 
int ns__checkPassword(std::string username, std::string password, bool& response);

//gsoap ns2 service method-protocol:    listSysRoot SOAP 
//gsoap ns2 service method-style:       listSysRoot rpc 
//gsoap ns2 service method-encoding:    listSysRoot literal 
//gsoap ns2 service method-action:      listSysRoot "" 
int ns__listSysRoot(std::string& response);

//gsoap ns2 service method-protocol:    addUser SOAP 
//gsoap ns2 service method-style:       addUser rpc 
//gsoap ns2 service method-encoding:    addUser literal 
//gsoap ns2 service method-action:      addUser "" 
int ns__addUser(std::string username, std::string password, bool& response);

//gsoap ns2 service method-protocol:    changePassword SOAP 
//gsoap ns2 service method-style:       changePassword rpc 
//gsoap ns2 service method-encoding:    changePassword literal 
//gsoap ns2 service method-action:      changePassword "" 
int ns__changePassword(std::string username, std::string password, bool& response);

//gsoap ns2 service method-protocol:    delUser SOAP 
//gsoap ns2 service method-style:       delUser rpc 
//gsoap ns2 service method-encoding:    delUser literal 
//gsoap ns2 service method-action:      delUser "" 
int ns__delUser(std::string username, bool& response);



#endif
