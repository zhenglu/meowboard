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
* @file help.js
* @brief Create window content to show help content and auxiliar functions
*
*/

/**
 * @brief Create temporary help window content
 *
 * @return	void
 * @author 	Danilo Yorinori
 * @see	Texts (lang), WINDOW_Help();
 */
function INTERFACE_HelpWindow()
{
	var Div;

	var HowDiv;
	var HelpList, HowList, PlayList;
	var L1,L2,L3;
	var C1,C2,C3;

	var Buttons = new Array();

	var ButtonDiv;
	var CloseButton;

	var Anchor;

	// Main Div
	Div = UTILS_CreateElement("div","HelpDiv");

	// Help Content
	HowDiv = UTILS_CreateElement("div","HowDiv");

	// Help Lists
	HelpList = UTILS_CreateElement("ul", "HelpList");
	HowList = UTILS_CreateElement("ul", "HowList");
	PlayList = UTILS_CreateElement("ul", "PlayList");

	// 1 - How Item
	L1 = UTILS_CreateElement("li","How.1T","selected",UTILS_GetText("help_t_1"));
	INTERFACE_HelpDisplay(L1, "How.1");
	C1 = UTILS_CreateElement("li","How.1C","show_help");

		// 1.1 - Play
		L2 = UTILS_CreateElement("li","How.1.1T","unselected",UTILS_GetText("help_t_1_1"));
		INTERFACE_HelpDisplay(L2, "How.1.1");
		C2 = UTILS_CreateElement("li","How.1.1C","hide_help");

			// 1.1.1 - Challenge unique user
			L3 = UTILS_CreateElement("li","How.1.1.1T","unselected",UTILS_GetText("help_t_1_1_1"));
			INTERFACE_HelpDisplay(L3, "How.1.1.1");
			C3 = UTILS_CreateElement("li","How.1.1.1C","hide_help");
			C3.appendChild(INTERFACE_CreateHelpContent(UTILS_GetText("help_c_1_1_1")));
			// Appending elements of section 1.1.1
			PlayList.appendChild(L3);
			PlayList.appendChild(C3);

			// 1.1.2 - Accept match offer
			L3 = UTILS_CreateElement("li","How.1.1.2T","unselected",UTILS_GetText("help_t_1_1_2"));
			INTERFACE_HelpDisplay(L3, "How.1.1.2");
			C3 = UTILS_CreateElement("li","How.1.1.2C","hide_help");
			C3.appendChild(INTERFACE_CreateHelpContent(UTILS_GetText("help_c_1_1_2")));
			// Appending elements of section 1.1.2
			PlayList.appendChild(L3);
			PlayList.appendChild(C3);

			// 1.1.3 - Announce a game
			L3 = UTILS_CreateElement("li","How.1.1.3T","unselected",UTILS_GetText("help_t_1_1_3"));
			INTERFACE_HelpDisplay(L3, "How.1.1.3");
			C3 = UTILS_CreateElement("li","How.1.1.3C","hide_help");
			C3.appendChild(INTERFACE_CreateHelpContent(UTILS_GetText("help_c_1_1_3")));
			// Appeding elements of section 1.1.3
			PlayList.appendChild(L3);
			PlayList.appendChild(C3);

		// Appending play content list 
		C2.appendChild(PlayList);

		// Appeding elements of section 1.1
		HowList.appendChild(L2);
		HowList.appendChild(C2);

		// 1.2 - Search user
		L2 = UTILS_CreateElement("li","How.1.2T","unselected",UTILS_GetText("help_t_1_2"));
		INTERFACE_HelpDisplay(L2, "How.1.2");
		C2 = UTILS_CreateElement("li","How.1.2C","hide_help");
		C2.appendChild(INTERFACE_CreateHelpContent(UTILS_GetText("help_c_1_2")));
		// Appeding elements of section 1.2
		HowList.appendChild(L2);
		HowList.appendChild(C2);

		// 1.3 - Chat with user
		L2 = UTILS_CreateElement("li","How.1.3T","unselected",UTILS_GetText("help_t_1_3"));
		INTERFACE_HelpDisplay(L2, "How.1.3");
		C2 = UTILS_CreateElement("li","How.1.3C","hide_help");
		C2.appendChild(INTERFACE_CreateHelpContent(UTILS_GetText("help_c_1_3")));
		// Appeding elements of section 1.3
		HowList.appendChild(L2);
		HowList.appendChild(C2);

		// 1.4 - Chat in room 
		L2 = UTILS_CreateElement("li","How.1.4T","unselected",UTILS_GetText("help_t_1_4"));
		INTERFACE_HelpDisplay(L2, "How.1.4");
		C2 = UTILS_CreateElement("li","How.1.4C","hide_help");
		C2.appendChild(INTERFACE_CreateHelpContent(UTILS_GetText("help_c_1_4")));
		// Appeding elements of section 1.4
		HowList.appendChild(L2);
		HowList.appendChild(C2);

		// 1.5 - Show or hide room's user list
		L2 = UTILS_CreateElement("li","How.1.5T","unselected",UTILS_GetText("help_t_1_5"));
		INTERFACE_HelpDisplay(L2, "How.1.5");
		C2 = UTILS_CreateElement("li","How.1.5C","hide_help");
		C2.appendChild(INTERFACE_CreateHelpContent(UTILS_GetText("help_c_1_5")));
		// Appeding elements of section 1.5
		HowList.appendChild(L2);
		HowList.appendChild(C2);

		// 1.6 - See another user's profile
		L2 = UTILS_CreateElement("li","How.1.6T","unselected",UTILS_GetText("help_t_1_6"));
		INTERFACE_HelpDisplay(L2, "How.1.6");
		C2 = UTILS_CreateElement("li","How.1.6C","hide_help");
		C2.appendChild(INTERFACE_CreateHelpContent(UTILS_GetText("help_c_1_6")));
		// Appeding elements of section 1.6
		HowList.appendChild(L2);
		HowList.appendChild(C2);

		// 1.7 - Add user in contact list
		L2 = UTILS_CreateElement("li","How.1.7T","unselected",UTILS_GetText("help_t_1_7"));
		INTERFACE_HelpDisplay(L2, "How.1.7");
		C2 = UTILS_CreateElement("li","How.1.7C","hide_help");
		C2.appendChild(INTERFACE_CreateHelpContent(UTILS_GetText("help_c_1_7")));
		// Appeding elements of section 1.7
		HowList.appendChild(L2);
		HowList.appendChild(C2);

		// 1.8 - Remove user from contact list
		L2 = UTILS_CreateElement("li","How.1.8T","unselected",UTILS_GetText("help_t_1_8"));
		INTERFACE_HelpDisplay(L2, "How.1.8");
		C2 = UTILS_CreateElement("li","How.1.8C","hide_help");
		C2.appendChild(INTERFACE_CreateHelpContent(UTILS_GetText("help_c_1_8")));
		// Appeding elements of section 1.8
		HowList.appendChild(L2);
		HowList.appendChild(C2);

		// 1.9 - Create status
		L2 = UTILS_CreateElement("li","How.1.9T","unselected",UTILS_GetText("help_t_1_9"));
		INTERFACE_HelpDisplay(L2, "How.1.9");
		C2 = UTILS_CreateElement("li","How.1.9C","hide_help");
		C2.appendChild(INTERFACE_CreateHelpContent(UTILS_GetText("help_c_1_9")));
		// Appeding elements of section 1.9
		HowList.appendChild(L2);
		HowList.appendChild(C2);

		// 1.10 - Change profile
		L2 = UTILS_CreateElement("li","How.1.10T","unselected",UTILS_GetText("help_t_1_10"));
		INTERFACE_HelpDisplay(L2, "How.1.10");
		C2 = UTILS_CreateElement("li","How.1.10C","hide_help");
		C2.appendChild(INTERFACE_CreateHelpContent(UTILS_GetText("help_c_1_10")));
		// Appeding elements of section 1.10
		HowList.appendChild(L2);
		HowList.appendChild(C2);

		// 1.11 - Enter chat room
		L2 = UTILS_CreateElement("li","How.1.11T","unselected",UTILS_GetText("help_t_1_11"));
		INTERFACE_HelpDisplay(L2, "How.1.11");
		C2 = UTILS_CreateElement("li","How.1.11C","hide_help");
		C2.appendChild(INTERFACE_CreateHelpContent(UTILS_GetText("help_c_1_11")));
		// Appeding elements of section 1.10
		HowList.appendChild(L2);
		HowList.appendChild(C2);

		// 1.12 - Examine game
		L2 = UTILS_CreateElement("li","How.1.12T","unselected",UTILS_GetText("help_t_1_12"));
		INTERFACE_HelpDisplay(L2, "How.1.12");
		C2 = UTILS_CreateElement("li","How.1.12C","hide_help");
		C2.appendChild(INTERFACE_CreateHelpContent(UTILS_GetText("help_c_1_12")));
		// Appeding elements of section 1.12
		HowList.appendChild(L2);
		HowList.appendChild(C2);
	
		// 1.13 - Watch game
		L2 = UTILS_CreateElement("li","How.1.13T","unselected",UTILS_GetText("help_t_1_13"));
		INTERFACE_HelpDisplay(L2, "How.1.13");
		C2 = UTILS_CreateElement("li","How.1.13C","hide_help");
		C2.appendChild(INTERFACE_CreateHelpContent(UTILS_GetText("help_c_1_13")));
		// Appeding elements of section 1.13
		HowList.appendChild(L2);
		HowList.appendChild(C2);
	
		// 1.14 - Search old games
		L2 = UTILS_CreateElement("li","How.1.14T","unselected",UTILS_GetText("help_t_1_14"));
		INTERFACE_HelpDisplay(L2, "How.1.14");
		C2 = UTILS_CreateElement("li","How.1.14C","hide_help");
		C2.appendChild(INTERFACE_CreateHelpContent(UTILS_GetText("help_c_1_14")));
		// Appeding elements of section 1.14
		HowList.appendChild(L2);
		HowList.appendChild(C2);
	
		// 1.15 - Examine game
		L2 = UTILS_CreateElement("li","How.1.15T","unselected",UTILS_GetText("help_t_1_15"));
		INTERFACE_HelpDisplay(L2, "How.1.15");
		C2 = UTILS_CreateElement("li","How.1.15C","hide_help");
		C2.appendChild(INTERFACE_CreateHelpContent(UTILS_GetText("help_c_1_15")));
		// Appeding elements of section 1.15
		HowList.appendChild(L2);
		HowList.appendChild(C2);
	
		// 1.16 - Help
		L2 = UTILS_CreateElement("li","How.1.16T","unselected",UTILS_GetText("help_t_1_16"));
		INTERFACE_HelpDisplay(L2, "How.1.16");
		C2 = UTILS_CreateElement("li","How.1.16C","hide_help");
		C2.appendChild(INTERFACE_CreateHelpContent(UTILS_GetText("help_c_1_16")));
		// Appeding elements of section 1.16
		HowList.appendChild(L2);
		HowList.appendChild(C2);
	
	// Appending how list 
	C1.appendChild(HowList);

	// Appending elements of section 1
	HelpList.appendChild(L1);
	HelpList.appendChild(C1);

	// Appeding help list
	HowDiv.appendChild(HelpList);

	// Button Div
	ButtonDiv = UTILS_CreateElement("div","ButtonDiv");

	// Create close button
	CloseButton = UTILS_CreateElement("input",null,"button");
	CloseButton.type = "button";
	CloseButton.value = UTILS_GetText("window_close");

	Buttons.push(CloseButton);

	// Appending close button
	ButtonDiv.appendChild(CloseButton);

	// Appeding elements in Main div
	Div.appendChild(HowDiv);
	Div.appendChild(ButtonDiv);

	return{Div:Div, Buttons:Buttons}
}

