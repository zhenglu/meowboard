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
* Handle contacts and user status
*/

/**
 * @brief	Show contact and contact online Div
 *
 * Create contact list object and contact online list object. Hide contact list and show online list.
*
* @author Rubens Suguimoto
*/
function CONTACT_StartContact()
{
	MainData.Contact = new ContactObj();
	MainData.ContactOnline = new ContactOnlineObj();

	//Hide user contact list and show online list
	MainData.Contact.hide();
	MainData.ContactOnline.show();
}


/************************************
*** FUNCTIONS - CONTACT ONLINE LIST
*************************************/
/**
 * @brief	Handle presence to online list
 *
* Handle all presence from online list (General room);
*
* @param        XML The xml come from server with tag presence
*
* @author       Ulysses Bonfim
*/
function CONTACT_HandleOnlinePresence(XML)
{
	var From, User, Type, Show, Status;
	var Buffer = "";
	var ContactOnline = MainData.ContactOnline;
	var RoomName;

	// Get Attributes from XML
	Show = XML.getElementsByTagName("show");
	From = XML.getAttribute('from');
	Type = XML.getAttribute('type');
	RoomName = From.replace(/@.*/, "");
	User = From.replace(/.*\//, "");


	// Check if the type is error
	if (Type == "error")
		return "";


	if(RoomName != MainData.RoomDefault)
	{
		return "";
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



	if (Type == "unavailable")
	{	
		// Remove from contact online list in interface
		ContactOnline.userList.removeUser(User);
	}
	else
	{
		// Add user in contact list
		CONTACT_AddUserOnlineList(User, Status);	
	}

	return Buffer;
}

/**
 * @brief 	Add user in the online list
 * 
 * Add a user with status in online list.
 *
 * @param 	User 	Username
 * @param	Status	User's status
 *
 * @author	Rubens Suguimoto
 */
function CONTACT_AddUserOnlineList(User, Status)
{
	var RatingTmp;
	var Rating, Type;
	var UserPos;
	var ContactOnline = MainData.ContactOnline;

	// Find user in online user struct == find in General user list
	UserPos = MainData.FindUserInRoom(MainData.RoomDefault, User);

	// Get user Type and Current rating
	Type = MainData.GetType(User);
	RatingTmp = MainData.GetRating(User);
	if(RatingTmp != null)
	{
		switch(MainData.CurrentRating)
		{
			case "blitz":
				Rating = RatingTmp.Blitz;
				break;
			case "lightning":
				Rating = RatingTmp.Lightning;
				break;
			case "standard":
				Rating = RatingTmp.Standard;
				break;	
		}
	}
	else
	{
		Rating = null;
	}
	
	// if user is not founded, add to online list	
	if( ContactOnline.userList.findUser(User) == null)
	{
		ContactOnline.userList.addUser(User, Status, Rating, Type);
	}
	// else, update status
	else
	{
		ContactOnline.userList.updateUser(User, Status, Rating, Type);
	}

	// Hide loading box
	ContactOnline.hideLoading();
}

/**
 * @brief 	Sort online list by nick name
 * 
 * Sort on line user list by nick name. Sort in MainData and for each user in MainData online user, remove and insert it.
 *
 * @author	Rubens Suguimoto
 */
function CONTACT_OnlineSortUserByNick()
{
	var Room, RoomName;
	var i, j;
	var UserName, Status, Rating, Type;
	var ContactOnline = MainData.ContactOnline;
	
	// General room
	Room = MainData.RoomList[0];

	if(Room == null)
	{
		return false;
	}
		
	// Test the current order mode 
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
	
	// Sort user list by nick name in data struct
	MainData.SortUserByNickInRoom(MainData.RoomDefault);

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
	
		//Show in contact online list
		ContactOnline.userList.removeUser(UserName);
		ContactOnline.userList.addUser(UserName, Status, Rating, Type);
	}

	return true;
}

/**
 * @brief 	Sort online list by rating category
 * 
 * Sort on line user list by rating. Sort in MainData and for each user in MainData online user, remove and insert it.
 *
 * @param	Category	Chess game category
 *
 * @author	Rubens Suguimoto
 */
function CONTACT_OnlineSortUserByRating(Category)
{
	var Room, RoomName;
	var i, j;
	var UserName, Status, Rating, Type;
	var ContactOnline = MainData.ContactOnline;

	MainData.RoomCurrentRating = Category;	

	Room = MainData.RoomList[0];
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

		//Show in contact online list
		ContactOnline.userList.removeUser(UserName);
		ContactOnline.userList.addUser(UserName, Status, Rating, Type);
	}

	return true;
}

