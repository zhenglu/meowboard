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
* @file	connection.js
* @brief	This file has all functions that is used to provide a connection
* 		with a Jabber Server.
*/

/**
* @brief	 Start connection to Jabber server
*
* @return 	Empty string;
* @author	Pedro Rocha
*/
function CONNECTION_ConnectJabber(XML)
{
	if(XML == null)
	{
		switch (MainData.ConnectionStatus)
		{
			// Start connection, open stream with bosh
			case (1):
				CONNECTION_SendJabber(MESSAGE_StartConnection());
				LOGIN_LoginMsg(UTILS_GetText("login_connection_start"));
				break;

			// Send Username
			case (2):
				CONNECTION_SendJabber(MESSAGE_SendUsername());
				LOGIN_LoginMsg(UTILS_GetText("login_connection_user"));
				break;

			// Send password and resource to login
			case (3):
				CONNECTION_SendJabber(MESSAGE_SendPasswd());
				LOGIN_LoginMsg(UTILS_GetText("login_connection_passwd"));
				break;
		}
	}
	else
	{
		// Send a wait message
		CONNECTION_SendJabber();
	}

	return "";
}

/**
* @brief	Send a XML post
*
* @param	XMPP	XMPP messages;
* @return	Empty string
* @author	Pedro Rocha
*/
function CONNECTION_SendJabber()
{
	var Post = "", DT, i;
	var HttpRequest;

	// If receive too many parameters, merge then
	for (i=0; i<arguments.length; i++)
	{
		Post += arguments[i];
	}

	// Check if connection status == "disconnected" or SID not initialized
	if ((MainData.SID != -1) && (MainData.ConnectionStatus != -1))
	{
		Post = MESSAGE_MakeXMPP(Post);
	}

	// Create XMLHttpRequest
	if (window.XMLHttpRequest) 
	{
		// Mozilla, Opera, Galeon
		HttpRequest = new XMLHttpRequest();
		if (HttpRequest.overrideMimeType) 
			HttpRequest.overrideMimeType("text/xml");
	}
	else if (window.ActiveXObject) 
	{
		// Internet Explorer
		try
		{
			HttpRequest = new ActiveXObject("Microsoft.XMLHTTP");
		}
		catch(e)
		{
			HttpRequest = new ActiveXObject("Msxml2.XMLHTTP");
		}
	}

	// Avoid browser caching
	DT = Math.floor(Math.random()*10000);

	//HttpRequest.open('POST','http://'+MainData.HostPost+'/http-bind?id='+DT , true);
	HttpRequest.open('POST','http://'+MainData.HostPost+'/jabber?id='+DT , true);
	HttpRequest.setRequestHeader("Content-Type", "application/x-www-form-urlencoded; charset=UTF-8");
	
	// Normal parse messages
	if (MainData.ConnectionStatus == 0)
	{
		HttpRequest.onreadystatechange = function(){
			CONNECTION_ReceiveXml(HttpRequest);
		}
	}
	// Conection parse messages
	else if (MainData.ConnectionStatus > 0)
	{
		HttpRequest.onreadystatechange = function(){
			CONNECTION_ReceiveConnection(HttpRequest);
		}
	}

	// Send request to server
	HttpRequest.send(Post);

	// Save last post in Data Struct
	MainData.LastXML = Post;

	// Add Post in data Struct
	MainData.AddHttpPost(HttpRequest);


	// Increment RID
	MainData.RID++;

	return "";
}


