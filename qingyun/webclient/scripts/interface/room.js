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
* Control rooms interface
*/



/*********************************************
 * FUNCTIONS - ROOM INTERFACE OBJECT
 *********************************************/
/*
* Room Object
*
* @public
* @params       
* @return       void
* @author       Rubens
*/

function RoomObj(Roomname)
{
	var Room = INTERFACE_CreateRoom(Roomname);
	
	//Attributes
	this.roomName = Roomname;
	this.room = Room.RoomDiv;
	this.msgList = Room.MsgList;
	this.input = Room.Input;

	this.userListButton = Room.UserListButton;
	this.userListVisibility = false;

	this.userList = new UserListObj(Room.RoomDiv);
	this.userList.setSortUserFunction(ROOM_SortUsersByNick);
	this.userList.setSortRatingFunction(ROOM_SortUsersByRating);

	//Methods Public
	this.show = INTERFACE_ShowRoom;
	this.hide = INTERFACE_HideRoom;
	this.remove = INTERFACE_RemoveRoom;
	this.addMsg = INTERFACE_AddMsgInRoom;
	this.addMsgError = INTERFACE_AddMsgErrorInRoom;
	this.focus = INTERFACE_FocusRoomInput;
	this.showUserList = INTERFACE_ShowRoomUserList;
	this.hideUserList = INTERFACE_HideRoomUserList;

	this.hideUserList();
//	this.showUserList();
}


/**
* Create a room
* 
* @private
* @params	Room name;
* @return 	RoomDiv, User list and Message list in this room
* @authos	Pedro and Rubens
*/
function INTERFACE_CreateRoom(RoomName)
{
	var RoomDiv, RoomInside;
	var MessageList;
	var Input, Emoticon;
	var UserListVisibility;

	// General room
	RoomDiv = UTILS_CreateElement("div", "Room_"+RoomName, "Room");
	RoomDiv.style.display = "none";
	RoomInside = UTILS_CreateElement("div", "RoomInside_"+RoomName, "RoomInside");

	// Show/Hide user list
	UserListVisibility = UTILS_CreateElement("span",null,"UserListVisibility",UTILS_GetText("room_show_user_list"));
	UserListVisibility.onclick = function(){
		ROOM_ShowHideUserList(RoomName);
	}


	// MessageList
	MessageList = UTILS_CreateElement("ul", RoomName+"_Messages", "MessageList");
	Input = UTILS_CreateElement("input", "Input_"+RoomName);
	Input.type = "text";
	Input.onkeypress = function(event) {
		if ((UTILS_ReturnKeyCode(event) == 13) && (Input.value != ""))
		{
			if( Input.value.length <= MainData.MaxChatChar)
			{
				// Send message to room
				ROOM_SendMessage(RoomName, Input.value);
				Input.value = "";
			}
			else
			{
				ROOM_ErrorMessageLength(RoomName);
			}
		}
	}

	Emoticon = UTILS_CreateElement("img", null, "emoticon");
	Emoticon.src = "./images/emoticons/default.png";
	Emoticon.onclick = function () {
		INTERFACE_ShowEmoticonList(RoomName);
	}

	// MessageList
	MessageList = UTILS_CreateElement("ul", RoomName+"_Messages", "MessageList");
	Input = UTILS_CreateElement("input", "Input_"+RoomName);
	Input.type = "text";
	Input.onkeypress = function(event) {
		if ((UTILS_ReturnKeyCode(event) == 13) && (Input.value != ""))
		{
			if(Input.value.length < 2000)
			{
				// Send message to room
				ROOM_SendMessage(RoomName, Input.value);
				Input.value = "";
			}
			else
			{
				ROOM_ErrorMessageLength(RoomName);
			}
		}
	}

	Emoticon = UTILS_CreateElement("img", null, "emoticon");
	Emoticon.src = "./images/emoticons/default.png";
	Emoticon.onclick = function () {
		INTERFACE_ShowEmoticonList(RoomName);
	}

	RoomInside.appendChild(UserListVisibility);
	RoomInside.appendChild(MessageList);
	RoomInside.appendChild(Input);
	RoomInside.appendChild(Emoticon);

	RoomDiv.appendChild(RoomInside);

	return {RoomDiv:RoomDiv, MsgList:MessageList, Input:Input, UserListButton:UserListVisibility};
}