/************************************
*** FUNCTIONS - CONTACT LIST
*************************************/

/**
 * @brief	Handle user contact list
 *
 * Handle user list received from jabber with yours contacts.
*
* @return string
*
* @author Ulysses Bonfim
*/
function CONTACT_HandleUserList(XML)
{
	var Users, Jid, Subs, i, Pending = "";
	var Group;

	Users = XML.getElementsByTagName("item");

	for (i=0; i < Users.length; i++)
	{
		Jid = Users[i].getAttribute("jid").match(/[^@]+/)[0];
		Subs = Users[i].getAttribute ("subscription"); 
		
		if(Users[i].getElementsByTagName("group")[0] != null)
		{
			Group = UTILS_GetNodeText(Users[i].getElementsByTagName("group")[0]);
		}
		else
		{
			Group = "default";
		}

		// Check subscription state of users
		switch (Subs)
		{
			// Store xml pending messages
			case("to"):
				// If there's a pending invite send a accept
				Pending += MESSAGE_InviteAccept(Jid);
				CONTACT_InsertUser(Jid, "offline", "both", Group);
				break;

			// Insert users and group in data structure
			case("both"):
				if (MainData.Username != Jid)
				{
					CONTACT_InsertUser(Jid, "offline", Subs, Group);	
				}
				break;

			// Store the contact on structure with subs "none"
			case("none"):
				// If there's a pending invite send a accept
				CONTACT_InsertUser(Jid, "offline", Subs, Group);
				break;
			
			// Remove user from contact list
			case("remove"):
				CONTACT_RemoveUser(Jid);
				break;

			// Do nothing =)
			case("from"):
				break;
		}
	}

	CONNECTION_SendJabber(MESSAGE_UserListInfo());

	// two eggs
	// a cup of milk 
	// a spoon of sugar
	// a 'tea spoon' of yeast
	// two cups of flour
	return Pending;
}


/**
 * @brief	Handle user contact receive subscribe
 *
 * Handle user list received from jabber with yours contacts.
*
* @return string
*
* @author Ulysses Bonfim
*/
function CONTACT_HandleSetSubscribe(XML)
{
	var Users, Jid, Subs, i, Pending = "";
	var Group;

	Users = XML.getElementsByTagName("item");

	for (i=0; i < Users.length; i++)
	{
		Jid = Users[i].getAttribute("jid").match(/[^@]+/)[0];
		Subs = Users[i].getAttribute ("subscription"); 
		
		if(Users[i].getElementsByTagName("group")[0] != null)
		{
			Group = UTILS_GetNodeText(Users[i].getElementsByTagName("group")[0]);
		}
		else
		{
			Group = "default";
		}

		// Check subscription state of users
		switch (Subs)
		{
			// Remove user from contact list
			case("remove"):
				CONTACT_RemoveUser(Jid);
				break;
		}
	}

	// two eggs
	// a cup of milk 
	// a spoon of sugar
	// a 'tea spoon' of yeast
	// two cups of flour
	return Pending;
}



/**
 * @brief	Handle a user presence
 *
 * Handle user presence for accept or decline invite to contact list and user status.
*
* @return string
*
* @author Ulysses Bonfim
*/

function CONTACT_HandleUserPresence(XML)
{
	var Jid, Type, Show, NewStatus;
	var Buffer = "";

	// Get Jid
	Jid = XML.getAttribute('from').replace(/@.*/,"");

	Type = XML.getAttribute('type');

	/* HANDLE INVITE TO CONTACT LIST */
	
	// User is offline
	if (Type == "unavailable")
	{
		CONTACT_SetUserStatus(Jid, "offline");
		return "";
	}
	// Receive a invite message
	else if (Type == "subscribe")
	{
		Buffer += CONTACT_ReceiveSubscribe(Jid);
		return Buffer;
	}
	// User is allowed
	else if (Type == "subscribed")
	{
		Buffer += CONTACT_ReceiveSubscribed(Jid);
		return Buffer;
	}
	// User is not allowed
	else if (Type == "unsubscribed")
	{
		Buffer += CONTACT_ReceiveUnsubscribed(Jid);
		return Buffer;
	}

	/* HANDLE USER STATUS */

	// Searching for the user status
	Show = XML.getElementsByTagName('show');
	if (Show.length > 0)
	{
		// Get status name
		NewStatus = UTILS_GetNodeText(Show[0]);

		if ((NewStatus == "busy") || (NewStatus == "away") || (NewStatus == "unavailable") || (NewStatus == "playing"))
		{
			CONTACT_SetUserStatus(Jid, NewStatus);
		}
	}
	// If tag 'show' doesnt exists, status online
	else
	{
		CONTACT_SetUserStatus(Jid, "available");
	}
	
	return Buffer;
}