/**
 * @brief Create help content div and set text passed as parameter
 *
 * @param	Text	Text to be insert in div content
 * @return	HTML Div Element
 * @see INTERFACE_HelpWindow
 * @author 	Danilo Yorinori
 */
function INTERFACE_CreateHelpContent(Text)
{
	var TextContent;

	TextContent = UTILS_CreateElement("div","Explanation");

	TextContent.innerHTML = Text;

	return TextContent;
}

/**
 * @brief Show or hide help content of section with Id passed as parameter
 *
 * @param	ObjId	Section's Id
 * @return	boolean
 * @see INTERFACE_HelpDisplay
 * @author 	Danilo Yorinori
 */
function INTERFACE_ShowHideHelpContent(ObjId)
{
	var Node = document.getElementById(ObjId+"T");
	var NodeText;

	if (!Node)
	{
		return false;
	}

	if (Node.className == "unselected")
	{
		Node.className = "selected";

		NodeText = document.getElementById(ObjId+"C");
		if (NodeText)
		{
			NodeText.className = "show_help";
		}
		else
		{
			return false;
		}
	}
	else if (Node.className == "selected")
	{
		Node.className = "unselected";

		NodeText = document.getElementById(ObjId+"C");
		if (NodeText)
		{
			NodeText.className = "hide_help";
		}
		else
		{
			return false;
		}
	}

	return true;
}

