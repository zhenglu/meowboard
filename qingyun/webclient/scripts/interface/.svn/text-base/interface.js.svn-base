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
* Control interface DOM objects
*/


/**
* Create all object needed to start interface
*/
function INTERFACE_CreateInterface()
{
	var Page, Main, Chat, Top, Left, Center;
	
	Page = UTILS_CreateElement("div", "Page");
	Main = UTILS_CreateElement("div", "Main");
	Center = UTILS_CreateElement("div", "Center");
	Top = INTERFACE_CreateTop();
	Left = INTERFACE_CreateLeft();
	Chat = INTERFACE_CreateChatList();

	Center.appendChild(INTERFACE_CreateWelcome());

	Main.appendChild(Left);
	Main.appendChild(Center);
	Page.appendChild(Top);
	Page.appendChild(Chat);
	Page.appendChild(Main);

	// Logout from jabber when close or reload page;
	document.body.setAttribute("onbeforeunload","LOGIN_LeavePage()");

	return Page;
}

/**
* Show interface in the user screen
*/
function INTERFACE_ShowInterface(Tree)
{
	document.body.appendChild(Tree);
}


/**
* Remove interface in the user screen
*/
function INTERFACE_StopInterface()
{
	var Page = document.getElementById("Page");
	var Windows = document.getElementsByTagName("div");
	var i;

	// Remove the interface div
	if (Page)
	{
		Page.parentNode.removeChild(Page);
	}

	for(i=Windows.length-1; i>=0; i--)
	{
		Windows[i].parentNode.removeChild(Windows[i]);
	}
}



function INTERFACE_CreateLoadingBox(Id, Text)
{
	var Div = UTILS_CreateElement("div",Id,"loading_box");
	var Span = UTILS_CreateElement("Span",null,null,Text);

	Div.appendChild(Span);

	return Div;
}

/**
 * @brief	
 *
 * @return	boolean
 * @author 	Danilo Yorinori
 */
function INTERFACE_DisableSelect(e)
{
	return false;
}

/**
 * @brief	
 *
 * @return	boolean
 * @author 	Danilo Yorinori
 */
function INTERFACE_ReEnableSelect()
{
	return true;
}

/**
 *	@brief Create a hint to show full name
 * 
 *	@param	Obj
 *		Obj that have name shorted
 *	@param FullName
 *		String to be displayed
 *	@return void
 *	@author	Danilo
 */
function INTERFACE_ShowFullName(Obj,FullName)
{
	var Hint, Name, ParentNode, Pos, i;

	Hint = UTILS_CreateElement("div", "FullNameDiv");

	Name = UTILS_CreateElement("p", null, null, FullName);

	Hint.appendChild(Name);
	
	// Get parent scrolling
	
	ParentNode = UTILS_GetParentDiv(Obj);

	// Get position of user list item
	Pos = UTILS_GetOffset(Obj);
	Hint.style.top = (Pos.Y+16-ParentNode.scrollTop)+"px";
	Hint.style.left = Pos.X+"px";
	Hint.style.width = FullName.length*6+'px';

	document.body.appendChild(Hint);
}

/**
 *	@brief Close a hint created by INTERFACE_ShowFullName function
 * 
 *	@return void
 *	@author	Danilo
 */
function INTERFACE_CloseFullName()
{
	var Hint = document.getElementById("FullNameDiv");
	if (Hint)
		document.body.removeChild(Hint);
}