/**
 * @brief	Add user in contact list
 *
 * Add user in MainData contact list, sort and show this user in interface.
*
* @param 	User	User's name
* @param	Status	User's status
* @param	Subs	User's subscribe status
* @param	Group	User's group in contact list
* @author Pedro Rocha
*/
function CONTACT_InsertUser(User, Status, Subs, Group)
{
	if(MainData.FindUser(User) == null)
	{
		//Add in data struct
		MainData.AddUser(User, Status, Subs, Group)
		MainData.SortUserByNick();

		//Show in interface
		if(MainData.Contact != null)
		{
			MainData.Contact.addUser(Group, User, Status);
		}
	}
}


/**
 * @brief	Show user menu options
 *
 * Show user menu to send private message, match, view profile, etc.
*
* @param 	Obj 	Object with menu options and functions
* @param	Username	User's name
*
* @author Pedro Rocha
*/
function CONTACT_ShowUserMenu(Obj, Username)
{
	var Func, Options = new Array();
	var i = 0, Hide = 0;
	var Rating;
	var Button1 = new Object(), Button2 = new Object();

	Func = function () {
		Hide += 1;
		
		if (Hide == 2)
		{
			UTILS_RemoveListener(document, "click", Func, false);

			// Remove menu from screen
			INTERFACE_HideUserMenu();
		}
	};

	/**
	* Setting options
	*/

	// If isn't your name
	if (MainData.Username != Username)
	{
		// Send message
		Options[i] = new Object();
		if (MainData.GetStatus(Username) != "offline")
		{
			Options[i].Name = UTILS_GetText("usermenu_send_message");
		}
		// Send a offline message (scrap)
		else 
		{
			Options[i].Name = UTILS_GetText("usermenu_send_offlinemessage");
		}
		Options[i].Func = function () {
			CHAT_OpenChat(Username);
		}
		i += 1;

		// Match user
		Options[i] = new Object();
		Options[i].Name = UTILS_GetText("usermenu_match");

		// Request for match is possible only if the status
		// of opponent is (avaiable, away, busy) and the user
		// isn't playing a game
		if ( ((MainData.GetStatus(Username) != "offline") && (MainData.GetStatus(Username) != "playing") && (MainData.GetStatus(Username) != "unavailable")) && (MainData.Status != "playing"))
		{
			Options[i].Func = function () {
				Rating = MainData.GetUserRatingInRoom(MainData.RoomDefault,Username);
				WINDOW_Challenge(Username, Rating);
			};
		}
		else
		{
			Options[i].Func = null;
		}
		i += 1;

		// Add or remove contact
		if (MainData.IsContact(Username))
		{
			Options[i] = new Object();
			Options[i].Name = UTILS_GetText("usermenu_remove_contact");
			Options[i].Func = function () { 
				Button1.Name = UTILS_GetText("window_yes");
				Button1.Func = function () {
					CONTACT_SendRemoveUser(Username);
				}
				Button2.Name = UTILS_GetText("window_cancel");
				Button2.Func = null;
				WINDOW_Confirm (UTILS_GetText("contact_remove_title"), UTILS_GetText("contact_remove_text").replace("%s", "<strong>"+Username+"</strong>"), Button1, Button2);
			};
			i += 1;
		}
		else
		{
			Options[i] = new Object();
			Options[i].Name = UTILS_GetText("usermenu_add_contact");
			Options[i].Func = function () { 
				CONTACT_InviteUser(Username);
			};
			i += 1;
		}

		// Administrative functions
		if (MainData.Type == "admin")
		{
			// Disconnet user
			Options[i] = new Object();
			Options[i].Name = UTILS_GetText("usermenu_disconnect_user");
			Options[i].Func = function () {
				//ADMIN_KickUser(Username)
				WINDOW_KickUser(Username);
			};
			i += 1;

			// Ban user
			Options[i] = new Object();
			Options[i].Name = UTILS_GetText("usermenu_ban");
			Options[i].Func = function () {
				//ADMIN_BanUser(Username)
				WINDOW_BanUser(Username);
			};
			i += 1;

			// Mute user
			Options[i] = new Object();
			Options[i].Name = UTILS_GetText("usermenu_mute");
			Options[i].Func = function () {
			};
			i += 1;

			// Abusive
			Options[i] = new Object();
			Options[i].Name = UTILS_GetText("usermenu_abusive");
			Options[i].Func = function () {
			};
			i += 1;
		}

	}

	// View user's profile
	Options[i] = new Object();
	Options[i].Name = UTILS_GetText("usermenu_view_profile");
	Options[i].Func = function () {
		PROFILE_StartProfile(Username);
	};
	i += 1;


	// Show menu in user's screen
	INTERFACE_ShowUserMenu(Obj, Options);
	
	UTILS_AddListener(document, "click", Func, false);
}

