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
 * @file	chat.js
 * @brief	Functions to parse chat messages.
 *
 * See interface chat (scripts/interface/chat.js).
*/


/**
 * @brief 	Parse and show ordinary users messages
 *
 * @param	XML	XML with message
 * @return	Buffer with other XMPP to send
 * @author	Pedro Rocha
*/
function CHAT_HandleMessage(XML)
{
	var From, Message, Body;
	var Buffer = "";

	// Get the sender name
	From = XML.getAttribute('from').replace(/@.*/,"");

	Body = XML.getElementsByTagName("body");

	// If there is a body
	if (Body.length > 0)
	{
		// Get the message
		Message = UTILS_GetNodeText(Body[0]);
	}

	// Show the message on interface
	// UTILS_ConvertChatString -> Replace < and >
	CHAT_ReceiveMessage(From, UTILS_ConvertChatString(Message));

	return Buffer;
}

/**
 * @brief 	Parse and show user chat presence
 *
 * @param	XML	XML with presence
 * @return	Buffer with other XMPP to send
 * @author	Rubens Suguimoto
*/
function CHAT_HandlePresence(XML)
{
	var From;
	var Status;
	var Username;
	var Type = XML.getAttribute("type");
	var Buffer = "";

	if(Type == "error")
	{
		return "";
	}

	
	From = XML.getAttribute("from");
	Username = From.split("/")[1];
	
	if(Type == "unavailable")
	{
		Status = "offline";
	}
	else
	{
		Status = "online";
	}

	CHAT_ChangeUserChatStatus(Username, Status);

	return Buffer;
}

/**
 * @brief 	Show announcement users messages
 *
 * @param	XML	XML with announce message
 * @return	Buffer with other XMPP to send
 * @author	Pedro Rocha
*/
function CHAT_HandleAnnounceMessage(XML)
{
	var From, Subject, Message, Body;
	var Buffer = "";

	// Get the sender name
	From = XML.getAttribute('from').replace(/@.*/,"");

	// Show Annouce message only if sender is host server
	if (From != MainData.Host)
		return "";

	// Announce's subject
	Subject = UTILS_GetNodeText(XML.getElementsByTagName("subject")[0]);

	Body = XML.getElementsByTagName("body");

	// If there is a body
	if (Body.length > 0)
	{
		// Get the message
		Message = UTILS_GetNodeText(Body[0]);
	}

	// Show the message on interface
	WINDOW_Alert(Subject,Message);

	return Buffer;
}

/**
 * @brief 	Open a chat with some user
 *
 * @param	Username	Name used by user
 * @return	Empty string
 * @author	Pedro Rocha
*/

function CHAT_OpenChat(Username)
{
	var Title, Msg;
	var Status;
	var ChatObject = null;
	var Position, ChatPos;

	ChatPos = MainData.FindChat(Username);
	if (ChatPos != null)
	{
		MainData.ChatList[ChatPos].Chat.chatTitle.className = "title_selec";
		return null;
	}

	if (MainData.FindUserInRoom(MainData.RoomDefault, Username) == null)
	{
		Status = "offline";
	}
	else
	{
		Status = "online";
	}

	Position = MainData.ChatList.length;

	if(Position < MainData.MaxChats)
	{
		// Create a chat object
		ChatObject = new ChatObj(Username, Position);

		if(Status != "offline")
		{
			ChatObject.setTitle(Username)
		}
		else
		{
			ChatObject.setTitle(Username+" (offline)");
		}

		// Add chat in main data structure
		MainData.AddChat(Username, ChatObject)

		// Show window maximized
		ChatObject.maximize();
		ChatObject.show();

	}
	else //Max chats reached
	{
		// Show error message to user
		Title = UTILS_GetText("chat_warning");
		Msg = UTILS_GetText("chat_max_exceeded");
		Msg = Msg.replace(/%i/, MainData.MaxChats);
		WINDOW_Alert(Title, Msg);
	}

	return ChatObject;
}

/**
 * @brief 	Close a chat with user
 *
 * @param	Username	Name used by user
 * @return	Empty string
 * @author	Pedro Rocha
*/
function CHAT_CloseChat(Username)
{
	var ChatObj;

	if(MainData.FindChat(Username) == null)
	{
		return "";
	}

	ChatObj = MainData.GetChat(Username);
	
	// Close chat window
	ChatObj.Chat.close();

	// Remove from main data sctruct
	MainData.RemoveChat(Username);

	return "";
}


