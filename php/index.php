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
		'local_cert'  => "",
	)
);
var_dump($client->__getFunctions());
//echo "Types:" . PHP_EOL;
//var_dump($client->__getTypes());
try{
	#print_r($client->getusers());
	#print_r($client->soapinfo());
	#print_r($client->userExists(array("username"=>"root")));
	#print_r($client->addUser(array("username"=>"sample", "password"=>"sample1", "homedir"=>"", "shell"=>"", "groupname"=>"")));
	#print_r($client->infoUser(array("username"=>"sample")));
	#print_r($client->getUserQuotaData(array("username"=>"sample")));
	#print_r($client->setUserQuotaData(array("qd"=>array("device"=>"", "groupname"=>"", "username"=>"sample", "block-softlimit"=>"10000", "block-hardlimit"=>"10000", "inode-softlimit"=>"100", "inode-hardlimit"=>"100", "blocks-used"=>0, "inodes-used"=>0))));
	
	#print_r($client->getUserQuotaData(array("username"=>"sample")));
	#print_r($client->checkPassword(array("username"=>"sample", "password"=>"sample1")));
	#print_r($client->changePassword(array("username"=>"sample", "password"=>"sample2")));
	#print_r($client->checkPassword(array("username"=>"sample", "password"=>"sample2")));
	#print_r($client->delUser(array("username"=>"sample")));
	#print_r($client->getUserGroupMembership(array("username"=>"admin")));
	#print_r($client->getGroupMembers(array("groupname"=>"admin")));
	#print_r($client->addGroup(array("groupname"=>"samplegroup")));
	#print_r($client->getGroupMembers(array("groupname"=>"samplegroup")));
	#print_r($client->setGroupMembers(array("groupname"=>"samplegroup", "members"=>array("admin", "sample"))));
	#print_r($client->getGroupMembers(array("groupname"=>"samplegroup")));
	#print_r($client->getUserGroupMembership(array("username"=>"sample")));
	#print_r($client->delUser(array("username"=>"sample")));
	#print_r($client->delGroup(array("groupname"=>"samplegroup")));
	#print_r($client->getAllVhosts());
	#print_r($client->getAllVhostData());
	#print_r($client->getVhostEntryString(array("vhostName"=>"mysite1.de")));
	#print_r($client->addVhost(array("siteName"=>"mysite1", "vhostName"=>"sub2.mysite1.de", "documentRoot"=>"/home/rainer/Programmierung/cpp")));
	#print_r($client->delVhost(array("siteName"=>"mysite1", "vhostName"=>"sub2.mysite1.de")));
	#print_r($client->listSysRoot());
	print_r($client->listSysRootC());
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

 