function INTERFACE_ShowRoom()
{
	var RoomMain;

	// The code above is used in specific case of this interface.
	// All rooms should be in "Rooms" div.
	RoomMain = document.getElementById("Rooms");
	if(this.room.parentNode != RoomMain)
	{
		//RoomMain = document.getElementById("Rooms");
		RoomMain.appendChild(this.room);
	}
	
	this.room.style.display = "block";
}

function INTERFACE_HideRoom()
{
	this.room.style.display = "none";
}

function INTERFACE_RemoveRoom()
{
	var RoomParent = this.room.parentNode;
	RoomParent.removeChild(this.room);
}

function INTERFACE_FocusRoomInput()
{
	this.input.focus();
}


function INTERFACE_ShowRoomUserList()
{
	this.userList.show();
	this.userListVisibility = true;

	this.userListButton.innerHTML = UTILS_GetText("room_hide_user_list");
	this.userListButton.className = "UserListVisibilityOn";
}

function INTERFACE_HideRoomUserList()
{
	this.userList.hide();
	this.userListVisibility = false;
	this.userListButton.innerHTML = UTILS_GetText("room_show_user_list");
	this.userListButton.className = "UserListVisibility";
}

function INTERFACE_AddMsgInRoom(Username, Msg, Timestamp)
{
	var Item;
	var Message, Time, EmoticonNum;

	Msg =UTILS_ConvertChatString(Msg);

	// Show emoticons
	while (Msg.match(/\[img{\d*}\]/) != null)
	{
		EmoticonNum = Msg.match(/\[img{\d*}\]/)[0];
		EmoticonNum = EmoticonNum.match(/\d+/);
		
		Msg = Msg.replace(/\[img{\d*}\]/, "<img src='./images/emoticons/"+EmoticonNum+".png' />");
	}

	// Get time from a given timestamp
	Time = UTILS_GetTime(Timestamp);

	Message = "<strong>"+Time+" "+Username+"</strong>: "+Msg;
	Item = UTILS_CreateElement("li", null, null, Message);

	this.msgList.appendChild(Item);
	this.msgList.scrollTop += Item.clientHeight + 1000;

	return true;
}

function INTERFACE_AddMsgErrorInRoom(Msg)
{
	var Item;

	Item = UTILS_CreateElement("li", null, "error", Msg);

	this.msgList.appendChild(Item);
	this.msgList.scrollTop += Item.clientHeight + 1000;
}

/* Refresh room's occupants number
*
* @param       RoomName
*              Room's name
* @return void
* @author Danilo 
*/
function INTERFACE_RefreshOccupantsNumber(RoomName)
{
	var N_Occupants;
	var Focused;
	var Node;

	// If general room
	if (RoomName == "general")
	{
		// Get element in interface that will be refreshed
		Node = document.getElementById("general_occupants");
	}
	else {
		// else get name of focused room
//		Focused = document.getElementById("RoomSecName").innerHTML;
	
		Focused = document.getElementById("Room_"+RoomName);

		// If change of occupant's number occured in focused room
		if ((Focused != null) && (Focused.style.display == 'block'))
		{
			// Get element in interface that will be refreshed
			Node = document.getElementById("Sec_occupants");
		}
		// else do nothing
		else
		{
			Node = null;
		}
	}

	// If Room is showed at interface, refresh the number of occupants
	if(Node)
	{
		// Get number of occupants in room data struct
		N_Occupants = MainData.RoomList[MainData.FindRoom(RoomName)].UserList.length;
		Node.innerHTML= " ("+N_Occupants+")";
	}
}

/*********************************************
 * FUNCTIONS - ROOM TOP MENU LIST 
 *********************************************/
