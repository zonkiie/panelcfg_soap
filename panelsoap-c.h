// panelsoap.h
#ifndef __panelsoap_c__
#define __panelsoap_c__

//gsoap ns2 service method-action:      getusersC "" 
int ns__getusersC(ns__array_string* userlist);

//gsoap ns2 service method-action:      helloC "" 
int ns__helloC(ns__array_string* arr);

//gsoap ns2 service method-action:      listSysRootC "" 
int ns__listSysRootC(char** response);

//gsoap ns2 service method-action:      getAllVhostsC "" 
int ns__getAllVhostsC(ns__array_string* vhostlist);

//gsoap ns2 service method-action:      getVHostSiteC "" 
int ns__getVHostSiteC(char * vhostName, char ** response);



#endif
