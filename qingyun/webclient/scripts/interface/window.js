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
* Window Interface Object
*/

function WindowObj(Height, Width, Div, Title, CloseCommands)
{
	//Constructor and attribute
	var tmp = WINDOW_CreateWindow(Height, Width, Div, Title, CloseCommands);


	this.window = tmp.Div; //WindowObject;
	this.eventButtons = tmp.Close; //Event buttons;

	//Methods
	this.show = WINDOW_ShowWindow;
	this.close = WINDOW_CloseWindow;
	this.setId = WINDOW_SetId;
	this.setSize = WINDOW_SetSize;
	this.setTitle = WINDOW_SetTitle;
	this.focus = WINDOW_SetFocus;
	this.blur = WINDOW_SetBlur;
	this.setZIndex = WINDOW_SetZIndex;
	this.getZIndex = WINDOW_GetZIndex;
	this.pushEventButtons = WINDOW_ConcatEventButtons;
}


function WINDOW_ShowWindow(Element, Top, Left)
{
	var RandomTop, RandomLeft;
	var TopTmp = 0;
	var LeftTmp = 0;


	if(Element == null)
	{
		document.body.appendChild(this.window);

		// Set random top and left position
		RandomTop = (Math.floor((Math.random()*10000)) % 60) * ((-1)*Math.floor(Math.random()*10000)%2);
		RandomLeft = (Math.floor((Math.random()*10000)) % 60) * ((-1)*Math.floor(Math.random()*10000)%2);

		// Check Internet Explorer
		if(MainData.Browser == 0)
		{

			if(Top != null)
			{
				TopTmp += Top;
			}
			else
			{
				TopTmp += (document.body.clientHeight/2) - (document.body.clientHeight/5)+ RandomTop;
			}

			if(Left != null)
			{
				LeftTmp += Left;
			}
			else
			{
				LeftTmp += (document.body.clientWidth/2) - (document.body.clientWidth/10) + RandomLeft;
			}
		}
		else
		{
			if(Top != null)
			{
				TopTmp += Top;
			}
			else
			{
				TopTmp += (window.innerHeight/2) - (window.innerHeight/5)+ RandomTop;
			}

			if(Left != null)
			{
				LeftTmp += Left;
			}
			LeftTmp += (window.innerWidth/2) - (window.innerWidth/10) + RandomLeft;
		}


		this.window.style.top = TopTmp+"px";
		this.window.style.left = LeftTmp+"px";
	}
	else
	{
		Element.appendChild(this.window);
	}
}

function WINDOW_CloseWindow()
{
	if(this.window.parentNode != null)
	{
		this.window.parentNode.removeChild(this.window);
	}
}

function WINDOW_SetId(Id)
{
	this.window.id=Id;
}

function WINDOW_SetSize(Width, Height)
{
	this.window.style.width = Width;
	this.window.style.height = Height;
}

function WINDOW_SetTitle(Title)
{
	this.window.getElementById("Title").innerHTML = Title;
}

function WINDOW_SetFocus()
{
	//Title
	this.window.getElementsByTagName("div")[0].className = "focus";
	//Content
	this.window.getElementsByTagName("div")[1].className = "focus";
}

function WINDOW_SetBlur()
{
	//Title
	this.window.getElementsByTagName("div")[0].className = "";
	//Content
	this.window.getElementsByTagName("div")[1].className = "";
}

function WINDOW_SetZIndex(Index)
{
	this.window.style.zIndex = Index;
}

function WINDOW_GetZIndex()
{
	return this.window.style.zIndex;
}

function WINDOW_ConcatEventButtons(ButtonsList)
{
	var i;

	for(i=0; i<ButtonsList.length; i++)
	{
		this.eventButtons.push(ButtonsList[i]);
	}
}

/***************************************************/
function WINDOW_CreateWindow(Height, Width, Div, Title)
{
	var WindowBox = document.createElement("div");
	var SubWindowBox     = document.createElement("div");

	var TitleBar= document.createElement("div");

	var TmpTitle = document.createElement("span");

	var TmpClose = document.createElement("span");

	//Array of buttons;
	var Close = new Array();

	var i;
	
	WindowBox.id ="WindowBox";
	WindowBox.style.width = Width+"px";
	if(Height != null)
	{
		WindowBox.style.height= Height+"px";
	}

	SubWindowBox.id ="SubWindowBox";
	SubWindowBox.style.width = Width +"px";
	if(Height != null)
	{
		SubWindowBox.style.height =(Height - 21)+"px";
	}

	TitleBar.id = "TitleBar";
	TitleBar.style.width = Width+"px";
	//Set title bar text unselectable
	UTILS_DisableSelection(TitleBar);

	//Drag and Drop Window
	UTILS_AddListener(TitleBar, "mousedown", function(event){ UTILS_StartDragWindow(event, WindowBox); }, false);


	TmpTitle.innerHTML = Title;
	TmpTitle.id = "Title";

	TmpClose.innerHTML = "X";
	TmpClose.id = "Close";

	Close.push(TmpClose);

	//Additional Commands when close a window
	/*
	if(CloseCommands != null)
	{
		UTILS_AddListener(TmpClose, "click", CloseCommands ,false);
	}
	*/
//	Div.style.width = Width - 10+"px";


	TitleBar.appendChild(TmpTitle);
	TitleBar.appendChild(TmpClose);

	SubWindowBox.appendChild(Div);

	WindowBox.appendChild(TitleBar);
	WindowBox.appendChild(SubWindowBox);

	return {Div:WindowBox, Close:Close}
}


