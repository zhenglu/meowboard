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
 * @file 	announce.js
 * @brief	Functions to parse announces offered and receive.
 *
 * See interface announce window (scripts/interface/announce.js), data
 * methods to announce list (scripts/data/data.js) and window announce
 * (scripts/window/window.js).
 */

/**
 * Handle announce list
 */
function ANNOUNCE_HandleAnnounce(XML)
{
	var i;
	var Id = XML.getAttribute("id");
	var Category;
	var AutoFlag;
	var AnnounceId;
	var Rated;

	var PlayerTag;
	var Username, Color, Time, Inc;
	var Announces;

	var Buffer = "";

	// Get Announce list
	if(Id == MainData.Const.IQ_ID_GetAnnounceMatch)
	{
		Announces = XML.getElementsByTagName("announcement");
		
		// There is no announced match
		if(Announces.length == 0)
		{
			ANNOUNCE_HideLoadingAnnounce();
			ANNOUNCE_ShowNoAnnounce();
		}
		else
		{
			for(i=0; i< Announces.length ; i++)
			{
				Category = Announces[i].getAttribute("category");
				AutoFlag = Announces[i].getAttribute("autoflag");
				AnnounceId = Announces[i].getAttribute("id");
				Rated = Announces[i].getAttribute("rated");
				AnnounceId = Announces[i].getAttribute("id");

				PlayerTag = Announces[i].getElementsByTagName("player")[0];
				Username = PlayerTag.getAttribute("jid").split("@")[0];
				Color = PlayerTag.getAttribute("color");
				Time = PlayerTag.getAttribute("time");
				Inc = PlayerTag.getAttribute("inc");

				if(Color == null)
				{
					Color = "";
				}

				ANNOUNCE_AddAnnounce(Username, Color, Time, Inc, Category, Rated, AutoFlag, AnnounceId);

				// Hide loading and no announce message
				ANNOUNCE_HideLoadingAnnounce();
				ANNOUNCE_HideNoAnnounce();

				// TODO -> Still show no announce message
				// if come some announce from server
			}
		}
	}
	// Accepted announce
	/*
	else if(Id == MainData.Const.IQ_ID_AcceptAnnounceMatch)
	{
		WINDOW_Alert("Announce accept","Announce accepted, start game!")
	}
	else if(Id == MainData.Const.IQ_ID_RemoveAnnounceMatch)
	{
		Announces = XML.getElementsByTagName("announcement");

		AnnounceId = Announces[0].getAttribute("id");

		ANNOUNCE_RemoveAnnounce(AnnounceId);
	}
	*/
	return Buffer;
}

/**
 * Handle announce start game
 */
function ANNOUNCE_HandleAnnounceGame(XML)
{
	var Id = XML.getAttribute("id");

	var GameRoomTag, Room;

	var Buffer = "";

	// Accepted announce, start game
	GameRoom = XML.getElementsByTagName("game_room")[0];

	Room = GameRoom.getAttribute("jid");
	Room += "/"+MainData.Username;

	Buffer += CONTACT_ChangeStatus("playing", false);
	Buffer += MESSAGE_Presence(Room);

	return Buffer;
}
/*
 * Handle announce error messages
 */
function ANNOUNCE_HandleAnnounceError(XML)
{
	var Id = XML.getAttribute("id");
	var Error;
	var Type;
	var AnnounceId, Announces;

	if(Id == MainData.Const.IQ_ID_AcceptAnnounceMatch)
	{
		Error = XML.getElementsByTagName("error")[0];
		Type = Error.getAttribute("type");

		Announces = XML.getElementsByTagName("announcement");
		AnnounceId = Announces[0].getAttribute("id");

		switch(Type)
		{
			case "modify":
				//WINDOW_Alert("Announce error","Announce non exists anymore");
				WINDOW_Alert(UTILS_GetText("announce_accept_error"),UTILS_GetText("announce_non_exist"));
				// Remove announce
				ANNOUNCE_RemoveAnnounce(AnnounceId);
				break;
		}
	}
}

