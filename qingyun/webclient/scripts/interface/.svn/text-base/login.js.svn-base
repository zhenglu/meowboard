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
* Shows login screen to user
*/

/**
* Global object that stores all data needed
* by interface
*/
//var MainData;

/**
* Create elements and show login screen to user
*
* @return void
* @public
*/
function INTERFACE_StartLogin(Lang)
{
	var LoginBoxDiv, LoginTextBoxDiv, LoginFormBoxDiv;
	var Title, TitleEnd, Text, Link, Text2, Text3, Banner, Version;
	var LoginLabel, PasswdLabel, InputLogin, InputPasswd, InputSubmit, CheckBox, CheckBoxLabel, ErrorLabel, SignIn;
	var LoginMessage;
	//var Lang;
	var ConfTmp;

	var ev; //Temp event

	var Table = document.createElement('table');
	var Tr = document.createElement('tr');
	var Td = document.createElement('td');
	var Br = document.createElement('br');
	
	var MainDiv;
	
	MainDiv = document.getElementById("MainDiv");
	
	if(MainDiv != null)
	{
		MainDiv.parentNode.removeChild(MainDiv);
	}
	else
	{
		MainDiv = UTILS_CreateElement('div','MainDiv');
	}

	//Internet Explorer Table
	var TBody = document.createElement('tbody');
/*
	// What language show?
	// Find lang in cookie
	Lang = UTILS_ReadCookie("lang");
	// if language is not found in cookie
	if (Lang == "")
	{
		// Get from browser language
		//Lang = UTILS_GetLanguage();
		
		// QuickFix!
		// Get default lang from configuration file
		ConfTmp = UTILS_OpenXMLFile("scripts/data/conf.xml");
		Lang = UTILS_GetTag(ConfTmp, "default-lang");
	}

	// Read xml config files and starting data structure
	MainData = new DATA("scripts/data/conf.xml", "scripts/lang/"+Lang+".xml");
	MainData.Lang = Lang;
*/
	// Creating elements and setting properties
	LoginBoxDiv = UTILS_CreateElement("div", "LoginDiv");
	LoginTextBoxDiv = UTILS_CreateElement("div", "TextDiv");
	LoginFormBoxDiv = UTILS_CreateElement("div", "FormDiv");
	Title = UTILS_CreateElement("h3", null, null, UTILS_GetText("login_title"));
	TitleEnd = UTILS_CreateElement("h3", null, null, UTILS_GetText("login_footer"));
	Text = UTILS_CreateElement("p", null, null, UTILS_GetText("login_text1"));
	Text2 = UTILS_CreateElement("p", null, null, UTILS_GetText("login_text2"));
	Text3 = UTILS_CreateElement("p", null, null, UTILS_GetText("login_text3"));
//	Link = UTILS_CreateElement("a", null, "link", "http://localhost/elgg/");
//	Link.href = "http://localhost/elgg/";

	Version = UTILS_CreateElement("p", "version", null, MainData.Version);
	
	LoginLabel = UTILS_CreateElement("span", null, "Label", UTILS_GetText("login_user")+":");
	PasswdLabel = UTILS_CreateElement("span", null, "Label", UTILS_GetText("login_passwd")+":");
	LoginMessage = UTILS_CreateElement("span","LoginMessage");
	LoginMessage.style.display = "none";
	InputLogin = UTILS_CreateElement("input", "login");
	InputPasswd = UTILS_CreateElement("input", "password");
	InputSubmit = UTILS_CreateElement("input", "login_button", "entrar");
	CheckBox = UTILS_CreateElement("input", "checkbox", "checkbox");
	InputLogin.type = "text";
	InputPasswd.type = "password";
	InputLogin.value = UTILS_ReadCookie("Username");
	InputPasswd.value = UTILS_ReadCookie("Passwd");
	InputSubmit.type = "submit";
	InputSubmit.value = "";
	CheckBox.type = "checkbox";
	CheckBoxLabel = UTILS_CreateElement("label", null, null, UTILS_GetText("login_remember_pass"));
	ErrorLabel = UTILS_CreateElement("span", "ErrorLabel", "error_label");
	SignIn = UTILS_CreateElement("a", null, null, UTILS_GetText("login_signin"));
	Banner = UTILS_CreateElement("img", "BannerLogin");
	Banner.src = "images/login/banner_login.gif";

	if (UTILS_ReadCookie("RememberPass") == "true")
		CheckBox.checked = true;

	UTILS_AddListener(InputSubmit, "click", function() { LOGIN_Login(InputLogin.value,InputPasswd.value,CheckBox.checked); } , false);

	UTILS_AddListener(InputLogin, "keypress", function(event) { ev = UTILS_ReturnEvent(event); if (ev.keyCode == 13) LOGIN_Login(InputLogin.value,InputPasswd.value,CheckBox.checked); }, false);

	UTILS_AddListener(InputPasswd, "keypress", function(event) { ev = UTILS_ReturnEvent(event); if (ev.keyCode == 13) LOGIN_Login(InputLogin.value,InputPasswd.value,CheckBox.checked); }, false);

	UTILS_AddListener(SignIn, "click", function() { window.location.href="../elgg"})
	
	// Creating tree
	LoginTextBoxDiv.appendChild(Title);
	LoginTextBoxDiv.appendChild(Text);
//	Text2.appendChild(Link);
	LoginTextBoxDiv.appendChild(Text2);
	LoginTextBoxDiv.appendChild(Text3);
	LoginTextBoxDiv.appendChild(TitleEnd);

	// Creating tree
	Td.appendChild(LoginLabel);
	Tr.appendChild(Td);
	Td = document.createElement('td');
	Td.appendChild(InputLogin);
	Tr.appendChild(Td);
	TBody.appendChild(Tr);

	Tr = document.createElement('tr');
	Td = document.createElement('td');
	Td.appendChild(PasswdLabel);
	Tr.appendChild(Td);
	Td = document.createElement('td');
	Td.appendChild(InputPasswd);
	Td.appendChild(Br);
	Td.appendChild(CheckBox);
	Td.appendChild(CheckBoxLabel);
	Tr.appendChild(Td);
	TBody.appendChild(Tr);

	Table.appendChild(TBody);

	Br = document.createElement('br');
	LoginFormBoxDiv.appendChild(Table);
	LoginFormBoxDiv.appendChild(InputSubmit);
	LoginFormBoxDiv.appendChild(Br);
	LoginFormBoxDiv.appendChild(LoginMessage);
	LoginFormBoxDiv.appendChild(ErrorLabel);
	LoginFormBoxDiv.appendChild(SignIn);

	LoginBoxDiv.appendChild(LoginTextBoxDiv);
	LoginBoxDiv.appendChild(LoginFormBoxDiv);

	MainDiv.appendChild(INTERFACE_CreateLanguage());
	MainDiv.appendChild(LoginBoxDiv);
	MainDiv.appendChild(Banner);

	document.body.appendChild(MainDiv);

	// Show webclient version
	document.body.appendChild(Version);

	// Setting document title
	document.title = UTILS_GetText("general_title");

	// Focus to login input
	InputLogin.focus()

	// Block context menu
	document.oncontextmenu = function() { return false; };

	// Set vertical align middle to MainDiv when resize window
	document.body.setAttribute("onresize", "INTERFACE_LoginVerticalAlignMiddle()");
	// Align main div in the center
	INTERFACE_LoginVerticalAlignMiddle();
}

