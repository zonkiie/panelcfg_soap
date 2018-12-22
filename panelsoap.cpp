#include <includes.h>
#include <soap_includes.h>

int main()
{
	struct soap *soap = soap_new();
	soap->authrealm = "panelsoap";
	return soap_serve(soap);
	//return soap_serve(soap_new());
	//start_service();
}
