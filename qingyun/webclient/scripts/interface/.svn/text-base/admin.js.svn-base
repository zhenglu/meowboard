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
* @file admin.js
* @brief Create window content to show admin options
*
* Create window contents to admins for ban, kick, unban, etc.
*/

/**
 * @brief Create temporary unban window content
 *
 * Create temporary unban window content with a input user name who will be unbaned.
 *
 * @author 	Rubens Suguimoto
 * @see 	WINDOW_UnbanUser
 */
function INTERFACE_UnbanUserWindow()
{
	var Div;
	var Buttons = new Array();
	var Elements = new Object();

	var Input;

	var ButtonsDiv;
	var OkButton, CancelButton;


	Div = UTILS_CreateElement("div","UnbanDiv");

	ButtonsDiv = UTILS_CreateElement("div","ButtonsDiv");

	Input = UTILS_CreateElement("input","UnbanInput");

	OkButton = UTILS_CreateElement("input",null,"button");
	OkButton.type = "button";
	OkButton.value= "Unban";
	UTILS_AddListener(OkButton,"click", function(){
		ADMIN_UnbanUser(Input.value);
	}, false);

	CancelButton = UTILS_CreateElement("input",null,"button");
	CancelButton.type = "button";
	CancelButton.value = "Cancel";

	Buttons.push(OkButton);
	Buttons.push(CancelButton);

	ButtonsDiv.appendChild(OkButton);
	ButtonsDiv.appendChild(CancelButton);

	Div.appendChild(Input);
	Div.appendChild(ButtonsDiv);

	Elements.Input = Input;
	Elements.OkButton = OkButton;
	Elements.CancelButton = CancelButton;

	return{Div:Div, Buttons:Buttons, Elements:Elements}
}

/**
 * @brief Create temporary ban window content
 *
 * Create temporary ban user window content with a input reason.
 *
 * @author 	Rubens Suguimoto
 * @see 	WINDOW_BanUser
 */
function INTERFACE_BanUserWindow(Username)
{
	var Div;
	var Buttons = new Array();
	var Elements = new Object();

	var Input;

	var ButtonsDiv;
	var OkButton, CancelButton;


	Div = UTILS_CreateElement("div","BanDiv");

	ButtonsDiv = UTILS_CreateElement("div","ButtonsDiv");

	Input = UTILS_CreateElement("input","BanInput");

	OkButton = UTILS_CreateElement("input",null,"button");
	OkButton.type = "button";
	OkButton.value= "ban";
	UTILS_AddListener(OkButton,"click", function(){
		ADMIN_BanUser(Username, Input.value);
	}, false);

	CancelButton = UTILS_CreateElement("input",null,"button");
	CancelButton.type = "button";
	CancelButton.value = "Cancel";

	Buttons.push(OkButton);
	Buttons.push(CancelButton);

	ButtonsDiv.appendChild(OkButton);
	ButtonsDiv.appendChild(CancelButton);

	Div.appendChild(Input);
	Div.appendChild(ButtonsDiv);

	Elements.Input = Input;
	Elements.OkButton = OkButton;
	Elements.CancelButton = CancelButton;

	return{Div:Div, Buttons:Buttons, Elements:Elements}
}

/**
 * @brief Create temporary kick window content
 *
 * Create temporary kick user window content with a input reason.
 *
 * @author 	Rubens Suguimoto
 * @see 	WINDOW_BanUser
 */
function INTERFACE_KickUserWindow(Username)
{
	var Div;
	var Buttons = new Array();
	var Elements = new Object();

	var Input;

	var ButtonsDiv;
	var OkButton, CancelButton;


	Div = UTILS_CreateElement("div","KickDiv");

	ButtonsDiv = UTILS_CreateElement("div","ButtonsDiv");

	Input = UTILS_CreateElement("input","KickInput");

	OkButton = UTILS_CreateElement("input",null,"button");
	OkButton.type = "button";
	OkButton.value= "kick";
	UTILS_AddListener(OkButton,"click", function(){
		ADMIN_KickUser(Username, Input.value);
	}, false);

	CancelButton = UTILS_CreateElement("input",null,"button");
	CancelButton.type = "button";
	CancelButton.value = "Cancel";

	Buttons.push(OkButton);
	Buttons.push(CancelButton);

	ButtonsDiv.appendChild(OkButton);
	ButtonsDiv.appendChild(CancelButton);

	Div.appendChild(Input);
	Div.appendChild(ButtonsDiv);

	Elements.Input = Input;
	Elements.OkButton = OkButton;
	Elements.CancelButton = CancelButton;

	return{Div:Div, Buttons:Buttons, Elements:Elements}
}


/**
 * @brief Create admin window with administrative tools
 *
 * Create window content with administrative tools.
 *
 * @author 	Rubens Suguimoto
 * @see 	WINDOW_AdminWindow
 */
function INTERFACE_CreateAdminWindow()
{
	var Div;
	var UnbanList;
	var ButtonsDiv, Close;
	var Buttons = new Array();

	Div = UTILS_CreateElement("div","AdminDiv");
	UnbanList = UTILS_CreateElement("ul","AdminUnbanList",null,"Unban List");
	ButtonsDiv = UTILS_CreateElement("div","ButtonsDiv");

	Close = UTILS_CreateElement("input", null,"button");
	Close.type = "button";
	Close.value = UTILS_GetText("window_cancel");

	Buttons.push(Close);

	ButtonsDiv.appendChild(Close);

	Div.appendChild(UnbanList);
	//Div.appendChild(ButtonsDiv);

	return {Div:Div, Buttons:Buttons};
}


function INTERFACE_AddBannedUser(Username)
{
	var List = document.getElementById("AdminUnbanList");
	var Item;
	
	if(List == null)
	{	
		return false;
	}

	Item = UTILS_CreateElement("li",null,null,Username);
	Item.onclick = function (){
		ADMIN_UnbanUser(Username);
		List.removeChild(this);
	}

	List.appendChild(Item);

	return true;
}