/**
* Remove login screen
*
* @return void
* @public
*/
function INTERFACE_EndLogin()
{
	var Div = document.getElementById("LoginDiv");
	var Banner = document.getElementById("BannerLogin");
	var Lang = document.getElementById("LangDiv");
	var Version = document.getElementById("version");

	// If login div is on screen
	if (Div)
	{
		Div.parentNode.removeChild(Div);
	}

	if (Banner)
	{
		Banner.parentNode.removeChild(Banner);
	}

	if (Lang)
	{
		Lang.parentNode.removeChild(Lang);
	}

	if (Version)
	{
		Version.parentNode.removeChild(Version);
	}
}

/**
* Remove login screen
*
* @return Div elements with tags
* @public
*/
function INTERFACE_CreateLanguage()
{
	var DivLang = UTILS_CreateElement("div","LangDiv");
	var Ul = UTILS_CreateElement("ul","LangUl");
	var i;

	var Languages = UTILS_OpenXMLFile("scripts/data/lang.xml");
	var Langs = Languages.getElementsByTagName("lang");

	for(i=0; i<Langs.length; i++)
	{
		Ul.appendChild(INTERFACE_CreateLangItem(UTILS_GetNodeText(Langs[i]), Langs[i].getAttribute("name")));
	}

	DivLang.appendChild(Ul);

	return DivLang;
}

