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
* Create element of top of the screen
*/

function INTERFACE_CreateTop()
{
	var MainDiv, Logo, MenuDiv, IconsList, MenuList, Item, ItemTitle;
	var Pos;
	var ExitText;

	MainDiv = UTILS_CreateElement("div", "Top");
	Logo = UTILS_CreateElement("h1", null, null, UTILS_GetText("general_name"));
	MenuDiv = UTILS_CreateElement("div", "TopMenu");
	IconsList = UTILS_CreateElement("ul", null, "icons");
	MenuList = UTILS_CreateElement("ul", null, "menu");

	// Append icons to list
	// Adjourn game
	if(MainData.Type == "admin")
	{
		ItemTitle = UTILS_GetText("menu_adjourn")
		Item = UTILS_CreateElement("li", null, "adjourn_game");
		Item.title = ItemTitle;
		UTILS_AddListener(Item,"click",function() { WINDOW_AdminWindow(); ADMIN_GetBanList(); }, "false");
		IconsList.appendChild(Item);
	}
	else
	{ // None
		Item = UTILS_CreateElement("li", "admin_icon", "null", null);
		IconsList.appendChild(Item);
	}
	// Search game
	ItemTitle = UTILS_GetText("menu_search_game")
	Item = UTILS_CreateElement("li", null, "search_game");
	Item.title = ItemTitle;
	UTILS_AddListener(Item,"click",function() { OLDGAME_OpenOldGameWindow(); }, "false");
	IconsList.appendChild(Item);
	
	// Search user
	ItemTitle = UTILS_GetText("menu_search_user")
	Item = UTILS_CreateElement("li", null, "search_user");
	Item.title = ItemTitle;
	UTILS_AddListener(Item,"click",function() { WINDOW_SearchUser(); }, "false");
	IconsList.appendChild(Item);

	// Preferences
	ItemTitle = UTILS_GetText("menu_preferences")
	Item = UTILS_CreateElement("li", null, "preferences");
	Item.title = ItemTitle;

	Item.onclick = function () {
		WINDOW_Alert(UTILS_GetText("not_implemented_title"),UTILS_GetText("not_implemented"));
	}

	IconsList.appendChild(Item);

	// Help
	ItemTitle = UTILS_GetText("menu_help")
	Item = UTILS_CreateElement("li", null, "help");
	Item.title = ItemTitle;

	UTILS_AddListener(Item,"click",function() { WINDOW_Help(); }, "false");
/*	Item.onclick = function () {
		WINDOW_Alert(UTILS_GetText("not_implemented_title"),UTILS_GetText("not_implemented"));
	} 
*/
	IconsList.appendChild(Item);

	// Exit
	ItemTitle = UTILS_GetText("menu_exit");
	Item = UTILS_CreateElement("li", "ExitButton", "exit");
	ExitText = UTILS_CreateElement("span","ExitText", null, ItemTitle);
	Item.onclick = function () { 
		LOGIN_Logout();
	}
	Item.title = ItemTitle;
	Item.appendChild(ExitText);
	IconsList.appendChild(Item);

	// Appending itens to menu
	// Current games
	Item = UTILS_CreateElement("li", null, "currentGames", UTILS_GetText("menu_current_games"));
	Item.onclick = function () {
		Pos = UTILS_GetOffset(this);
		ROOM_ShowGameRoomList(Pos.X);
	}
	MenuList.appendChild(Item);
	
	// Challenges
	Item = UTILS_CreateElement("li", null, null, UTILS_GetText("menu_challenges"));
	MenuList.appendChild(Item);
		Item.onclick = function () {
		Pos = UTILS_GetOffset(this);
		CHALLENGE_ShowChallengeMenu(Pos.X, Pos.Y);
	}

	MenuList.appendChild(Item);
	// Tourneys
	Item = UTILS_CreateElement("li", null, null, UTILS_GetText("menu_tourneys"));
	Item.onclick = function () {
		WINDOW_Alert(UTILS_GetText("not_implemented_title"),UTILS_GetText("not_implemented"));
	}

	MenuList.appendChild(Item);
	
	// Rooms
	Item = UTILS_CreateElement("li", null, "rooms", UTILS_GetText("menu_rooms"));
	Item.onclick = function () {
		Pos = UTILS_GetOffset(this);
		ROOM_ShowRoomList(Pos.X);
	}
	MenuList.appendChild(Item);
	
	MenuDiv.appendChild(IconsList);
	MenuDiv.appendChild(MenuList);
	MainDiv.appendChild(Logo);
	MainDiv.appendChild(MenuDiv);

	return MainDiv;
}