/**
 * @brief 	Change  visibility of chat window
 *
 * @param	Username	Name used by user
 * @param	Obj1		Chat item
 * @param	Obj2		Chat item inner content
 * @param	State		Chat status (show/hidden)
 * @return	Empty string
 * @author	Pedro Rocha
*/
function CHAT_ChangeChatState(Username)
{
	var i = MainData.FindChat(Username);
	var ChatObj;

	if (i == null)
	{
		return "";
	}

	ChatObj = MainData.GetChat(Username);

	// Changing the visibility of chat window
	if (ChatObj.Chat.visible == false)
	{
		ChatObj.Chat.maximize();
		ChatObj.Chat.visible == true;
		ChatObj.Chat.minmax.className = "minimize";
		if(MainData.Browser == 0)
		{
			ChatObj.Chat.minmax.src = "./images/ie/minimize_chat.gif";
		}
		else
		{
			ChatObj.Chat.minmax.src = "./images/minimize_chat.png";
		}
	}
	else
	{
		ChatObj.Chat.minimize();
		ChatObj.Chat.visible == false;
		ChatObj.Chat.minmax.className = "maximize";
		if(MainData.Browser == 0)
		{
			ChatObj.Chat.minmax.src = "./images/ie/maximize_chat.gif";
		}
		else
		{
			ChatObj.Chat.minmax.src = "./images/maximize_chat.png";
		}
	}
	
	return "";
}


/**
 * @brief 	Send a chat message
 *
 * @param	Username	Name used by user
 * @param	Message		Message to send
 * @return	Empty string
 * @author	Pedro Rocha
*/
function CHAT_SendMessage(Username, Message)
{
	var ChatObj;
	//Replace < and  >
	var Msg = UTILS_ConvertChatString(Message)
	var XML = MESSAGE_Chat(Username, Msg);

	CONNECTION_SendJabber(XML);

	// Find and get Chat object
	if(MainData.FindChat(Username) != null)
	{
		ChatObj = MainData.GetChat(Username);

		// Show message in chat list
		//INTERFACE_ShowChatMessage(Username, Msg, true);
		ChatObj.Chat.addMessage(MainData.Username, UTILS_ConvertChatString(Message));
	}

	return "";
}

/**
 * @brief 	Show a message received from another user
 *
 * @param	Username	Name used by user
 * @param	Message		Message received
 * @return	Empty string
 * @author	Pedro Rocha
*/
function CHAT_ReceiveMessage(Username, Message)
{
	var ChatPos = MainData.FindChat(Username);
	var ChatObj;

	// Do not exists a opened chat session
	if (ChatPos == null)
	{
		ChatObj = CHAT_OpenChat(Username);
	}
	else
	{
		ChatObj = MainData.GetChat(Username);
		//Quick fix
		ChatObj = ChatObj.Chat;
	}

	// Show message in chat list
	ChatObj.addMessage(Username, Message);

	// Set focus to chat window
	ChatObj.focus();

	return "";
}

function CHAT_ErrorMessageLength(Username)
{
	var ChatPos = MainData.FindChat(Username);
	var ChatObj;
	var Message;
	var Limit = MainData.MaxChatChar;

	if(ChatPos != null)
	{
		ChatObj = MainData.GetChat(Username);
		//Quick fix
		ChatObj = ChatObj.Chat;
	}
	else
	{
		return "";
	}

	Message = UTILS_GetText("room_error_message_length");
	Message = Message.replace("%s",Limit);

	ChatObj.addMessageError(Message);

	return Message;
}

function CHAT_ChangeUserChatStatus(Username, Status)
{
	var ChatObj;

	if(MainData.FindChat(Username) != null)
	{
		ChatObj = MainData.GetChat(Username);
		
		if(Status == "offline")
		{
			ChatObj.Chat.setTitle(Username + " (offline)");
		}
		else
		{
			ChatObj.Chat.setTitle(Username);
		}
	}

	return "";
}

function CHAT_BlurChat(Username)
{
	var ChatObj;

	if(MainData.FindChat(Username) != null)
	{
		ChatObj = MainData.GetChat(Username);
	
		ChatObj.Chat.blur();
	}

	return "";

}
