<?php //header('Content-Type: text/xml');?>

<?php

require_once("md5lib.php");

class Register{

	private $file = "../scripts/data/conf.xml";
	
	function openxml()
	{
		
		//Open XML file
		$fd = fopen($this->file, "r");
		
		//Read XML file
		$data = fread($fd, filesize($this->file));
		
		//Close XML file
		fclose($fd);

		//Get host name and port to connect
		$dom = new DOMDocument();
		$dom->loadXML($data);

		$host = $dom->GetElementsByTagName("host")->item(0)->textContent;
		$port = $dom->GetElementsByTagName("port")->item(0)->textContent;		

		makeRegister($host, $port);


	}

	function makeRegister($username, $password, $mail){
		
		$sock = connect();

	}


	/*
	* Connect in register server
	* @parm string host name 
	* @parm int connection port
	* @return socket 
	*/
	function sendMessage($host, $port){
		
		$sock = sock_create(AF_INET, SOCK_STREAM, 0);
		$sock_data = sock_connect($sock, $addr, $port);




	}

	function make_md5($password){


	}

	function make_xml($username, $password, $mail){

	}

}

$reg = new Register();

$reg->openXML();


/*
$ENC_TYPE="md5";

$username=$_POST["username"];
$mail=$_POST["mail"];
$pwd=$_POST["pwd"];


if($ENC_TYPE == "md5"){
    $salt = $pwd[3] . $pwd[4];
    $magic = "$1$";

    $encpwd = md5crypt($pwd , $salt, $magic);

}else
    $encpwd=sha1($pwd);




$dbConn = new db();

$query = "select * from standart_register('$username', '$mail', '$encpwd', 'shiva')";

$result = $dbConn->executeData($query);

$xmlRet = "<result type='inert'>\n\t<username>$username</username><mail>$mail</mail><sql_result>$result</sql_result></result>";
echo($xmlRet);
*/
?>