/**
* Show room list in the room menu, if it exists
*
* @public
* @params       Array of room names
* @return       void
* @author       Pedro
*/
function INTERFACE_ShowRoomList(Rooms)
{
        var Node = document.getElementById("RoomMenuList");
        var Room, i;

        // If menu is not on screen
        if (!Node)
        {
                return null;
        }
        // If rooms was already been inserted
        else if (Node.childNodes.length > 0)
        {
                return null;
        }

        // Create elements and insert rooms
        for (i=0; i < Rooms.length; i++)
        {
                Room = UTILS_CreateElement("li", Rooms[i].Id, null, Rooms[i].Name);
                Room.onclick = function () {
									ROOM_EnterRoom(this.id);
                }
                Node.appendChild(Room);
        }
        return true;
}
/**
* Hide room list menu
*
* @public
* @params       void
* @return       void
* @author       Pedro
*/
function INTERFACE_HideRoomList()
{
        var Node = document.getElementById("RoomMenuDiv");

        if (!Node)
        {
                return false;
        }
        Node.parentNode.removeChild(Node);

				return true;
}

/********************************************
 * FUNCTIONS - ROOM GAME TOP MENU LIST 
 *********************************************/
/**
* Hide game room list menu
*
* @public
* @return       bool
* @author       Ulysses
*/
function INTERFACE_HideGameRoomList()
{
	var Node = document.getElementById("GameRoomMenuDiv");

	if (!Node)
	{
		return false;
	}
	Node.parentNode.removeChild(Node);

	return true;
}

/**
* Show game room list in the room menu
*
* @param 	Rooms An Array with game rooms
* @return 	bool
* @author 	Ulysses
*/
function INTERFACE_ShowGameRoomList(GameId, PW, PB, GameType)
{
	// Get game menu
	var Node = document.getElementById("GameRoomMenuDiv");
	var List;
	var Room, i;

	if (Node == null)
	{
		return false;
	}
	else
	{
		// Get default list
		List = document.getElementById("GameRoomMenuList"+UTILS_Capitalize(GameType));
		// If list doesn't exists, create one
		if(List == null)
		{
			List = UTILS_CreateElement("ul","GameRoomMenuList"+UTILS_Capitalize(GameType),null, UTILS_GetGameCategory(GameType));
			Node.appendChild(List);
		}
	}

	// Create elements and insert rooms
	Room = UTILS_CreateElement("li", null, null, PW.Name+" x "+PB.Name);

	Room.onclick = function(){
		var Buffer="";
		var To;

		//Check if user is not playing or observe a game
		if(MainData.CurrentGame == null)
		{
			if((PB.Name!= MainData.Username) &&(PW.Name != MainData.Username))
			{
				Buffer += GAME_StartObserverGame(GameId, PW, PB);				
			}
			else
			{
				//Open game board and enter game in room
				Buffer += GAME_StartGame(GameId, PW, PB);
				To = GameId+"@"+MainData.GameComponent+"."+MainData.Host+"/"+MainData.Username;
				Buffer += MESSAGE_Presence(To)
			}
		}
		else
		{
			WINDOW_Alert(UTILS_GetText("game_observer_alert_title"), UTILS_GetText("game_observer_alert"));
		}
		CONNECTION_SendJabber(Buffer);
	}

	// Insert item in current game list
	List.appendChild(Room);
	//List.style.visibility = "visible";
	return true;
}

/*********************************************
 * FUNCTIONS - ROOM LIST (Right of rooms div)
 *********************************************/
/** 
* Show or hide list of user's rooms 
* 
* @public 
*/ 
function INTERFACE_ChangeRoomListVisibility() 
{ 
	var Div, List, Node, Item, i; 
	var Menu = document.getElementById('RoomListMenu'); 
	var Node = document.getElementById('RoomList'); 

	// If already exists room list menu, hide it 
	if (Menu != null) 
	{ 
		Menu.parentNode.removeChild(Menu); 
		return true; 
	} 
	//else show (create) menu 
	Div = UTILS_CreateElement("div", "RoomListMenu"); 
	List = UTILS_CreateElement('ul'); 

	Div.style.position = "absolute"; 
	 
	// Population list with user's rooms 
	for (i=0; i < MainData.RoomList.length; i++) 
	{ 
		Item = UTILS_CreateElement('li'); 
		if (MainData.RoomList[i].Name == MainData.RoomDefault)
		{
			Item.innerHTML = UTILS_GetText("room_default"); 
		}
		else
		{
			Item.innerHTML = MainData.RoomList[i].Name; 
		}
		Item.onclick = function () { 
			if (this.innerHTML == UTILS_GetText("room_default"))
			{
				ROOM_FocusRoom(MainData.RoomDefault); 
			}
			else
			{
				ROOM_FocusRoom(this.innerHTML); 
			}
			INTERFACE_ChangeRoomListVisibility(); 
		} 
		List.appendChild(Item); 
	} 

	Div.appendChild(List); 

	try 
	{ 
		document.getElementById('Rooms').insertBefore(Div, Node); 
	} 
	catch(e) 
	{ 
		return false; 
	} 
	return true; 
}




