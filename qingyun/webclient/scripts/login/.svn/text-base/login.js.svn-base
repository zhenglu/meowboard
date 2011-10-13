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

/*
* Login Controller
* This file has all functions that is used on Login
*/


/**
* Make login on Jabber Server
*
* @return none
* @public
*/
function LOGIN_Login(Username, Passwd, RememberPass)
{
	// Pre-validation
	if (!UTILS_ValidateUsername(Username))
	{
		alert (UTILS_GetText("login_validate_user"));
		return;
	}

	// Store user infomations 
	MainData.Username = Username;
	MainData.Password = Passwd;

	// Set connection status to conneting
	MainData.ConnectionStatus = 1;

	// Set new RID and reset SID
	MainData.RID = Math.round( 100000.5 + ( ( (900000.49999) - (100000.5) ) * Math.random() ) );
	MainData.SID = -1;

	// Login on Jabber Server
	CONNECTION_ConnectJabber();

	// Create Cookies
	UTILS_CreateCookie("Username", Username, MainData.CookieValidity);
	UTILS_CreateCookie("RememberPass", RememberPass, MainData.CookieValidity);

	//TODO -> Fix to IE
	if (RememberPass)
		UTILS_CreateCookie("Passwd", Passwd, MainData.CookieValidity);
	else
		UTILS_DeleteCookie("Passwd");
	
	// Disable inputs
	INTERFACE_LoginDisableInput();

	// Clear error message
	INTERFACE_ClearError();
}


/**
* Make logout 
*
* @return none
* @public
*/
function LOGIN_Logout()
{
	var XMPP = "";
	NoCache.DateTime = new Date();
	// Setting structure as disconnected
	MainData.ConnectionStatus = -1;

	// Logout from jabber
	XMPP += MESSAGE_EndConnection(MESSAGE_Unavailable());
	CONNECTION_SendJabber(XMPP);

	//Stop game count timer of current game 
	if(MainData.CurrentGame != null)
	{
		MainData.CurrentGame.Game.StopTimer();
	}

	START_Restart();
}


function LOGIN_LeavePage()
{
	var XMPP = "";
	XMPP += MESSAGE_EndConnection(MESSAGE_Unavailable());
	CONNECTION_SendJabber(XMPP);
}

/**
* Make logout 
*
* @return none
* @public
*/
function LOGIN_Disconnected()
{
	// Setting structure as disconnected
	MainData.ConnectionStatus = -1;

	INTERFACE_StopInterface();
}

/**
* Clear Login window and start interface 
*
* @return none
* @public
*/
function LOGIN_EndLogin()
{
	INTERFACE_EndLogin();

	// Remove auto vertical align middle to login
	document.body.removeAttribute("onresize");
}

/**
* Show a error message on login
*
* @return none
* @public
*/
function LOGIN_LoginFailed(Msg)
{
	//Show error message
	INTERFACE_ShowErrorMessage(Msg);

	//Enable inputs
	INTERFACE_LoginEnableInput();

	// Hide login message
	INTERFACE_HideLoginMessage();
}

/**
 * Show a connection string when start login
 *
 * @param	Msg	Message string
 * @author	Rubens Suguimoto
 */
function LOGIN_LoginMsg(Msg)
{
	INTERFACE_ShowLoginMessage(Msg);
}
