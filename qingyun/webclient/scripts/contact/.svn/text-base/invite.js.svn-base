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
* Functions to add a user in your contact list
*/


/**
* Send a invite message to 'Username'
*/
function CONTACT_InviteUser(Username)
{
	var XML;

	// Create a invite message
	XML = MESSAGE_Invite(Username);

	// Insert user in structure
	MainData.AddUser(Username, "offline", "", "default");

	// Insert user in interface
	MainData.Contact.addUser("default",Username, "offline");

	// Send it to jabber
	CONNECTION_SendJabber(XML);

	// Sort userlist
	MainData.SortUserByNick();

	return "";
}

/**
* Remove user form your list
*/
function CONTACT_RemoveUser(Username)
{
	// Remove user from data structure
	MainData.DelUser(Username);

	// Remove user from interface
	MainData.Contact.removeUser(Username);
	//INTERFACE_RemoveContact(Username);

	return true;
}

function CONTACT_SendRemoveUser(Username)
{
	var XML;

	// Create a remove message
	XML = MESSAGE_RemoveContact(Username);
	
	// Send it to jabber
	CONNECTION_SendJabber(XML);
}


/**
* User has received a subscribe message
*/
function CONTACT_ReceiveSubscribe(Username)
{
	var XML = "", i;
	var Title, Text, Button1, Button2;

	// Search user in sctructure
	i = MainData.FindUser(Username);

	// If user is in your list
	if (i != null)
	{
		// Try to add a removed user
		if (MainData.UserList[i].Subs == "none")
		{
			MainData.SetSubs(Username, "from");
			
			// Send a subscribe and a subscribed to user
			XML += MESSAGE_InviteAccept(Username);
			XML += MESSAGE_Invite(Username);

			return XML;
		}
		// Last confirmation
		else if (MainData.UserList[i].Subs != "from")
		{
			// Send a subscribed to user
			XML = MESSAGE_InviteAccept(Username);

			return XML;
		}
		else
			return "";
	}
	// show window to confirm user invitation
	else 
	{
		Title = UTILS_GetText("contact_invite");
		Text = UTILS_GetText("contact_invite_text").replace(/%s/, "<strong>"+UTILS_Capitalize(Username)+"</strong>");
		Button1 = new Object();
		Button1.Name = UTILS_GetText("window_accept");
		Button1.Func = function () {
			var XML = "";

			// See data/data.js
			//MainData.AddUser(Username, "offline", "from");

			// See contact/contact.js
			CONTACT_InsertUser(Username, "offline","from","default");

			// Send a subscribe and a subscribed to user
			XML += MESSAGE_InviteAccept(Username);
			XML += MESSAGE_Invite(Username);

			CONNECTION_SendJabber(XML);
		}

		Button2 = new Object();
		Button2.Name = UTILS_GetText("window_cancel");
		Button2.Func = function () {
			// Send a deny to user
			CONNECTION_SendJabber(MESSAGE_InviteDeny(Username));
		}

		WINDOW_Confirm(Title, Text, Button1, Button2);
	}
	return "";
}

/**
* User has received a subscribed message
*/
function CONTACT_ReceiveSubscribed(Username)
{
	// Setting user subscription state to 'both'
	if (MainData.SetSubs(Username, "both"))
	{
		//INTERFACE_AddContact(Username, "available");
		//MainData.Contact.addUser(Username, "available");

		// See contact/contact.js
		CONTACT_InsertUser(Username, "online","from","default");

		// Ask user type and rating
		return MESSAGE_Info(Username);
	}
	return "";
}

/**
* User has received a unsubscribed message
*/
function CONTACT_ReceiveUnsubscribed(Username)
{
	var XML, i;

	i = MainData.FindUser(Username);

	// If user is not in your list, something wrong! =D
	if (i == null)
	{
		return "";
	}

	// User has removed you, do nothing
	if (MainData.UserList[i].Subs == "both")
	{
		// Changing subscription state to none
		MainData.SetSubs(Username, "none");

		// Set user as offline
		CONTACT_SetUserStatus(Username, "offline");
		return "";
	}

	// Deny user invite
	if (MainData.DelUser(Username))
	{
		// Create a remove message
		XML = MESSAGE_RemoveContact(Username);
		return XML;
	}
	return "";
}
