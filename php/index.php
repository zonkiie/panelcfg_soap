<?php
$client = new SoapClient('http://localhost/~rainer/panelcfg_soap/ns.wsdl', array('location' => "http://localhost/~rainer/panelcfg_soap/panelsoap.cgi", "trace"=>true, 'cache_wsdl' => WSDL_CACHE_NONE, "login"=>"username1", "password"=>"password1", "authentication"=>SOAP_AUTHENTICATION_BASIC));
var_dump($client->__getFunctions());
//echo "Types:" . PHP_EOL;
//var_dump($client->__getTypes());
try{
	print_r($client->getusers());
	print_r($client->authinfo());
}
catch(Exception $e)
{
	echo "Exception:"; print_r($e);
	echo "\nLastRequestHeaders:"; print_r($client->__getLastRequestHeaders());
	echo "\nLastRequest:"; print_r($client->__getLastRequest());
	echo "\nLastResponse:";print_r($client->__getLastResponse());
	echo "\nLastResponseHeaders:";print_r($client->__getLastResponseHeaders());
}
?>

 
