<?php
/*
Convert base64 to image
@param base64 String
@return image
@author Fabiano Kuss
*/

	$data = split(";", $_SERVER["QUERY_STRING"]);
	$type = $data[0];
	//Get only image b64 characteres 
	$data = split(",", $data[1]);
	//Format header HTTP for return a image type
	header("Content-type: ".$type);
	//Write image in request
	echo base64_decode($data[1]);
?>