/**
 * @brief	Load contact list in interface
 *
 * Load user contact list in interface.
*
* @author Rubens Suguimoto
*/
function CONTACT_LoadUserContactList()
{
	var i;
	var User;

	// Loading user list
	for (i=0; i < MainData.UserList.length; i++)
	{
		User = MainData.UserList[i];

		// Find user group, if not exists create group
		if(MainData.Contact.findGroup(User.Group) == null)
		{
			MainData.Contact.addGroup(User.Group);
		}

		MainData.Contact.addUser(User.Group, User.Username, User.Status, User.Rating.Blitz, User.Type);

	}

	MainData.Contact.hideLoading();

}


/************************************
*** FUNCTIONS - SORT CONTACT LIST
*************************************/
/**
 * @brief 	Sort user contact list by nick name
 * 
 * Sort user online list by nick name. Sort in MainData and for each user in MainData online user, remove and insert it.
 *
 * @author	Rubens Suguimoto and Danilo Yorinori
 */
function CONTACT_SortUsersByNick()
{
	var i;
	var User;
	var Rating;

	// Test the current order mode
	// If ordered into ascending order, change to descending order
	if (MainData.OrderBy == "0")
	{
		MainData.OrderBy = "1";
	}
	// other modes, change to ascending order
	else
	{
		MainData.OrderBy = "0";
	}
	
	MainData.SortUserByNick();

	for(i=0; i<MainData.UserList.length; i++)
	{
		User = MainData.UserList[i];
		
		switch(MainData.CurrentRating)
		{

			case "blitz":
				Rating = User.Rating.Blitz;
				break;
			case "lightning":
				Rating = User.Rating.Lightning;
				break;
			case "standard":
				Rating = User.Rating.Standard;
				break;	
		}

		MainData.Contact.removeUser(User.Username);
		MainData.Contact.addUser(User.Group, User.Username, User.Status, Rating, User.Type);
	}
}

/**
 * @brief 	Sort user contact list by rating category
 * 
 * Sort user contact list by rating. Sort in MainData and for each user in MainData online user, remove and insert it.
 *
 * @param	Category	Chess game category
 *
 * @author	Rubens Suguimoto and Danilo Yorinori
 */
function CONTACT_SortUsersByRating(Category)
{
	var i;
	var User;
	var Rating;

	// Test the current order mode
	// If ordered into ascending order, change to descending order
	/*
	if (MainData.OrderBy == "0")
	{
		MainData.OrderBy = "1";
	}
	// other modes, change to ascending order
	else
	{
		MainData.OrderBy = "0";
	}
	*/
	MainData.CurrentRating = Category;

	MainData.OrderBy = "2";

	MainData.SortUserByRating();

	for(i=0; i<MainData.UserList.length; i++)
	{
		User = MainData.UserList[i];
		
		switch(MainData.CurrentRating)
		{

			case "blitz":
				Rating = User.Rating.Blitz;
				break;
			case "lightning":
				Rating = User.Rating.Lightning;
				break;
			case "standard":
				Rating = User.Rating.Standard;
				break;	
			case "untimed":
				Rating = User.Rating.Untimed;
				break;	
		}

		MainData.Contact.removeUser(User.Username);
		MainData.Contact.addUser(User.Group, User.Username, User.Status, Rating, User.Type);
	}
}
