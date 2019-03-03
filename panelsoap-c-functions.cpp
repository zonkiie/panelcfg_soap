#include <includes.h>

int ns__getusersC(struct soap* soap, ns__array_string* userlist)
{
    char ** ulist = getUserListC();
    if((userlist->__size = copy_carr_to_soap_carr(soap, &(userlist->__ptr), ulist)) < 0) return 500;
    userlist->__offset = 0;
    free_carr(&ulist);
    return SOAP_OK;
}

int ns__helloC(struct soap* soap, ns__array_string* arr)
{
    arr->__ptr = (char**)soap_malloc(soap, sizeof(char*) * 2);
    arr->__ptr[0] = soap_strdup(soap, "Hello!");
    arr->__ptr[1] = NULL;
    arr->__size = 1;
    return SOAP_OK;
}

int ns__listSysRootC(struct soap* soap, char** response)
{
    char* result = NULL;
    int len = 0;
    const char* cargs[] = {"-la", "/", NULL};
    int state = pexec_to_carr2(&result, &len, "ls", cargs);
    *response = soap_strdup(soap, result);
    free(result);
    if(state != 0) return 500;
	return SOAP_OK;
}

int ns__getAllVhostsC(struct soap* soap, ns__array_string* vhostlist)
{
	//if(!check_auth(soap)) return 403;
    char ** vlist = get_all_vhosts_c_ext();
    if((vhostlist->__size = copy_carr_to_soap_carr(soap, &(vhostlist->__ptr), vlist)) < 0) return 500;
    vhostlist->__offset = 0;
    free_carr(&vlist);
    return SOAP_OK;
}

int ns__getVHostSiteC(struct soap* soap, char * vhostName, char ** response)
{
	//if(!check_auth(soap)) return 403;
    char * site = get_site_for_vhost_c_ext(vhostName);
    *response = soap_strdup(soap, site);
    free(site);
    return SOAP_OK;
}


