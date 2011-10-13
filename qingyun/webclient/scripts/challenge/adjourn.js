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
 * @file 	adjourn.js
 * @brief	Functions to parse adjourned games message and show result
 * 		in interface
 *
 * See interface challenge menu (scripts/interface/challengemenu.js) and data
 * methods to adjourn/postpone game list (scripts/data/data.js)
 *
 * PS: Currently, adjourn and postpone games are considered the same thing.
*/

/**
 * @brief 	Parse XML with adjourned games list and show in challenge menu
 *
 * @param	XML	XML with adjourned games
 * @return	Buffer with other XMPP to send
 * @author	Rubens Suguimoto
 */
function CHALLENGE_HandleAdjourn(XML)
{
	var Query = XML.getElementsByTagName("query");
	var Xmlns;
	var Buffer = "";
	
	var AdjournList = XML.getElementsByTagName("game");
	var Game, Players;
	var Player1, Player2;
	var AdjournId, Category, Date;

	var i;

	for(i=0; i< AdjournList.length; i++)
	{
		Game = AdjournList[i];

		AdjournId = Game.getAttribute("id");
		Category = Game.getAttribute("category");
		Date = Game.getAttribute("time_stamp").split("T")[0];

		if(MainData.FindPostpone(AdjournId) == null)
		{
			Players = Game.getElementsByTagName("player")

			Player1 = new Object();
			Player2 = new Object();

			Player1.Name = Players[0].getAttribute("jid").split("@")[0];
			Player1.Color= Players[0].getAttribute("role");
			Player1.Time = Players[0].getAttribute("time");
			Player1.Inc  = Players[0].getAttribute("inc");

			Player2.Name = Players[1].getAttribute("jid").split("@")[0];
			Player2.Color= Players[1].getAttribute("role");
			Player2.Time = Players[1].getAttribute("time");
			Player2.Inc  = Players[1].getAttribute("inc");

			if(Player1.Name == MainData.Username)
			{
				// Add in main data postpone list
				MainData.AddPostpone(Player2, Category, Date, AdjournId);

				// Add in challenge menu
				MainData.ChallengeMenu.addPostpone(Player2, Category, Date, AdjournId);

				CHALLENGE_PostponePresence(Player2.Name);
			}
			else
			{
				// Add in main data postpone list
				MainData.AddPostpone(Player1, Category, Date, AdjournId);

				// Add in challenge menu
				MainData.ChallengeMenu.addPostpone(Player1, Category, Date, AdjournId);

				CHALLENGE_PostponePresence(Player1.Name);
			}
		}
	}

	// Show postpone games
	MainData.ChallengeMenu.showPostpone();

	return Buffer;
}

/**
 * @brief 	Parse presence to adjourn game user;
 *
 * Parse presence to user in challenge list. I.E.: if user comes offline
 * the adjourn game with this user turn unavailable to play.
 *
 * @param	XML	XML with adjourned games
 * @return	Buffer with other XMPP to send
 * @author	Rubens Suguimoto
 */
function CHALLENGE_HandlePresence(XML)
{
	var GeneralRoom = XML.getAttribute("from").split("@")[0];
	var UserRoom;
	var Item, Username, i;
	var Buffer = "";

	// Get presence from general room (where all user is connected)
	if(GeneralRoom == MainData.RoomDefault)
	{
		Username = XML.getAttribute("from").split("/")[1];
		CHALLENGE_PostponePresence(Username);
	}

	return Buffer;
}



/**
 * @brief	Update status of user in postpone list
 *
 * @param	Username	Name used by user
 * @return	Empty string;
 * @author	Rubens Suguimoto
 */
function CHALLENGE_PostponePresence(Username)
{
	var i = MainData.FindUserInRoom(MainData.RoomDefault, Username);

	//If user is founded, set adjourn game to available, else unavailable
	if(i != null)
	{
		MainData.ChallengeMenu.updatePostpone(Username, "online");
	}
	else
	{
		MainData.ChallengeMenu.updatePostpone(Username, "offline");
	}

	return "";
}


/**
 * @brief 	Create and send a message to resume a game with some player
 *
 * @param	AdjournId	Adjourned game Id;
 * @return	Empty string;
 * @author	Rubens Suguimoto
 */
function CHALLENGE_SendResumeGame(AdjournId)
{
	var XMPP = "";
	
	var PostponePos = MainData.FindPostpone(AdjournId);
	var Postpone = MainData.PostponeList[PostponePos];

	var Challenger = new Object();
	var Challenged = new Object();

	// Create challenge in challenge list
	Challenged.Name = Postpone.Challenged.Name;
	Challenged.Color = Postpone.Challenged.Color;
	Challenged.Time = Postpone.Challenged.Time;
	Challenged.Inc = Postpone.Challenged.Inc;

	Challenger.Name = MainData.Username;
	Challenger.Color = "undefined";
	Challenged.Time = 0;
	Challenged.Inc = 0;

	// Add challenge in challenge list
	MainData.AddChallenge("offer_adj", Challenger, Challenged, Postpone.Category, "false", null);

	// Create and send message to resume adjourned game
	XMPP += MESSAGE_ChallengeResumeGame(AdjournId);
	CONNECTION_SendJabber(XMPP);

	// Remove adjourned game from postponed list;
	CHALLENGE_RemovePostpone(AdjournId);

	//TODO -> Remove and don't show again a removed adjourn game with 
	// AdjournId

	return "";
}

/**
 * @brief 	Create and send message to get adjourned games list
 *
 * @return	Empty string;
 * @author	Rubens Suguimoto
 */
function CHALLENGE_GetAdjournGames()
{
	var XMPP;
	var Num = 10; // Get just 10 adjourned games

	XMPP = MESSAGE_ChallengeGetAdjournList(Num,0);

	CONNECTION_SendJabber(XMPP);

	return "";
}

/**
 * @brief	Remove post pone from maindata and challenge menu
 *
 * @param	Id	Adjourned/Postpone game Id
 * @return	Empty string;
 * @author	Rubens Suguimoto
 */

function CHALLENGE_RemovePostpone(Id)
{
	MainData.RemovePostpone(Id);
	MainData.ChallengeMenu.removePostpone(Id);

	return "";
}