/**
* Remove a room from room list if RoomListMenu is opened
*
* @param        Room name that will be removed from list
* @return       void
* @author       Rubens
*/
function INTERFACE_RemoveRoomFromList(Room)
{
	var Menu = document.getElementById("RoomListMenu");
	var ListItens;
	var i=0;

	// if list is not opened, then do nothing;
	if(Menu == null)
	{
		return;
	}

	ListItens = Menu.getElementsByTagName("li");

	while( (ListItens[i].innerHTML != Room) && (i<ListItens.length) )
	{
		i++;
	}

	// If room founded in list, then remove it from;
	if(i != ListItens.length)
	{
		ListItens[i].parentNode.removeChild(ListItens[i]);
	}
}





/**********************************
 * FUNCTIONS - EMOTICONS LIST
 ***********************************/
/**
* Show emoticon list
*
* @public
*/
function INTERFACE_ShowEmoticonList(RName)
{
	var Div, List, Item, Img, i;
	var Func, Hide = 0;
	var RoomName = RName;

	Func = function () {
		Hide += 1;

		if (Hide == 2)
		{
			UTILS_RemoveListener(document, "click", Func, false);
			INTERFACE_HideEmoticonList();
		}
	};

	Div = UTILS_CreateElement("div", "EmoticonDiv");
	List = UTILS_CreateElement("ul", "EmoticonList");

	for (i=0; i<MainData.EmoticonNum; i++)
	{
		Item = UTILS_CreateElement("li");
		Img = UTILS_CreateElement("img", null, i);
		Img.src = "./images/emoticons/"+i+".png";
		Img.onclick = function () {
			var Node = document.getElementById("Input_"+RoomName);
			var Num = i;

			if (!Node)
			{
				return null;
			}
			Node.value += "[img{"+this.className+"}] ";
			Node.focus();

			return true;
		}

		Item.appendChild(Img);
		List.appendChild(Item);
	}
	Div.appendChild(List);

	document.getElementById("RoomInside_"+RoomName).appendChild(Div);

	UTILS_AddListener(document, "click", Func, false);
}

/**
* Hide emoticon list
*
* @public
*/
function INTERFACE_HideEmoticonList()
{
	var Node = document.getElementById("EmoticonDiv");

	if (!Node)
	{
		return null;
	}

	Node.parentNode.removeChild(Node);
	return true;
}



/**********************************
 * FUNCTIONS - CHANGE ROOM BAR
 ***********************************/

/**
* Give focus to a room in change room bar
*
* @public
*/
function INTERFACE_FocusRoom(RoomName)
{
	var RoomList = document.getElementById("RoomList");
	var RoomItem, RoomClose, Current, NewRoom, Node, ShortName;

	if (RoomList == null)
	{
		return null;
	}

	// Focus to default room
	if (RoomName == MainData.RoomDefault)
	{
		RoomList.childNodes[0].className = "room_selec";

		// If exists other rooms, remove focus
		if (RoomList.childNodes.length > 2)
		{
			RoomList.childNodes[1].className = "";
		}
	}
	// Already have 2 opened rooms
	else if (RoomList.childNodes.length > 2)
	{
		Node = document.getElementById("RoomSecName");
		if (!Node)
		{
			return null;
		}

		Node.parentNode.onclick = function () {
			ROOM_FocusRoom(RoomName);
		}

		if(RoomName.length > 4)
		{
			ShortName = UTILS_ShortString(RoomName, 4);
			Node.innerHTML = ShortName;
			Node.onmouseover = function() { INTERFACE_ShowFullName(this, RoomName); }
			Node.onmouseout = function() { INTERFACE_CloseFullName(); }
		}
		else
		{
			Node.innerHTML = RoomName;
		}
		RoomList.childNodes[1].className = "room_selec";
		RoomList.childNodes[0].className = "";
		RoomClose = document.getElementById("CloseRoom");
		RoomClose.onclick = function() { ROOM_ExitRoom(RoomName); };
	}

	return true;
}

