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
* Interface function for chat
*/ 

function ChatObj(Username, Position)
{
	var Tmp = INTERFACE_CreateChat(Username);
	// Attributes
	this.chatWindow = Tmp.ChatWindow;
	this.chatInner = Tmp.ChatInner;
	this.chatList = Tmp.ChatList;
	this.chatTitle = Tmp.ChatTitle;
	this.chatTitleText = Tmp.ChatTitleSpan;
	this.chatInput = Tmp.Input;
	this.minmax = Tmp.MinMax;

	this.username = Username;
	this.position = Position;

	this.visible = true;

	// Methods
	this.show = INTERFACE_ShowChat;
	this.hide = INTERFACE_HideChat;
	this.close = INTERFACE_CloseChat;
	this.focus = INTERFACE_FocusChat;
	this.blur = INTERFACE_BlurChat;
	this.minimize = INTERFACE_MinimizeChat;
	this.maximize = INTERFACE_MaximizeChat;

	this.addMessage = INTERFACE_AddChatMessage;
	this.addMessageError = INTERFACE_AddChatMessageError;
	this.inputFocus = INTERFACE_InputChatFocus;
	this.setPosition = INTERFACE_SetChatPosition;
	this.setTitle = INTERFACE_SetChatTitle;
}


/**
* Create chat elements
*/
/*
function INTERFACE_OpenChat(Username, Status)
{
	var Chat;
	var Node;

	Node = document.getElementById("ChatList");

	if (!Node)
	{
		return false;
	}

	// Create chat elements
	Chat = INTERFACE_CreateChat(Username, Status);
	Node.appendChild(Chat.ChatItem);
	Chat.Elements.InputFocus();

	return true;
}
*/
/**
* Show chat window
*/
function INTERFACE_ShowChat(Element)
{
	var Pos = this.position;

	if(Element != null)
	{
		Element.appendChild(this.chatWindow);
	}
	else
	{
		var ChatBar = document.getElementById("ChatList");
		ChatBar.appendChild(this.chatWindow);
		//document.body.appendChild(this.chatWindow);
	}

	//this.chatWindow.style.left = (Pos * 200) +"px";
	this.chatWindow.style.display = "block";
}

/**
* Hide chat window
*/
function INTERFACE_HideChat()
{
	this.chatWindow.style.display = "none";
}

/**
* Close a chat
*/
function INTERFACE_CloseChat()
{
	this.chatWindow.parentNode.removeChild(this.chatWindow);
}

/**
* Giving focus to a chat window
*/
function INTERFACE_FocusChat()
{
	this.chatTitle.className = "title_selec";
}

/**
* Remove focus of a chat window
*/
function INTERFACE_BlurChat()
{
	this.chatTitle.className = "title";
}

function INTERFACE_MinimizeChat()
{
	this.chatInner.style.display = "none";
	this.chatWindow.style.top = "157px";
	this.visible = false;

	this.chatList.style.display = "none";
}

function INTERFACE_MaximizeChat()
{
	this.chatInner.style.display = "block";
	this.chatWindow.style.top = "0px";
	this.visible = true;
	
	this.chatList.style.display = "block";
}

/**
* Show a message in a open chat
*/
function INTERFACE_AddChatMessage(Username, Message)
{
	var Item, Time, NewMessage;

	// Get current time
	Time = UTILS_GetTime();

	NewMessage = "<strong>"+Time+" "+Username+"</strong>: "+Message;

	Item = UTILS_CreateElement("li", null, null, NewMessage);
	this.chatList.appendChild(Item);
	this.chatList.scrollTop = this.chatList.scrollHeight + this.chatList.clientHeight;

	return true;
}

function INTERFACE_AddChatMessageError(Message)
{
	var Item;

	Item = UTILS_CreateElement("li", null, "error", Message);

	this.chatList.appendChild(Item);
	this.chatList.scrollTop = this.chatList.scrollHeight + this.chatList.clientHeight;

}

/*
* Set focus in input element 
*
* @author Danilo
*/
function INTERFACE_InputChatFocus()
{
	this.chatInput.focus();
}


function INTERFACE_SetChatPosition(Num)
{
	this.position = Num;
}