/*
 * Send a message to create a announce with players parameters
 */
function ANNOUNCE_SendAnnounce(Username, Color, Time, Inc, Category, Rated, Min, Max)
{
	var Autoflag = "true";
	var Player = new Object();

	Player.Name = Username;
	Player.Color = Color;
	Player.Time = Time*60;
	Player.Inc = Inc;

	if (!Min.match(/^\d*$/))
	{
		WINDOW_Alert(UTILS_GetText("announce_error_title"),UTILS_GetText("announce_invalid_min_rating"));
		return false;
	}
	if (!Max.match(/^\d*$/))
	{
		WINDOW_Alert(UTILS_GetText("announce_error_title"),UTILS_GetText("announce_invalid_min_rating"));
		return false;
	}
	if (parseInt(Min) > parseInt(Max))
	{
		WINDOW_Alert(UTILS_GetText("announce_error_title"),UTILS_GetText("announce_invalid_interval"));
		return false;
	}
	
	CONNECTION_SendJabber(MESSAGE_AnnounceMatch(Player, Rated, Category, Min, Max, Autoflag));
	return true;
}

/*
 * Send a message to get announced games
 */
function ANNOUNCE_GetAnnounceGames()
{
	var XMPP = "";

	var Offset = 0;
	var NumResult = 10;
	var MinTime = "";
	var MaxTime = "";
	var Category = "";
	var User = true;

	XMPP += MESSAGE_GetAnnounceMatch(Offset, NumResult, MinTime, MaxTime, Category, User);
	User = false;
	XMPP += MESSAGE_GetAnnounceMatch(Offset, NumResult, MinTime, MaxTime, Category, User);

	CONNECTION_SendJabber(XMPP);
}

/*
 * Add a announce in data struct and show in challenge menu
 */
function ANNOUNCE_AddAnnounce(Username, Color, Time, Inc, Category, Rated, Autoflag, AnnounceId)
{
	var Player = new Object();

	if(MainData.FindAnnounce(AnnounceId) == null)
	{
		Player.Name = Username;
		Player.Color = Color;
		Player.Time = Time
		Player.Inc = Inc;

		MainData.AddAnnounce(Username, Color, Time, Inc, Category, Rated, Autoflag, AnnounceId)
		MainData.ChallengeMenu.addAnnounce(Player, Time, Inc, Rated, "true", AnnounceId);
	}
}

/*
 * Remove a announce in data struct and challenge menu using announce id
 * and send a message to cancel anounce
 */
function ANNOUNCE_RemoveAnnounce(Id)
{
	MainData.RemoveAnnounce(Id);
	MainData.ChallengeMenu.removeAnnounce(Id);

	ANNOUNCE_CancelAnnounce(Id);
}

/*
 * Sena a message to cancel announce using announce id
 */
function ANNOUNCE_CancelAnnounce(Id)
{
	var XMPP = "";

	XMPP += MESSAGE_RemoveAnnounceMatch(Id);

	CONNECTION_SendJabber(XMPP);
}

/*
 * Send a message to accept some announce using announce id
 */
function ANNOUNCE_AcceptAnnounce(Id)
{
	var XMPP = "";
	
	XMPP += MESSAGE_AcceptAnnounceMatch(Id);

	CONNECTION_SendJabber(XMPP);
}


function ANNOUNCE_ShowLoadingAnnounce()
{
	MainData.ChallengeMenu.showLoadingAnnounce();
}

function ANNOUNCE_HideLoadingAnnounce()
{
	MainData.ChallengeMenu.hideLoadingAnnounce();
}

function ANNOUNCE_ShowNoAnnounce()
{
	MainData.ChallengeMenu.showNoAnnounce();
}

function ANNOUNCE_HideNoAnnounce()
{
	MainData.ChallengeMenu.hideNoAnnounce();
}