/* Rubens
 */
function INTERFACE_CreateRoomInBar(RoomName)
{
	var RoomList = document.getElementById("RoomList");
	var RoomItem, RoomClose, ShortName;
	var RoomItemTitle, RoomOccupants;

	//Create Room default
	if(RoomList.childNodes.length == 1)
	{
		if (UTILS_GetText("room_default") > 4)
		{
			RoomItemTitle = UTILS_CreateElement("span",null,null,UTILS_GetText("room_default"));
			RoomItemTitle.onmouseover = function() { INTERFACE_ShowFullName(this, UTILS_GetText("room_default")); }
			RoomItemTitle.onmouseout = function() { INTERFACE_CloseFullName(); }
		}
		else
		{
			RoomItemTitle = UTILS_CreateElement("span",null,null,UTILS_GetText("room_default"));
		}
		RoomItemTitle.style.fontWeight = "bold";
		RoomItem = UTILS_CreateElement("li","RoomPrimary");
		RoomItem.appendChild(RoomItemTitle);
		RoomOccupants = UTILS_CreateElement("span",MainData.RoomDefault+"_occupants",null," (0)");
		RoomItem.appendChild(RoomOccupants);

		RoomItem.onclick = function () {
			ROOM_FocusRoom(RoomName);
		}

		RoomList.insertBefore(RoomItem, RoomList.childNodes[0]);
	}
	else if(RoomList.childNodes.length == 2)
	{
		// Create a item and set focus to it
		if(RoomName > 4)
		{
			ShortName = UTILS_ShortString(RoomName, 4);
			RoomItemTitle = UTILS_CreateElement("span","RoomSecName",null,ShortName);
			RoomItemTitle.onmouseover = function() { INTERFACE_ShowFullName(this, RoomName); }
			RoomItemTitle.onmouseout = function() { INTERFACE_CloseFullName(); }
		}
		else
		{
			RoomItemTitle = UTILS_CreateElement("span","RoomSecName",null,RoomName);
		}
		RoomItemTitle.style.fontWeight = "bold";
		RoomItem = UTILS_CreateElement("li", "RoomSecondary");
		RoomItem.appendChild(RoomItemTitle);
		RoomOccupants = UTILS_CreateElement('span',"Sec_occupants",null," (0)");
		RoomItem.appendChild(RoomOccupants);

		RoomItem.onclick = function () {
			ROOM_FocusRoom(RoomName);
		}

		RoomClose = UTILS_CreateElement("img", "CloseRoom", "close");
		if (MainData.Browser == 0)
		{
			RoomClose.src = "./images/ie/close_chat.gif";
		}
		else
		{
			RoomClose.src = "./images/close_chat.png";
		}
		RoomClose.onclick = function() { ROOM_ExitRoom(RoomName); };
		RoomItem.appendChild(RoomClose);

		RoomList.insertBefore(RoomItem, RoomList.childNodes[1]);
	}
}

/**
* Close the room that are displayed
* as a secondary room 
*
* @public
*/
function INTERFACE_CloseRoom(RoomName)
{
	var NodeParent, Node, Div;

	// Find element
	Node = document.getElementById("RoomSecondary");

	if (Node == null)
	{
		return null;
	}

	// Remove room's div
	Div = document.getElementById("Room_"+RoomName);
	Div.parentNode.removeChild(Div);

	// Remove from change room bar
	NodeParent = Node.parentNode;
	NodeParent.removeChild(Node);
	
	return RoomName;
}



