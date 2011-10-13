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
*/


/**
 * @file	admin.js
 * @brief	Functions to parse messages to admin and send admin messages to server
 *
 * See interface functions to admin (script/interface/admin.js)
 */

/**
 * @brief	Parser admin messages for rooms
 *
* Parser the admin messages with type 'set'
* 
* @param 	XML 	Xml with the messages
* @return 	Buffer with other XMPP to send
* @author 	Ulysses
*/
function ADMIN_HandleRoomAdmin(XML)
{
	var Buffer = "";
	
	return Buffer;
}

/**
 * @brief	Parser administrative messages for admin
 *
* Parser messages with administrative commands
* 
* @param 	XML 	Xml with the messages
* @return 	Buffer with other XMPP to send
* @author 	Rubens Suguimoto
*/
function ADMIN_HandleAdmin(XML)
{
	var Id = XML.getAttribute("id");
	var Buffer = "";

	switch(Id)
	{
		// Show banned user's list
		case MainData.Const.IQ_ID_GetBanList:
			ADMIN_HandleBanList(XML);
			break;

		// Show a window alert with banned user confirmation 
		/*
		case MainData.Const.IQ_ID_BanUser:
			ADMIN_Notification(XML)
			break;
		*/

		// Show a window alert with unbanned user confirmation 
		case MainData.Const.IQ_ID_UnbanUser:
			ADMIN_Notification(XML)
			break;

		// Show a window alert with kiked user confirmation 
		/*
		case MainData.Const.IQ_ID_KickUser:
			ADMIN_Notification(XML)
			break;
		*/
	}
	
	return Buffer;
}

/**
 * @brief	Parser admin type.
 *
* Parser admin type to show menu with administrative tools.
* 
* @param 	XML 	Xml with the messages
* @return 	Buffer with other XMPP to send
* @author 	Rubens Suguimoto
*/
function ADMIN_HandleInfo(XML)
{
	var Items = XML.getElementsByTagName("type");
	var Username;
	var i =0;
	var Type;
	var Buffer = "";

	// Find user
	while((i<Items.length) &&(Items[i].getAttribute("jid").split("@")[0])!= MainData.Username)
	{
		i++;
	}
	
	if( i != Items.length)
	{
		Type = Items[i].getAttribute("type");
		if(Type == "admin")
		{
			INTERFACE_ShowAdminIcon();
		}
	}

	return Buffer;
}

/**
 * @brief	Parser and show notification of some action done by admin.
 *
* Parse admin message with result of some action done and show in interface.
* 
* @param 	XML 	Xml with the messages
* @return 	Empty string;
* @author 	Rubens Suguimoto
*/
function ADMIN_Notification(XML)
{
	var Node = XML.firstChild;

	switch(Node.tagName)
	{
		case "kick":
			WINDOW_Alert("Kick",UTILS_GetText("admin_kick_ok"));
			break;
		case "ban":
			WINDOW_Alert("Ban",UTILS_GetText("admin_ban_ok"));
			break;
		case "unban":
			WINDOW_Alert("Unban",UTILS_GetText("admin_unban_ok"));
			break;
	}

	return "";
}

/**
 * @brief	Parser and show notification of some action done by admin to normal user.
 *
* Parse admin message with result of some action done and show in interface to normal user.
* 
* @param 	XML 	Xml with notification message
* @return 	Buffer with other XMPP to send;
* @author 	Rubens Suguimoto
*/
function ADMIN_HandleUserNotification(XML)
{	
	var Node = XML.firstChild;
	var ReasonTag = XML.getElementsByTagName("reason")[0];
	var Reason = UTILS_GetNodeText(ReasonTag);
	var Buffer= "";

	switch(Node.tagName)
	{
		case "ban":
			alert(UTILS_GetText("admin_user_ban")+Reason);
			break;
		case "kick":
			alert(UTILS_GetText("admin_user_kick")+Reason);
			break;
	}	
	return Buffer;
}

/**
 * @brief	Parse and show banned user list to admin.
 *
 * Parse admin message with banned users and show as list in interface to admin.
* 
* @param 	XML 	Xml with banned users
* @return 	Buffer with other XMPP to send;
* @author 	Rubens Suguimoto
*/
function ADMIN_HandleBanList(XML)
{	
	var Users = XML.getElementsByTagName("user");
	var i;
	var Username;
	var Buffer = "";

	//Get all users in the message and show;
	for(i=0;i<Users.length;i++)
	{
		Username = Users[i].getAttribute("jid").split("@")[0];
		INTERFACE_AddBannedUser(Username);
	}

	return Buffer;
}

/************************
 * ADMIN - MESSAGES
 * **********************/
/**
 * @brief	Create and send message to kick some user and reason;
 *
 * Create and send message to kick some user and reason to kick him/her;
* 
* @param	Username	User name used by user;
* @param	Reason		Reason to kick user;
* @return 	Empty string;
* @author 	Rubens Suguimoto
*/
function ADMIN_KickUser(Username, Reason)
{
	CONNECTION_SendJabber(MESSAGE_KickUser(Username,Reason));

	return "";
}

/**
 * @brief	Create and send message to ban some user and reason;
 *
 * Create and send message to ban some user and reason to ban him/her;
* 
* @param	Username	User name used by user;
* @param	Reason		Reason to ban user;
* @return 	Empty string;
* @author 	Rubens Suguimoto
*/
function ADMIN_BanUser(Username, Reason)
{
	CONNECTION_SendJabber(MESSAGE_BanUser(Username, Reason));

	return "";
}

/**
 * @brief	Create and send message to unban some user and reason;
 *
 * Create and send message to unban some user and reason to unban him/her;
* 
* @param	Username	User name used by user;
* @param	Reason		Reason to unban user;
* @return 	Empty string;
* @author 	Rubens Suguimoto
*/
function ADMIN_UnbanUser(Username, Reason)
{
	CONNECTION_SendJabber(MESSAGE_UnbanUser(Username, Reason));

	return "";
}

/**
 * @brief	Create and send message to get banned users's list;
 *
 * Create and send message to get all banned users from server; 
* 
* @return 	Empty string;
* @author 	Rubens Suguimoto
*/
function ADMIN_GetBanList()
{
	CONNECTION_SendJabber(MESSAGE_GetBanList());
	
	return "";
}
