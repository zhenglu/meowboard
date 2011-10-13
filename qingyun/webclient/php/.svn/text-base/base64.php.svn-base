<?php
/**
* CHESSD - WebClient
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* C3SL - Center for Scientific Computing and Free Software
*/

/**
* Load images, scripts and css used in the interface
* from server
*/

/**
* Show load screen to user, and begin to load scripts
*/

?>

<?php

define("MAXSIZE", 1000000);


class Image{
	private $image;
	private $image_new_name;
	private $result;
	private $error;
	private $type;
	private $size;
	private $newsize;
	private $B64String;

	public function getType(){
		if($this->B64String != "")
			return "image/png";
		else
			return 0;
	}

	public function getB64String(){
		return $this->B64String;
	}
	

	public function getResult(){
		return $this->result;

	}


	public function getError(){
		return $this->error;
	}

	public function Image($img, $height, $width){
		$this->image = $img["tmp_name"];
		$this->result = 0;
		$this->error = "";
		$this->type = $img["type"];
		$this->size = $img['size'];
		$this->newsize = array($height, $width);
		if($this->validateImage())
				$this->B64String = $this->readNewImage();

	}
		


	public function imageResize(){

		
		list($newwidth, $newheight) = $this->newsize;
		
		$posX = 0;
		$posY = 0;

		list($width, $height) = getimagesize($this->image);

		$scaleX = $newwidth / $width;
		$scaleY = $newheight / $height;

		if($scaleX > $scaleY){
			$snewidth = intval($width * $scaleY);
			$snewheight = intval($height * $scaleY);
			$posX = ($newwidth - $snewidth) / 2;
		}else{
			$snewidth = intval($width * $scaleX);
			$snewheight = intval($height * $scaleX);
			$posY = ($newheight - $snewheight) / 2;
		}
		

		$thumb = imagecreate($newwidth, $newheight);
		imagecolorallocate($thumb, 255, 255, 255);
		$source = $this->correctImage($this->image, $this->type);

		 $_result = imagecopyresized($thumb, $source, $posX, $posY, 0, 0, $snewidth, $snewheight,$width,$height);

		return imagepng($thumb,  $this->image.".png");

		
	}


	private function correctImage($img, $type){

		if(eregi("^image\/(jpeg|jpg)$", $type)){
			 return imagecreatefromjpeg($img);
		}else if(eregi("^image\/(gif)$", $type))
			return imagecreatefromgif($img);
		else if(eregi("^image\/(png)$", $type))
			return imagecreatefrompng($img);

	}
	
	private function readNewImage(){
		$type = $image["type"];
		//Open temporary file
		$fd = fopen($this->image.".png", "rb");
		$convert=fread($fd, $this->size);
		//Convert binary file to printable caracteres
		$convert = base64_encode($convert);
		$size=strlen($convert);
		fclose($fd);
		return $convert;
	}

	private function validateImage(){
		if(eregi("^image\/(jpeg|jpg|png|gif)$", $this->type)){
			//Validate image max size
			if($this->size < MAXSIZE){
				//Upload image to temp file
				if(is_uploaded_file($this->image)){
					return($this->imageResize($this->image, $this->type));		

				}else{ 
					$this->result = 1;
					$this->error="Error: Invalid file";
				}
			}else{
				$this->result = 2;
				$this->error="Error: Invalid file size ".$image['size'];
			}
		}else{
			$this->result = 3;
			$this->error="Error: Invalid file type ".$image['type'];
		}
	}

}


$img = $_FILES["image"];
$img_width = $_POST["width"];
$img_height = $_POST["height"];
$conv = new Image($img, $img_width , $img_height);


?>

<HTML>
<body bgcolor="black">
<img src="" id="img">
<script>
	//Call this function on create page
	imageResult();

	/*This function recive PHP results and call 
	* the function IMAGE_B64Img with parameter basead in PHP request
	* @parm 
	* @return
	* @author Fabiano Kuss
	*/

	function imageResult(){

		document.getElementById("img").src="base64_decode.php?<?php echo $conv->getType();?>;base64,<?php echo $conv->getB64String();?>";
		if(!<?php echo $conv->getResult();?>)
			parent.IMAGE_B64Img("<?php echo $conv->getB64String();?>", "<?php echo $conv->getType();?>", "<?php echo $conv->getError();?>");
		else
			parent.IMAGE_B64Img("<?php echo $conv->getB64String();?>", "", <?php echo $conv->getResult();?>);
		
	}
</script>
</body>
</HTML>