/**********************************
 * FUNCTIONS - START INTERFACE
 ***********************************/
/**
* Create rooms div
*
* @private
*/
function INTERFACE_CreateRooms()
{
	var RoomDiv, RoomsDiv, RoomsList, RoomsListGeneral, RoomsListArrow, Arrow;


	// Room list
	RoomsDiv = UTILS_CreateElement("div", "Rooms");
	RoomsList = UTILS_CreateElement("ul", "RoomList");
	RoomsListGeneral = UTILS_CreateElement("li", null, "room_selec", UTILS_GetText("room_default"));

	RoomsListArrow = UTILS_CreateElement("li", null, "room_arrow");
	RoomsListArrow.onclick = function () { INTERFACE_ChangeRoomListVisibility(); };
	Arrow = UTILS_CreateElement("img");
	Arrow.src = "images/room_arrow.png";

	// Creating DOM tree
	RoomsListArrow.appendChild(Arrow);
	
	RoomsList.appendChild(RoomsListArrow);

	RoomsDiv.appendChild(INTERFACE_CreateLoadingBox("room_loading",UTILS_GetText("room_loading")));
	RoomsDiv.appendChild(RoomsList);
	//RoomsDiv.appendChild(RoomDiv.RoomDiv);

	return RoomsDiv;
}

/**********************************
 * FUNCTIONS - ROOMS WINDOWS
 ***********************************/


/**
 * Create elements to create room window and return divs and array of buttons
 *
 * @ return     Div, Array 
 * @ see        WINDOW_CreateRoom();
 * @ author     Danilo Kiyoshi Simizu Yorinori
 */

function INTERFACE_ShowCreateRoomWindow()
{
	var Div;

	var OptionsDiv;
	var Label, Input, Br;
	var Description, Textarea;

	var CounterDiv;
	var CounterInput, CounterLabel;

	var ButtonsDiv;
	var Create, Cancel;

	var RoomName;
	var Buttons = new Array();

	// Main Div
	Div = UTILS_CreateElement('div', 'CreateRoomDiv');

	// Options Div
	OptionsDiv = UTILS_CreateElement('div', 'OptionsDiv');

	// Room Name Input
	Label = UTILS_CreateElement('p', null, null, UTILS_GetText('room_name'));
	Input = UTILS_CreateElement('input','CreateRoomInputName');
	Br = UTILS_CreateElement('br');

	CounterDiv = UTILS_CreateElement('div', 'CounterDiv');
	CounterInput = UTILS_CreateElement("input",null,"counter_input");
	CounterInput.type="text";
	CounterInput.value = 30;
	CounterInput.setAttribute("size",2);
	CounterInput.readOnly = true;
	CounterLabel = UTILS_CreateElement("span",null,null,UTILS_GetText("window_character").replace(/%s/,"30"));
	CounterLabel.innerHTML.replace(/%s/,"aaa");

	Input.type = "text";
	Input.setAttribute("size",22);
	Input.maxLength = 30;
	Input.onkeypress = function(event) {

		if (UTILS_ReturnKeyCode(event) == 13) // enter key pressed
		{
			if (Input.value == '' || Input.value == null)
			{
				return;
			}

			RoomName = Input.value.replace(/ /g,"_"); // replace ' ' with '_'

			if (RoomName == UTILS_GetText("room_default"))
			{
				WINDOW_Alert(UTILS_GetText('room_error'),UTILS_GetText('room_invalid_name'));
				CounterInput.value = 0;
				Input.value = "";
				return;
			}
			if (RoomName.length > 30)
			{
				WINDOW_Alert(UTILS_GetText('room_error'),UTILS_GetText('room_invalid_length'));
				CounterInput.value = 0;
				Input.value = "";
				return;
			}
			// message to create room
			else
			{
				CONNECTION_SendJabber(MESSAGE_Presence(RoomName+"@conference."+MainData.Host+"/"+MainData.Username));
			}
		}
	};

	Input.onkeyup = function() {
		CounterInput.value = 30 - Input.value.length;
		CounterLabel.innerHTML = UTILS_GetText("window_character").replace(/%s/,30 - Input.value.length);
	}
	
	// TODO - not implemented
	// Room Description Input 
	Description = UTILS_CreateElement('p',null,null,UTILS_GetText('room_description'));
	Textarea = UTILS_CreateElement('textarea','CreateRoomTextarea');
	Textarea.rows = "3";
	Textarea.cols = "20";
	Textarea.disabled = true; // Disable textarea field - Option not implemented

	// Buttons Div
	ButtonsDiv = UTILS_CreateElement('div', 'ButtonsDiv');

	// Create Button
	Create = UTILS_CreateElement('input',null,'button');
	Create.type = "button";
	Create.value = UTILS_GetText('room_create');

	Create.onclick = function() {
		if (Input.value == '' || Input.value == null)
		{
			return;
		}

		RoomName = Input.value.replace(/ /g,"_");
		if (RoomName == UTILS_GetText("room_default"))
		{
			WINDOW_Alert(UTILS_GetText('room_error'),UTILS_GetText('room_invalid_name'));
			Input.value = "";
			CounterInput.value = 0;
			return;
		}
		else if (RoomName.length > 30)
		{
			WINDOW_Alert(UTILS_GetText('room_error'),UTILS_GetText('room_invalid_length'));
			CounterInput.value = 0;
			Input.value = "";
			return;
		}
		// Send a message to create room
		else
		{
			CONNECTION_SendJabber(MESSAGE_Presence(RoomName+"@conference."+MainData.Host+"/"+MainData.Username));
		}
	};

	// Cancel Button
	Cancel = UTILS_CreateElement('input',null,'button');
	Cancel.type = "button";
	Cancel.value = UTILS_GetText('window_cancel');

	// Mount elements tree
	
	// Counter Div
//	CounterDiv.appendChild(CounterInput);
	CounterDiv.appendChild(CounterLabel);

	// Options Div
	OptionsDiv.appendChild(Label);
	OptionsDiv.appendChild(Input);
	OptionsDiv.appendChild(Br);
	OptionsDiv.appendChild(CounterDiv);
//	OptionsDiv.appendChild(Description);
//	OptionsDiv.appendChild(Textarea);

	// Buttons Div
	ButtonsDiv.appendChild(Create);
	ButtonsDiv.appendChild(Cancel);

	// Main Div
	Div.appendChild(OptionsDiv);
	Div.appendChild(ButtonsDiv);

	// Insert buttons in Buttons array
	Buttons.push(Create);
	Buttons.push(Cancel);
	Buttons.push(Input);

	return {Div:Div, Buttons:Buttons};
}