/**
* Show rooms menu
*/
function INTERFACE_ShowRoomMenu(OffsetLeft)
{
	var MenuDiv, RoomList, RoomItem, CreateP, Create;
	var Node, Menu, Func, i, Hide = 0;

	Node = document.getElementById("Page");
	Menu = document.getElementById("RoomMenuDiv");



	if (!Node || Menu)
	{
		return null;
	}

	Func = function () {
		Hide += 1;
		
		if (Hide == 2)
		{
			UTILS_RemoveListener(document, "click", Func, false);

			// Remove menu from screen
			INTERFACE_HideRoomList();
		}
	};

	// Creating elements
	MenuDiv = UTILS_CreateElement("div", "RoomMenuDiv");
	RoomList = UTILS_CreateElement("ul", "RoomMenuList");


	// Show the create room window
	CreateP = UTILS_CreateElement("p");
	Create = UTILS_CreateElement("span", "createRoom", null, UTILS_GetText("room_create_rooms"));
	Create.onclick = function () {
		WINDOW_CreateRoom();
	}
	
	// Show loading message
	MenuDiv.appendChild(INTERFACE_ShowLoadBox());

	MenuDiv.appendChild(RoomList);
	CreateP.appendChild(Create);
	MenuDiv.appendChild(CreateP);
	Node.appendChild(MenuDiv);

	MenuDiv.style.left = (OffsetLeft-1)+"px";

	UTILS_AddListener(document, "click", Func, false);

	return true;
}


/**
* Show rooms menu
* @see		room/room.js: ROOM_HandleGameRoomInfoList to load list itens
* @return 	bool
* @author 	Ulysses
*/
function INTERFACE_ShowGameRoomMenu(OffsetLeft)
{
	var MenuDiv, RoomItem;
	var Node, Menu, Func, i, Hide = 0;

	Node = document.getElementById("Page");
	Menu = document.getElementById("GameRoomMenuDiv");

	if (!Node || Menu)
	{
		return null;
	}

	Func = function () {
		Hide += 1;
		
		if (Hide == 2)
		{
			UTILS_RemoveListener(document, "click", Func, false);

			// Remove menu from screen
			INTERFACE_HideGameRoomList();
		}
	};

	// Creating elements
	MenuDiv = UTILS_CreateElement("div", "GameRoomMenuDiv");

	// Show loading message
	MenuDiv.appendChild(INTERFACE_ShowLoadBox());

	MenuDiv.style.left = OffsetLeft+"px";

	Node.appendChild(MenuDiv);

	UTILS_AddListener(document, "click", Func, false);

	return true;
}

function INTERFACE_ShowLoadBox()
{
	var Div = UTILS_CreateElement("div", "DivLoadBox");

	var Span = UTILS_CreateElement("span",null,null,UTILS_GetText("menu_loading"));

	Div.appendChild(Span);

	return Div;
}

function INTERFACE_RemoveLoadBox()
{
	var Div = document.getElementById("DivLoadBox");

	if(Div != null)
	{
		Div.parentNode.removeChild(Div);
	}
}

function INTERFACE_NoGamesInGameList()
{
	var GameList = document.getElementById("GameRoomMenuDiv");
	var Div = UTILS_CreateElement("div", "DivNoGames");

	var Span = UTILS_CreateElement("span",null,null,UTILS_GetText("menu_no_games"));

	if(GameList != null)
	{
		Div.appendChild(Span);
		GameList.appendChild(Div);
	}
}

function INTERFACE_ShowAdminIcon()
{
	var Item = document.getElementById("admin_icon");
	var ItemTitle;

	if(Item != null)
	{
		ItemTitle = UTILS_GetText("menu_adjourn")
		Item.title = ItemTitle;
		Item.className = "adjourn_game";
		Item.id = "";
		UTILS_AddListener(Item,"click",function() { WINDOW_AdminWindow(); ADMIN_GetBanList(); }, "false");
	}
}

/**
* Show challange menu
*
* @return 	bool
* @author 	Ulysses
*/
/*
function INTERFACE_ShowChallengeMenu(OffsetLeft)
{
	var Challenge, MenuDiv, ChallengeList, RoomItem;
	var Node, Menu, Func, i, Hide = 0;

	Node = document.getElementById("Page");
	Menu = document.getElementById("ChallengeMenuDiv");

	if (!Node || Menu)
	{
		return null;
	}

	Func = function () {
		Hide += 1;
		
		if (Hide == 2)
		{
			UTILS_RemoveListener(document, "click", Func, false);

			// Remove menu from screen
			INTERFACE_HideChallengeList();
		}
	};

	// Creating elements
	MenuDiv = UTILS_CreateElement("div", "ChallengeMenuDiv");
	ChallengeList = UTILS_CreateElement("ul", "ChallengeMenuList");

	// Create elements and insert challenges
	for (i=0; i < MainData.ChallengeList.length; i++)
	{
		Challenge = UTILS_CreateElement("li", null, null, "<img src='images/cancel.png' onclick='GAME_DeclineChallenge("+MainData.ChallengeList[i].Id+")' /> "+MainData.ChallengeList[i].Username); 
		ChallengeList.appendChild(Challenge);
	}

	MenuDiv.style.left = (OffsetLeft-1)+"px";

	MenuDiv.appendChild(ChallengeList);
	Node.appendChild(MenuDiv);

	UTILS_AddListener(document, "click", Func, false);

	return true;
}
*/
