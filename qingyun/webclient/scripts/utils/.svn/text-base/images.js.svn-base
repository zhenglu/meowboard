/*
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
* This file contais all function used to decode/encode images to base64
*/


/**
* Create input file to user
*
* @param        FormId is Form identificator
* @param 	Action is script source
* @return       Form HTML element
* @author       Fabiano and Rubens
*/
function IMAGE_CreateFormToEncode(FormId, Action){
	var Form, File, MaxSizeInput;

	Form = document.createElement("Form");
	Form.id= FormId;
	Form.action = Action;
	Form.method = "post";
	Form.name = FormId;
	Form.enctype="multipart/Form-data"
	MaxSizeInput = document.createElement("input");
	MaxSizeInput.type = "hidden";
	MaxSizeInput.name = "MAX_FILE_SIZE";
	MaxSizeInput.value = "1000000";
	File = document.createElement("input");
	File.type = "file";
	File.name = "image";
	Width = document.createElement("input");
	Width.type = "hidden";
	Width.name = "width";
	Width.value = "50";
	Height = document.createElement("input");
    Height.type = "hidden";
	Height.name = "height"
    Height.value = "50";

	Form.appendChild(MaxSizeInput);
	Form.appendChild(Height);
	Form.appendChild(Width);
	Form.appendChild(File);
	return Form;
}


/**
* This function is used to show image base64 in IE6
*
* @param 	ImgSrc is the image source in base64 Firefox standart
* @return       Image base64 in IE Format
* @author       Fabiano and Rubens
*/
function IMAGE_ImageDecode(ImgSrc){
	//DefaultPHP is loaded from conf.xml
	return "php/base64_decode."+MainData.DefaultPHP+"?"+ImgSrc.slice(5);
}

/**
* This function get image and convert to base64
*
* @param        FormId is Form identificator
* @return       void
* @author       Fabiano and Rubens
*/
function IMAGE_ImageEncode(FormId){
	var Frame, Body, Form;
	
	Frame = document.createElement("iFrame");
	Frame.name = "nda";
	Frame.setAttribute("style","width: 0; height: 0; border: none;");
	Body = document.getElementsByTagName("body")[0];
	Body.appendChild(Frame);
	Form = document.getElementById(FormId);
	Form.target = Frame.name;
	Form.submit();
}

/**
* When image is loaded this function is called 
*
* @param        Image is the image base 64
* @param        Type is the image type
* @return       void
* @author       Fabiano and Rubens
*/
function IMAGE_B64Img(Image, Type){
	var Profile;
	var Img;

	//if type == ""; error!!!!
	if(Type == "")
	{
		WINDOW_Alert(UTILS_GetText("profile_load_error"), UTILS_GetText("profile_load_error_title"));
		return false;
	}
	Img = "data:"+Type+";base64,"+Image;

	// Update user profile image
	Profile = MainData.GetProfile(MainData.Username+"@"+MainData.Host);
	if(Profile != null)
	{
		Profile.Profile.SetImgType(Type);
		Profile.Profile.SetImg64(Image);
		Profile.Profile.SetUserImg(Img);
	}
	return true;
}
