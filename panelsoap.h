// panelsoap.h
#ifndef __panelsoap__
#define __panelsoap__

//#import "wsse.h"
#import "stlvector.h"
#import "types.h"

//gsoap ns2 schema namespace:   urn:ns
//gsoap ns2 schema form:        unqualified
//gsoap ns2 service name:       panelcfg_soap
//gsoap ns2 service type:       panelcfg_soapPortType
//gsoap ns2 service protocol:   SOAP
//gsoap ns2 service style:      rpc
//gsoap ns2 service encoding:   literal
/*gsoap ns2 service port2:       http://localhost/~rainer/panelcfg_soap/panelsoap.cgi */
//gsoap ns2 service namespace:  urn:panelsoap
//gsoap ns2 service transport:  http://schemas.xmlsoap.org/soap/http

//gsoap ns2 service method-action:      getusers "" 
int ns__getusers(std::vector<std::string>& userlist);

//gsoap ns2 service method-action:      getusersC "" 
int ns__getusersC(ns__array_string* userlist);

//gsoap ns2 service method-action:      soapinfo "" 
int ns__soapinfo(std::string& response);

//gsoap ns2 service method-action:      helloC "" 
int ns__helloC(ns__array_string* arr);

//gsoap ns2 service method-action:      infoUser "" 
int ns__infoUser(std::string username, userinfo& uinfo);

//gsoap ns2 service method-action:      userExists "" 
int ns__userExists(std::string username, bool& response);

//gsoap ns2 service method-action:      checkPassword "" 
int ns__checkPassword(std::string username, std::string password, bool& response);

//gsoap ns2 service method-action:      listSysRoot "" 
int ns__listSysRoot(std::string& response);

//gsoap ns2 service method-action:      listSysRootC "" 
int ns__listSysRootC(char** response);

//gsoap ns2 service method-action:      addUser "" 
int ns__addUser(std::string username, std::string password, std::string homedir = "", std::string shell = "", std::string groupname = "", bool& response);

//gsoap ns2 service method-action:      changePassword "" 
int ns__changePassword(std::string username, std::string password, bool& response);

//gsoap ns2 service method-action:      changeMyPassword "" 
int ns__changeMyPassword(std::string password, bool& response);

//gsoap ns2 service method-action:      delUser "" 
int ns__delUser(std::string username, bool& response);

//gsoap ns2 service method-action:      changeShell "" 
int ns__changeShell(std::string username, std::string shell = "", bool& response);

//gsoap ns2 service method-action:      getUserGroupMembership "" 
int ns__getUserGroupMembership(std::string username, std::vector<std::string>& response);

//gsoap ns2 service method-action:      setGroupMembers "" 
int ns__setGroupMembers(std::string groupname, std::vector<std::string> members, bool& response);

//gsoap ns2 service method-action:      getGroupMembers "" 
int ns__getGroupMembers(std::string groupname, std::vector<std::string>& response);

//gospa ns2 service method-documentation: Add a group
//gsoap ns2 service method-action:      addGroup "" 
int ns__addGroup(std::string groupname, bool& response);

//gsoap ns2 service method-action:      delGroup "" 
int ns__delGroup(std::string groupname, bool& response);

//gsoap ns2 service method-action:      getUserQuotaData ""
int ns__getUserQuotaData(std::string username, std::string filesystem = "", quotadata& response);

//gsoap ns2 service method-action:      setUserQuotaData ""
int ns__setUserQuotaData(quotadata qd, bool& response);

//gsoap ns2 service method-action:      getAllVhosts "" 
int ns__getAllVhosts(std::vector<std::string>& response);

//gsoap ns2 service method-action:      getAllVhostsC "" 
int ns__getAllVhostsC(ns__array_string* vhostlist);

//gsoap ns2 service method-action:      getVHostSiteC "" 
int ns__getVHostSiteC(char * vhostName, char ** response);

//gsoap ns2 service method-action:      getAllVhostData "" 
int ns__getAllVhostData(std::vector<vhost>& response);

//gsoap ns2 service method-action:      getVhostEntryString "" 
int ns__getVhostEntryString(std::string vhostName, std::string& response);

//gsoap ns2 service method-action:      addVhost "" 
int ns__addVhost(std::string siteName, std::string vhostName, std::string documentRoot, bool& response);

//gsoap ns2 service method-action:      changeVhost "" 
int ns__changeVhost(std::string siteName, std::string vhostName, std::string documentRoot, bool& response);

//gsoap ns2 service method-action:      delVhost "" 
int ns__delVhost(std::string siteName, std::string vhostName, bool& response);

//gsoap ns2 service method-action:      setVhostEntryString "" 
int ns__setVhostEntryString(std::string siteName, std::string vhostName, std::string vhostString, bool& response);


#endif