/******************************************************
*******************************************************
*******************************************************
*******************************************************
*******************************************************
******************************************************/


/**
*	Create elements of game alert window and returns div
*
* @param	Text	Text to display in confirm box
* @return	Div; Array
* @see		WINDOW_Alert();
* @author Danilo Kiyoshi Simizu Yorinori
*/
function WINDOW_CreateAlert(Text)
{
	var Div;

	var TextDiv;
	var Label;

	var ButtonsDiv;
	var Ok;
	
	var Buttons = new Array();

	Div = UTILS_CreateElement("div","AlertDiv");

	TextDiv = UTILS_CreateElement("div","TextDiv");
	Label = UTILS_CreateElement("p",null,null,Text);

	ButtonsDiv = UTILS_CreateElement("div","ButtonsDiv");
	Ok = UTILS_CreateElement("input", null,"button");
	Ok.type = "button";
	Ok.value = "Ok";

	Buttons.push(Ok);

	TextDiv.appendChild(Label);

	ButtonsDiv.appendChild(Ok);

	Div.appendChild(TextDiv);
	Div.appendChild(ButtonsDiv);

	return {Div:Div, Buttons:Buttons};
}

/**
*	Create elements of confirm request window and returns div
*
* @param	Text		Text to display in confirm box
* @param 	Button1	Value and Fuction of Button1
* @param 	Button2	Value and Fuction of Button2
* @return	Div; Array
* @see		WINDOW_Confirm();
* @author Danilo Kiyoshi Simizu Yorinori
*/
function WINDOW_CreateConfirm(Text, Button1, Button2)
{
	var Div;

	var Buttons = new Array();

	var TextDiv;
	var Label;

	var ButtonsDiv;
	var Ok;
	var Cancel;

	Div = UTILS_CreateElement("div","ConfirmDiv");

	TextDiv = UTILS_CreateElement("div","TextDiv");
	Label = UTILS_CreateElement("p",null,null,Text);

	ButtonsDiv = UTILS_CreateElement("div","ButtonsDiv");
	Ok = UTILS_CreateElement("input", null,"button");
	Ok.type = "button";
	Ok.value = Button1.Name;

	if (Button1.Func)
	{
		UTILS_AddListener(Ok,"click", Button1.Func, false);
	}

	Cancel = UTILS_CreateElement("input", null,"button");
	Cancel.type = "button";
	Cancel.value = Button2.Name;

	if (Button2.Func)
	{
		UTILS_AddListener(Cancel,"click", Button2.Func, false);
	}

	Buttons.push(Ok);
	Buttons.push(Cancel);

	ButtonsDiv.appendChild(Ok);
	ButtonsDiv.appendChild(Cancel);

	TextDiv.appendChild(Label);

	Div.appendChild(TextDiv);
	Div.appendChild(ButtonsDiv);

	return {Div:Div, Buttons:Buttons};
}

/**
*	Create elements of send user image
*
* @param	Text		Text to display in confirm box
* @return	Div; Array
* @see		WINDOW_Confirm();
* @author	Rubens and Fabiano
*/
function WINDOW_CreateImageSend()
{
	var Div;

	var TextDiv, ButtonsDiv;
	var Label;
	var Ok;
	var Cancel;
	var Buttons = new Array();


	Div = UTILS_CreateElement("div","ChangeImageDiv");

	TextDiv = UTILS_CreateElement("div","TextDiv");
	Label = UTILS_CreateElement("p",null,null,UTILS_GetText("profile_change_image"));

	ButtonsDiv = UTILS_CreateElement("div","ButtonsDiv");
	Ok = UTILS_CreateElement("input", null,"button");
	Ok.type = "button";
	Ok.value = UTILS_GetText("window_ok");
	Ok.onclick = function (){ IMAGE_ImageEncode("ImageForm")};

	Cancel = UTILS_CreateElement("input", null,"button");
	Cancel.type = "button";
	Cancel.value = UTILS_GetText("window_cancel");


	Buttons.push(Ok);
	Buttons.push(Cancel);

	ButtonsDiv.appendChild(Ok);
	ButtonsDiv.appendChild(Cancel);

	TextDiv.appendChild(Label);

	Div.appendChild(TextDiv);
	Div.appendChild(IMAGE_CreateFormToEncode("ImageForm", "php/base64."+MainData.DefaultPHP));
	Div.appendChild(ButtonsDiv);
	
	return {Div:Div, Buttons:Buttons};
}
