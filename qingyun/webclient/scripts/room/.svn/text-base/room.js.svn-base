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
* Room controller
*/


/*********************************
 * FUNCTIONS - PARSERS
 *********************************/
/**
* Handle all presence from a room;
*
* @param 	XML The xml come from server with tag presence
* @return 	void
* @author 	Ulysses
*/
function ROOM_HandleRoomPresence(XML)
{
	var From, RoomName, Jid, Type, Item, Role, Affiliation, Show, Status, MsgTo, NewRoom = false;
	var Room;
	var Buffer = "";

	var Component;
	
	var LoadingBox;

	var RoomNotFound;

	// Get Attributes from XML
	Item = XML.getElementsByTagName("item");
	Show = XML.getElementsByTagName("show");
	From = XML.getAttribute('from');
	Type = XML.getAttribute('type');
	RoomName = From.replace(/@.*/, "");
	Jid = From.replace(/.*\//, "");
	MsgTo = From.replace(/\/.*/, "");

	Component = From.split("@")[1].split("/")[0].split(".")[0];


	// Check if the type is error
	if (Type == "error")
	{
		// Check if error is a inexist room; -> QuickFix to close room
		RoomNotFound = XML.getElementsByTagName("item-not-found")[0];

		if(RoomNotFound != null)
		{
			ROOM_RemoveRoom(RoomName);
		}

		return Buffer;
	}

	if(Item.length > 0)
	{
		Role = Item[0].getAttribute("role");
		Affiliation = Item[0].getAttribute("affiliation");
	}
	else
	{
		Role = "participant";
		Affiliation = "none";
	}

	// Remove loading box if user enter in general room;
	if(RoomName == MainData.RoomDefault)
	{
		LoadingBox = document.getElementById("room_loading");
		if(LoadingBox != null)
		{
			LoadingBox.parentNode.removeChild(LoadingBox);
		}
	}


	// Status of user
	if (Show.length > 0)
	{
		// Get status name
		Status = UTILS_GetNodeText(Show[0]);

		// Any different status, status = away
		if ((Status != "busy") && (Status != "away") && (Status != "unavailable") && (Status != "playing"))
		{
			Status = "away";
		}
	}
	// If tag 'show' doesnt exists, status online
	else
	{
		Status = "available";
	}

	// If 'RoomName' doesnt exists, insert it in strucutre
	// and show on the interface
	if (MainData.FindRoom(RoomName) == null)
	{
		ROOM_CreateRoom(RoomName);

		// Show room user list if room is a room game
		if(Component == MainData.GameComponent)
		{
			Room = MainData.GetRoom(RoomName);
			Room.Room.showUserList();
		}
	}

	// If its your presence
	if (Jid == MainData.Username)
	{
		if (Type == "unavailable")
		{
			ROOM_RemoveRoom(RoomName);
		}
		else
		{
			// Insert you in room user list
			Buffer += ROOM_AddUser(RoomName, Jid, Status, Role, Affiliation);
			// Set your role and affiliation in data struct
			MainData.SetRoom(RoomName, MsgTo, Role, Affiliation);
			INTERFACE_RefreshOccupantsNumber(RoomName);
		}
	}
	// Presence of others users
	else
	{
		if (Type == "unavailable")
		{
			//MainData.DelUserInRoom(RoomName, Jid);
			//INTERFACE_DelUserInRoom(RoomName, Jid);
			ROOM_RemoveUser(RoomName, Jid);
			INTERFACE_RefreshOccupantsNumber(RoomName);
		}
		else
		{
			Buffer += ROOM_AddUser(RoomName, Jid, Status, Role, Affiliation);
			INTERFACE_RefreshOccupantsNumber(RoomName);
		}
	}
	return Buffer;
}



/**
* Handle group chat messages
*
* @param 	XML The xml come from server with tag message
* @return 	void
* @author 	Ulysses
*/
function ROOM_HandleMessage(XML)
{
	var From, RoomName, Message, Body, X, Stamp = null;


	// Get the Chat Room name
	RoomName = XML.getAttribute('from').replace(/@.*/,"");

	// Get the message sender
	From = XML.getAttribute('from').replace(/.*\//,"");

	// Get stamp (old messages)
	X = XML.getElementsByTagName('x');

	if (X.length > 0)
	{
		Stamp = X[0].getAttribute('stamp');
	}

	Body = XML.getElementsByTagName("body");

	// If there is a body
	if (Body.length > 0)
	{
		// Get the message
		Message = UTILS_GetNodeText(Body[0]);
	}

	// Show message on interface
	ROOM_ShowMessage(RoomName, From, Message, Stamp);
	
	return "";
}

/**
* Handle room list in top menu.
*
* @param 	XML The xml that the server send's
* @return 	void
* @author 	Ulysses
*/
function ROOM_HandleRoomList(XML)
{
	var Items, Rooms, Room, RoomName, ID, i;
	var Buffer = "";
	var Exp = new RegExp("^"+MainData.RoomDefault+"$");

	Rooms = new Array();

	// Get the ID 
	ID = XML.getAttribute("id");
	
	// XML with all games rooms
	if (ID == MainData.Const.IQ_ID_GetGamesList)
	{
		Buffer += ROOM_HandleGameRoomList(XML);
	}
	// Chat Room List
	else
	{
		// Get items in XML
		Items = XML.getElementsByTagName("item");
		
		// Find room names
		for (i=0; i < Items.length; i++)
		{
			Room = new Object();

			Room.Id = Items[i].getAttribute("jid").split("@")[0];
			Room.Name = Items[i].getAttribute("name").replace(/ /,"");;

			Rooms[i] = Room;

			// Change name for general room
			if (Rooms[i].Id.match(Exp))
			{
				Rooms[i].Name = UTILS_GetText("room_default") + "(" + Rooms[i].Name.split("(")[1];
			}
		}
		INTERFACE_ShowRoomList(Rooms);
	}

	INTERFACE_RemoveLoadBox();

	return Buffer;
}

/**
* Handle game room list, and resend a request for game information for each
* game.
*
* @param 	XML The xml that the server send's
* @return 	void
* @author 	Ulysses
*/
function ROOM_HandleGameRoomList(XML)
{
	var Items, i;
	var Rooms = new Array();
	var Name, WName, BName, Jid, GameId;
	var P1, P2;
	var XMPP="";

	// Get items in XML
	Items = XML.getElementsByTagName("item");

	if(Items.length == 0)
	{
		// interface/top.js
		INTERFACE_NoGamesInGameList();
	}
	else
	{
		// Get the player's names
		for (i=0; i<Items.length; i++)
		{

			Jid = Items[i].getAttribute("jid");
			GameId = Jid.split("@")[0];
			XMPP += MESSAGE_GameRoomInfoList(GameId);
		}
	}

	return XMPP;
}

/**
* Handle game room information. Get game room information and show in interface
*
* @param 	XML The xml that the server send's
* @return 	void
* @author 	Rubens
*/
function ROOM_HandleGameRoomInfoList(XML)
{
	var PW = new Object();
	var PB = new Object();

	var Iq;
	var Identity, Name;
	var Game, GameType;
	var WName, BName;
	var Jid, GameId;
	var Players;

	
	//Identity = XML.getElementsByTagName("identity")[0];
	//Name = Identity.getAttribute("name");

	Jid = XML.getAttribute("from");
	GameId = Jid.split("@")[0];

	Game = XML.getElementsByTagName("game")[0];
	GameType = Game.getAttribute("category");

	Players = XML.getElementsByTagName("player");

	//WName = Name.split(" x ")[0].split("@")[0].replace(" ","");
	//BName = Name.split(" x ")[1].split("@")[0].replace(" ","");
	if(Players[0].getAttribute("role") == "white")
	{
		PW.Name = Players[0].getAttribute("jid").split("@")[0];
		PW.Time = Players[0].getAttribute("time");
		PW.Color = "white";
		PW.Inc = Players[0].getAttribute("inc");

		PB.Name = Players[1].getAttribute("jid").split("@")[0];
		PB.Time = Players[1].getAttribute("time");
		PB.Color = "black";
		PB.Inc = Players[1].getAttribute("inc");
	}
	else
	{
		PW.Name = Players[1].getAttribute("jid").split("@")[0];
		PW.Time = Players[1].getAttribute("time");
		PW.Color = "white";
		PW.Inc = Players[1].getAttribute("inc");

		PB.Name = Players[0].getAttribute("jid").split("@")[0];
		PB.Time = Players[0].getAttribute("time");
		PB.Color = "black";
		PW.Inc = Players[0].getAttribute("inc");
	}

	// interface/room.js
	INTERFACE_ShowGameRoomList(GameId, PW, PB, GameType);

	return "";
}

/**
* Handle chess user information.
* @param XML	XMPP that contains users ratings and type.
* @author	Rubens Suguimoto
*/
function ROOM_HandleInfo(XML)
{
	var RatingNodes, TypeNodes;

        var Username, Rating, Category
	var i,j;
	var Room;
	var Status, Rating;
	var User;
	var NewType, Type;

	RatingNodes = XML.getElementsByTagName('rating');
	TypeNodes = XML.getElementsByTagName('type');


	// Getting ratings nodes
	for (i=0 ; i<RatingNodes.length ; i++)
	{
		Username = RatingNodes[i].getAttribute('jid').replace(/@.*/,"");
		Category = RatingNodes[i].getAttribute('category');
		Rating = RatingNodes[i].getAttribute('rating');

		// Updating ratings in room lists
		for (j=0; j<MainData.RoomList.length; j++)
		{
			// Search user node in room user list
			User = MainData.FindUserInRoom(MainData.RoomList[j].Name, Username);
			if (User != null)
			{
				Room = MainData.RoomList[j];
				Status = Room.UserList[User].Status;
				Type = Room.UserList[User].Type;

				// Update in interface 
				if (Category == MainData.RoomCurrentRating)
				{
					Room.Room.userList.updateUser(Username, Status, Rating, Type);
				}
			}
		}

	}

	// Getting users type nodes
	for (i=0 ; i<TypeNodes.length ; i++)
	{
		Username = TypeNodes[i].getAttribute('jid').replace(/@.*/,"");
		NewType = TypeNodes[i].getAttribute('type');
	
		// Updating type in room lists
		for (j=0; j<MainData.RoomList.length; j++)
		{
			// Search user node in room user list
			User = MainData.FindUserInRoom(MainData.RoomList[j].Name, Username);
			if (User != null)
			{
				Room = MainData.RoomList[j];
				Status = Room.UserList[User].Status;
				Room.UserList[User].Type = NewType;

				// Update in interface
				if(NewType != "user")
				{
					// Search user node in room user list
					Room.Room.userList.updateUser(Username, Status, null, NewType);	
				}
			}
		}
	}
	return "";
}

/**
* Send a message to room;
*
* @param 	RoomName is the room name string
* @param	Message is the message that will be send
* @return 	void
* @author 	Rubens
*/
function ROOM_SendMessage(RoomName, Message)
{
	var To, i;

	// Search room in sctructure
	i = MainData.FindRoom(RoomName);

	// If room doesnt exists
	if (i == null)
	{
		return false;
	}

	// Send message to room
	To = MainData.RoomList[i].MsgTo;
	CONNECTION_SendJabber(MESSAGE_GroupChat(To, UTILS_ConvertChatString(Message)));
	return true;
}

/******************************
 * FUNCTIONS - TOP MENU
 ******************************/

/**
* Get Room list from server and show in pop down in top menu
*
* @param 	OffsetLeft is the position where room menu will be show
* @return 	void
* @author 	Rubens
*/

function ROOM_ShowRoomList(OffsetLeft)
{
	var XML = MESSAGE_RoomList();

	// Ask room list for jabber
	CONNECTION_SendJabber(XML);

	// Show menu on interface
	INTERFACE_ShowRoomMenu(OffsetLeft);
}

/**
* Get Game Room list from server and show in pop down top menu
*
* @param 	offsetleft is the position where room menu will be show
* @return 	void
* @author 	ulysses
*/
function ROOM_ShowGameRoomList(OffsetLeft)
{
	var XML = MESSAGE_GameRoomList();

	// Ask room list for jabber
	CONNECTION_SendJabber(XML);

	// Show menu on interface
	INTERFACE_ShowGameRoomMenu(OffsetLeft)
}


/**
* Send presence to a room (enter room)
*
* @param 	Room name is the name of room
* @return 	string ""
* @author 	Ulysses and Danilo
*/
function ROOM_EnterRoom(RoomName)
{
	var XML, To;

	var Room;

	// Send Message to general room - must be change to Focus Room
	Room = MainData.GetRoom(RoomName);

	if (Room != null)
	{
		ROOM_FocusRoom(RoomName);
	}
	else
	{
		To = RoomName+"@conference."+MainData.Host+"/"+MainData.Username;

		XML = MESSAGE_Presence(To);

		CONNECTION_SendJabber(XML);
	}

	return "";
}

/**
* Exit a room.
* @param 	ReturnMsg is a flag to return XML or send Jabber(if ReturnMsg is null)
* @return	XMPP with presence unavailable to a room
* @author	Pedro and Rubens
*/
function ROOM_ExitRoom(RoomName)
{
	// This function send a message to leave from room;
	// ROOM_RemoveRoom function remove room from data struct and interface,
	// and ROOM_RemoveRoom function is called when parse presence
	// type = unavailable

	var XML;
	var Room = MainData.GetRoom(RoomName)
	
	// If RoomName isn't in sctructure
	if (Room == null)
	{
		return "";
	}

	// if user is playing, show a message and don't close room
	if(MainData.CurrentGame != null)
	{
		if(RoomName == MainData.CurrentGame.Id)
		{
			if(MainData.CurrentGame.Finished == false)
			{
				WINDOW_Alert(UTILS_GetText("game_remove_game_title"), UTILS_GetText("game_remove_game"));
				return "";
			}
		}
	}

	XML = MESSAGE_Unavailable(Room.MsgTo);
	CONNECTION_SendJabber(XML);
	
	return XML;
}

/**
* Send presence to a room game(enter room game)
*/
function ROOM_EnterRoomGame(RoomName)
{
	var XML, To;

	To = RoomName+"@"+MainData.GameComponent+"."+MainData.Host+"/"+MainData.Username;

	XML = MESSAGE_Presence(To);

	CONNECTION_SendJabber(XML);

	return true;
}



function ROOM_ShowMessage(RoomName, From, Message, Stamp)
{
	var Room = MainData.GetRoom(RoomName);

	if(Room == null)
	{
		return "";
	}
	
	Room.Room.addMsg(From, Message, Stamp);

	return "";
}

function ROOM_ErrorMessageLength(RoomName)
{
	var Room = MainData.GetRoom(RoomName);
	var Message;
	var Limit = MainData.MaxChatChar;

	if(Room == null)
	{
		return "";
	}
	
	Message = UTILS_GetText("room_error_message_length");
	Message = Message.replace("%s",Limit);
	Room.Room.addMsgError(Message);

	return "";

}

/** 
* Insert user in room list 
*/ 
function ROOM_AddUser(RoomName, Jid, Status, Role, Affiliation) 
{ 
	var UserPos = MainData.FindUser(Jid); 
	var Type = "user", Rating = ""; 
	var Buffer = "";
	var Room;

	Room = MainData.GetRoom(RoomName);
	if(Room == null)
	{
		return null;
	}

	// Check if user has already inserted. 
	// If not inserted, add user, else update information
	if(Room.Room.userList.findUser(Jid) == null)
	{
		/* //Review this code to be more 
		if (UserPos != null) 
		{ 
			Type = MainData.UserList[UserPos].Type; 
			Rating = eval("MainData.UserList["+UserPos+"].Rating."+UTILS_Capitalize(MainData.CurrentRating)); 
		} 
		else if (Jid == MainData.Username) 
		{ 
			Type = MainData.Type; 
			Rating = eval("MainData.Rating."+UTILS_Capitalize(MainData.CurrentRating)); 
		} 
		else 
		{
			// Ask user info, if it's not your contact 
			Buffer += MESSAGE_Info(Jid); 
		} 
		*/

		// Get user rating and type information
		Buffer += MESSAGE_Info(Jid); 

		//Add user in data struct
		MainData.AddUserInRoom(RoomName, Jid, Status, Type, Role, Affiliation); 
		MainData.SortUserByNickInRoom(RoomName); 
	
		//Add user in interface
		Room.Room.userList.addUser(Jid, Status, Rating, Type); 
	} 
	else
	{
		//Update user information in data struct
		MainData.SetUserAttrInRoom(RoomName, Jid, Status, Role, Affiliation) 
		//Update user information in interface
		Room.Room.userList.updateUser(Jid, Status); 
	} 
	return Buffer; 
}

function ROOM_RemoveUser(RoomName, UserName)
{
	var Room = MainData.GetRoom(RoomName);
	
	MainData.DelUserInRoom(RoomName,UserName)
	
	// Remove user from interface
	Room.Room.userList.removeUser(UserName);

	return "";
}

function ROOM_CreateRoom(RoomName)
{
	var Room;

	// Create a room object
	Room = new RoomObj(RoomName);

	// Add a room in data struct
	MainData.AddRoom(RoomName, null, null, null, Room);

	// Show new room in interface
	INTERFACE_CreateRoomInBar(RoomName);
	// Set focus and current room
	ROOM_FocusRoom(RoomName);
}

function ROOM_FocusRoom(RoomName)
{
	var Room = MainData.GetRoom(RoomName);

	if((Room != MainData.CurrentRoom)&&(MainData.CurrentRoom != null))
	{
		// Hide current room div;
		MainData.CurrentRoom.Room.hide();
	}

	// Show new room and set it to current
	INTERFACE_FocusRoom(RoomName);
	Room.Room.show();
	INTERFACE_RefreshOccupantsNumber(RoomName);
	MainData.CurrentRoom = Room;
	Room.Room.focus();
}

function ROOM_RemoveRoom(RoomName)
{
	var Room = MainData.GetRoom(RoomName);
	var NextRoom, NextRoomPos;

	if(Room == null)
	{
		return "";
	}
	
	//Remove room from interface
	INTERFACE_RemoveRoomFromList(RoomName);
	ROOM_FocusRoom(RoomName);
	INTERFACE_CloseRoom(RoomName);
	Room.Room.hide();

	//Remove from data struct
	MainData.DelRoom(RoomName)


	if(MainData.RoomList.length > 1)
	{
		//Get next room from data struct
		//RoomList[0] == General Room
		NextRoomPos = MainData.RoomList.length - 1;
		NextRoom = MainData.RoomList[NextRoomPos];
	
		//Show next room in interface
		INTERFACE_CreateRoomInBar(NextRoom.Name);
		INTERFACE_RefreshOccupantsNumber(NextRoom.Name);
		ROOM_FocusRoom(NextRoom.Name)
	}
	else
	{
		//Set focus to general room
		ROOM_FocusRoom(MainData.RoomDefault);
	}

	return RoomName;
}

//Sort all user in all rooms by nick name
function ROOM_SortUsersByNick()
{
	var Room, RoomName;
	var i, j;
	var UserName, Status, Rating, Type;

	// Get all rooms
	for(j=0; j<MainData.RoomList.length; j++)
	{
		Room = MainData.RoomList[j];
		if(Room == null)
		{
			return false;
		}
		
		// Test the current order mode (order == sort)
		// If ordered into ascending order, change to descending order
		if (Room.OrderBy == "0")
		{
			Room.OrderBy = "1";
		}
		// other modes, change to ascending order
		else
		{
			Room.OrderBy = "0";
		}
		
		RoomName = Room.Name;
		// Sort user list by nick name in data struct
		MainData.SortUserByNickInRoom(RoomName);

		// Show new user list sorted
		for(i=0; i<Room.UserList.length; i++)
		{
			UserName = Room.UserList[i].Username;
			Status = Room.UserList[i].Status;
			Type = Room.UserList[i].Type;

			// Get rating
			switch(MainData.RoomCurrentRating)
			{
				case "blitz":
					Rating = Room.UserList[i].Rating.Blitz;
					break;
				case "lightning":
					Rating = Room.UserList[i].Rating.Lightning;
					break;
				case "standard":
					Rating = Room.UserList[i].Rating.Standard;
					break;
			}

			Room.Room.userList.removeUser(UserName);
			Room.Room.userList.addUser(UserName, Status, Rating, Type);
		}
		// TODO - Fix user menu position in FF3
		// Proposital hide
		if (MainData.Browser == 2)
		{
			Room.Room.hideUserList();
		}
	}

	return true;
}

//Sort all user in all rooms by rating name
function ROOM_SortUsersByRating(Category)
{
	var Room, RoomName;
	var i, j;
	var UserName, Status, Rating, Type;

	MainData.RoomCurrentRating = Category;	

	// Get all rooms
	for(j=0; j<MainData.RoomList.length; j++)
	{
		Room = MainData.RoomList[j];
		if(Room == null)
		{
			return false;
		}
		
		// If ordered into ascending order, change to descending order
		Room.OrderBy = "2";
		
		RoomName = Room.Name;
		// Sort user list by nick name in data struct
		MainData.SortUserByRatingInRoom(RoomName);

		// Show new user list sorted
		for(i=0; i<Room.UserList.length; i++)
		{
			UserName = Room.UserList[i].Username;
			Status = Room.UserList[i].Status;
			Type = Room.UserList[i].Type;

			// Get rating
			switch(MainData.RoomCurrentRating)
			{
				case "blitz":
					Rating = Room.UserList[i].Rating.Blitz;
					break;
				case "lightning":
					Rating = Room.UserList[i].Rating.Lightning;
					break;
				case "standard":
					Rating = Room.UserList[i].Rating.Standard;
					break;
				case "untimed":
					Rating = Room.UserList[i].Rating.Untimed;
					break;
			}

			Room.Room.userList.removeUser(UserName);
			Room.Room.userList.addUser(UserName, Status, Rating, Type);
		}

		// TODO - Fix user menu position in FF3
		// Proposital hide
		if (MainData.Browser == 2)
		{
			Room.Room.hideUserList();
		}
	}

	return true;
}

function ROOM_ShowHideUserList(RoomName)
{
	var Room = MainData.GetRoom(RoomName);

	if(Room == null)
	{
		return false;
	}

	if(Room.Room.userListVisibility == false)
	{
		Room.Room.showUserList();
	}
	else
	{
		Room.Room.hideUserList();
	}
	return true;
}