/**
 * Create elements to cancel room creation window and return divs and array of buttons
*
* @return     Div, Array 
* @see        WINDOW_CancelRoom();
* @author     Danilo Kiyoshi Simizu Yorinori
*/
function INTERFACE_ShowCancelRoomWindow()
{
	var Div;

	var TextDiv;
	var Label;

	var ButtonsDiv;
	var Yes, No;

	var RoomName;
	var Buttons = new Array();

	// Main Div
	Div = UTILS_CreateElement('div', 'CancelRoomDiv');

	// Text Div
	TextDiv = UTILS_CreateElement('div', 'TextDiv');
	Label = UTILS_CreateElement('p', null, null, UTILS_GetText('room_cancel_text'));

	// Buttons Div
	ButtonsDiv = UTILS_CreateElement('div', 'ButtonsDiv');

	// Yes Button
	Yes = UTILS_CreateElement('input',null,'button');
	Yes.type = "button";
	Yes.value = UTILS_GetText('window_yes');

	// No Button
	No = UTILS_CreateElement('input',null,'button');
	No.type = "button";
	No.value = UTILS_GetText('window_no');

	// Mount elements tree
	// Text Div
	TextDiv.appendChild(Label);

	// Buttons Div
	ButtonsDiv.appendChild(Yes);
	ButtonsDiv.appendChild(No);

	// Main Div
	Div.appendChild(TextDiv);
	Div.appendChild(ButtonsDiv);

	// Insert buttons in Buttons array
	Buttons.push(Yes);
	Buttons.push(No);

	return {Div:Div, Buttons:Buttons};
}