/**
* Create language links
*
* @param Lang is language (i.e.: en_US, pt_BR, zh_CN,...)
* @return List item
* @private
*/
function INTERFACE_CreateLangItem(Lang, Name)
{
	var Li = UTILS_CreateElement("li");
	//var Item = UTILS_CreateElement("img");
	var Item = UTILS_CreateElement("span");

	//Item.src = "images/lang/"+Lang+".png";
	Item.innerHTML = UTILS_Capitalize(Name);
	Li.appendChild(Item);
	
	Li.onclick = function(){
		START_ChangeLanguage(Lang);
	}

	return Li;
}


/**
 * @brief       Align main div in middle
 * @author      Rubens
 */

function INTERFACE_LoginVerticalAlignMiddle()
{
        var MainDiv = document.getElementById("MainDiv");

        var WindowHeight = window.innerHeight;

	MainDiv.style.position = "relative";
        if(WindowHeight > 600)
        {
                MainDiv.style.top = ((WindowHeight / 2) - 300) + "px";
        }
}

/**
 *
 *
 */
function INTERFACE_LoginDisableInput()
{
	var UsernameBox = document.getElementById("login");
	var PassWBox = document.getElementById("password");
	var CheckBox = document.getElementById("checkbox");
	var OkButton = document.getElementById("login_button");

	UsernameBox.disabled = true;
	PassWBox.disabled = true;
	CheckBox.disabled = true;
	OkButton.disabled = true;
}

function INTERFACE_LoginEnableInput()
{
	var UsernameBox = document.getElementById("login");
	var PassWBox = document.getElementById("password");
	var CheckBox = document.getElementById("checkbox");
	var OkButton = document.getElementById("login_button");

	UsernameBox.disabled = false;
	PassWBox.disabled = false;
	CheckBox.disabled = false;
	OkButton.disabled = false;
}

function INTERFACE_ClearError()
{	
	var ErrorLabel = document.getElementById("ErrorLabel");
	ErrorLabel.innerHTML = "";
}

function INTERFACE_ShowLoginMessage(Msg)
{
	var LoginMsg = document.getElementById("LoginMessage");
	if(Msg != null)
	{
		LoginMsg.innerHTML = Msg;
	}
	else
	{
		LoginMsg.innerHTML = "";
	}
	LoginMsg.style.display = "block";
}

function INTERFACE_HideLoginMessage()
{
	var LoginMsg = document.getElementById("LoginMessage");
	LoginMsg.style.display = "none";
}

function INTERFACE_ShowErrorMessage(Msg)
{
	var ErrorMsg = document.getElementById("ErrorLabel");

	if(ErrorMsg == null)
	{
		return false;
	}

	if(Msg != null)
	{
		ErrorMsg.innerHTML = Msg;
	}
	else
	{
		ErrorMsg.innerHTML = "";
	}

	ErrorMsg.style.display = "block";

	return true;
}

function INTERFACE_HideErrorMessage()
{
	var ErrorMsg = document.getElementById("ErrorLabel");

	ErrorMsg.style.display = "none";
}