function INTERFACE_SetChatTitle(Str)
{
	this.chatTitleText.innerHTML = Str;
}


/**
* Create chat elements
*/
function INTERFACE_CreateChat(Username)
{
	var ChatItem, ChatInside, ChatInner, ChatTitleDiv, ChatTitle, ChatMessages;
	var Close, Input, State;
	var TitleSpan;

	ChatItem = UTILS_CreateElement("li", "Chat_"+Username, "chat");

	ChatInside = UTILS_CreateElement("div", null, "ChatInside");

	ChatTitleDiv = UTILS_CreateElement("div", "ChatTitleDiv");

	ChatTitle = UTILS_CreateElement("h3", null, "title_selec");

	TitleSpan = UTILS_CreateElement("span","ChatTitle_"+Username);

	ChatMessages = UTILS_CreateElement("ul", "ChatMessages_"+Username,"MessageList");
	ChatInner = UTILS_CreateElement("div", "ChatInner");

	State = UTILS_CreateElement("img",null,"minimize");

	Close = UTILS_CreateElement("img",null,"close");

	if (MainData.Browser == 0)
	{
		State.src = "./images/ie/minimize_chat.gif";
		Close.src = "./images/ie/close_chat.gif";
	}
	else
	{
		State.src = "./images/minimize_chat.png";
		Close.src = "./images/close_chat.png";
	}

	// Show/hide chat
	State.onclick = function () {
		CHAT_ChangeChatState(Username);
	}

	// Close chat
	Close.onclick = function () {
		CHAT_CloseChat(Username);
	}

	Input = UTILS_CreateElement("input");
	Input.type = "text";
	
	// Send message
	Input.onkeypress = function (event) {
		if ((UTILS_ReturnKeyCode(event) == 13) && (Input.value != ""))
		{
			if(Input.value.length <= MainData.MaxChatChar)
			{
				// Send chat message
				CHAT_SendMessage(Username, Input.value);
				Input.value = "";
			}
			else
			{
				CHAT_ErrorMessageLength(Username);
			}
		}
	}
	
	// Remove focus
	ChatItem.onclick = function(){
		CHAT_BlurChat(Username);
	}
	ChatInner.onclick = function(){
		CHAT_BlurChat(Username);
	}
	Input.onfocus = function(){
		CHAT_BlurChat(Username);
	}

	ChatTitle.appendChild(TitleSpan);
	ChatTitle.appendChild(State);
	ChatTitle.appendChild(Close);
	
	ChatTitleDiv.appendChild(ChatTitle);

	ChatInner.appendChild(ChatMessages);
	ChatInner.appendChild(Input);

	ChatInside.appendChild(ChatTitleDiv);
	ChatInside.appendChild(ChatInner);

	ChatItem.appendChild(ChatInside);

	return {ChatWindow:ChatItem, ChatInner:ChatInner, ChatList:ChatMessages, ChatTitle:ChatTitle, ChatTitleSpan:TitleSpan, ChatInput:Input, MinMax:State}
}





/***********************************
 * CHAT BOTTOM BAR
 * *********************************/
/**
* Create chat list elements
*/
function INTERFACE_CreateChatList()
{
	var ChatDiv, ChatList;
	var ScreenHeight = document.documentElement.clientHeight;

	ChatDiv = UTILS_CreateElement("div", "Chat");
	ChatList = UTILS_CreateElement("ul", "ChatList");

	ChatDiv.style.top = (ScreenHeight-179)+"px";
	ChatDiv.appendChild(ChatList);

	window.onresize = function() { INTERFACE_ChatListPositioning(); };
	window.onscroll = function() { INTERFACE_ChatListPositioning(); };

	return ChatDiv;
}

/**
* Positioning chat list
*/
function INTERFACE_ChatListPositioning()
{
	var Node = document.getElementById("Chat");
	var ScreenHeight, ScreenScroll;

	if (!Node)
	{
		return false;
	}
	ScreenHeight = document.documentElement.clientHeight;
	ScreenScroll = document.documentElement.scrollTop;
	Node.style.top = (ScreenHeight+ScreenScroll-179)+"px";

	return true;
}
