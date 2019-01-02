<?php
error_reporting(E_ALL);

set_exception_handler(function($exception) {
	echo "Exception:";
	print_r($exception);
});

//$base_url = 'https://localhost/~rainer/panelcfg_soap'; $wsdl_url = $base_url . '/ns.wsdl';
$base_url = 'http://localhost:8080/'; $wsdl_url = $base_url . '?wsdl';

$client = new SoapClient($wsdl_url, 
	array(
		'location' => $base_url . '/panelsoap.cgi',
		'trace'=>true,
		'cache_wsdl' => WSDL_CACHE_NONE,
		'login'=>"admin",
		'password'=>"password1",
		'authentication'=>SOAP_AUTHENTICATION_BASIC,
		'soap_version'=>SOAP_1_2,
		//'use' => SOAP_ENCODED,
		'style'=> SOAP_RPC,
		'use'      => SOAP_LITERAL,
		'connection_timeout'=>10,
	)
);
//var_dump($client->__getFunctions());
//echo "Types:" . PHP_EOL;
//var_dump($client->__getTypes());
try{
	#print_r($client->getusers());
	#print_r($client->soapinfo());
	#print_r($client->userExists(array("username"=>"root")));
	#print_r($client->addUser(array("username"=>"sample", "password"=>"sample1")));
	#print_r($client->checkPassword(array("username"=>"sample", "password"=>"sample1")));
	#print_r($client->changePassword(array("username"=>"sample", "password"=>"sample2")));
	#print_r($client->checkPassword(array("username"=>"sample", "password"=>"sample2")));
	#print_r($client->delUser(array("username"=>"sample")));
	print_r($client->getAllVhosts());
	print_r($client->getAllVhostData());
	#print_r($client->listSysRoot());
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

 