/**
 * @brief Show help content of linked section with Id passed as parameter
 *
 * @param	ObjId	Section's Id
 * @return	boolean
 * @see INTERFACE_HelpWindow
 * @author 	Danilo Yorinori
 */
function INTERFACE_ShowLinkContent(ObjId)
{
	var Node = document.getElementById(ObjId+"T");
	var NodeText;

	if (!Node)
	{
		return false;
	}

	if (Node.className == "unselected")
	{
		Node.className = "selected";

		NodeText = document.getElementById(ObjId+"C");
		if (NodeText)
		{
			NodeText.className = "show_help";
		}
		else
		{
			return false;
		}
	}

	return true;
}

/**
 * @brief	Set help section title element action to open content and not be selected when clicked 
 *
 * @param	Obj	Help section title element
 * @param Id	Section's Id
 * @return	void
 * @see INTERFACE_HelpWindow
 * @author 	Danilo Yorinori
 */
function INTERFACE_HelpDisplay(Obj,Id)
{
	Obj.onselectstart = function() { return false; }
	if (window.sidebar) 
	{
		Obj.onmousedown = INTERFACE_DisableSelect;
		Obj.onclick = function() { INTERFACE_ShowHideHelpContent(Id); INTERFACE_ReEnableSelect; }
	}
	else
	{
		Obj.onclick = function() { INTERFACE_ShowHideHelpContent(Id); INTERFACE_ReEnableSelect; }
	}
}