/**
* @brief	Receive Connection messages and make all steps to connect user
*
* @return 	Empty string
* @author	Pedro Rocha
*/
function CONNECTION_ReceiveConnection(HttpRequest)
{
	var XML, XMLBuffer;
	var Error, ErrorCode;
	var BodyType;
	var Iq, IqType;
	var Status;

	// Check ready state of HTTP Request
	if (HttpRequest.readyState == 4 )
	{
		try
		{
			Status = HttpRequest.status;
		}
		catch(e)
		{
			return "";
		}	

		if(Status == 200)
		{
			XML = HttpRequest.responseXML;

			// Check Bosh connection 
			BodyType = XML.getElementsByTagName("body")[0].getAttribute("type");
			if(BodyType != null)
			{
				if(BodyType == "terminate")
				{
					LOGIN_LoginFailed(UTILS_GetText("login_connection_closed"));
					return "";
				}
			}

			switch (MainData.ConnectionStatus)
			{
				 case (1):
					if(XML.getElementsByTagName("body")[0].getAttribute("sid") == null)
					{
						LOGIN_LoginFailed(UTILS_GetText("login_connection_refused"));
						return "";
					}
					else // Step one OK
					{
						// Get SID from first message
						MainData.SID = XML.getElementsByTagName("body")[0].getAttribute("sid");

						MainData.ConnectionStatus++;

						// Send second step connection
						CONNECTION_ConnectJabber();
					}
					break;

				case(2):
					Iq = XML.getElementsByTagName("iq")[0];
					if(Iq != null)
					{
						IqType = Iq.getAttribute("type");
						if(IqType == "result")
						{
							MainData.ConnectionStatus++;
							// Send third step connection
							CONNECTION_ConnectJabber();
						}
					}
					// this case should happen when server is very slow to response authentication;
					else
					{
						// Send wait to bosh until server response the first message of authentication
						CONNECTION_ConnectJabber(MESSAGE_Wait());
					}
					break;

				case(3):
					Iq = XML.getElementsByTagName("iq")[0];
					if(Iq != null)
					{
						IqType = Iq.getAttribute("type");
						// Check errors in username and passwd authentication
						if(IqType == "error")
						{
							Error = XML.getElementsByTagName("error")[0];
							if (Error != null)
							{
								ErrorCode = Error.getAttribute("code");
								switch(ErrorCode)
								{
									// Username and passwd invalid
									case "401":
										LOGIN_LoginFailed(UTILS_GetText("login_invalid_user"));
										break;
									// User was banned by admin
									case "405":
										LOGIN_LoginFailed(UTILS_GetText("login_banned_user"));
										break;
								}
							}
						}
						else //if(IqType == "result")
						{
							// Send a message to bosh, to
							// wait while loading scripts, css and images
							//CONNECTION_SendJabber(MESSAGE_Wait());
							/******** LOAD FILES**********/
							// Start load scripts, css and images
							LOAD_StartLoad();

							// Set connected status
							MainData.ConnectionStatus = 0;

							CONNECTION_SendJabber(MESSAGE_Wait());
						}
					}
					// this case should happen when server is very slow to response authentication;
					else
					{
						// Send wait to bosh until server response the second message of authentication
						CONNECTION_ConnectJabber(MESSAGE_Wait());
					}

					break;
			}
		}
		// Server offline
		else if (HttpRequest.status == 503)
		{
			LOGIN_LoginFailed(UTILS_GetText("login_server_down"));
		}
		else if (HttpRequest.status == 404)
		{
			LOGIN_LoginFailed(UTILS_GetText("login_server_not_founded"));
		}
		else
		{	
			LOGIN_LoginFailed(UTILS_GetText("login_server_down"));
		}

		// Remove post from data struct
		MainData.RemoveHttpPost(HttpRequest);
	}
	return "";
}


/**
* @brief 	Receive a Jabber message when user is already connected
*
* @return 	Empty string
* @author	Pedro Rocha
*/
function CONNECTION_ReceiveXml(HttpRequest)
{
	var XML, Buffer = "";
	var State, Status;

	//Check if HttpRequest Object exists
	/*
	if((MainData == null) || (MainData.HttpRequest == null))
	{
		return "";
	}
	*/
	// User was disconnected 
	if (MainData.ConnectionStatus == -1)
	{
		return "";
	}


	//if (MainData.HttpRequest.readyState == 4)
	if (HttpRequest.readyState == 4)
	{
		// Try to get http request status
		try
		{
			Status = HttpRequest.status;
		}
		catch (e)
		{
			return "";
		}

		if (Status == 200)
		{
			// Get Xml response
			XML = HttpRequest.responseXML;

			// User disconnected 
			if (MainData.ConnectionStatus == -1)
			{
				return "";
			}

			// Forward XML to parser
			Buffer = PARSER_ParseXml(XML);

			// Parser returned some xml: send it
			if ((Buffer != "") && (Buffer != null) && (Buffer != undefined))
			{
				CONNECTION_SendJabber(Buffer);
			}
			else
			{
				// Remove post from data struct
				MainData.RemoveHttpPost(HttpRequest);

				// Send a wait message to jabber if is there 
				// no pendend post
				if(MainData.HttpRequest.length == 0)
				{
					CONNECTION_SendJabber(MESSAGE_Wait());
				}

				return "";
			}
		}

		// Re-send last XML
		else if (HttpRequest.status == 502)
		{
			CONNECTION_SendJabber(MainData.LastXML);
		}

		// Server down
		else if (HttpRequest.status == 503)
		{
			// Show this message if user is connected
			if(MainData.ConnectionStatus == 0)
			{
				alert(UTILS_GetText("error_disconnected"));
			
				START_Restart();	
			}
		}

		// Remove post from data struct
		MainData.RemoveHttpPost(HttpRequest);
	}

	return "";
}
