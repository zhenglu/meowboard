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
* Create xmpp messages
*/


/**
* Append xmpp body to messages
*/
function MESSAGE_MakeXMPP(Msg, Type)
{
	var XMPP;
	

	if (Msg != "")
	{
		XMPP = "<body rid='"+MainData.RID+"' sid='"+MainData.SID+"' xmlns='http://jabber.org/protocol/httpbind'>"+Msg+"</body>";
	}
	else
	{
		XMPP = '<body rid="'+MainData.RID+'" sid="'+MainData.SID+'" xmlns="http://jabber.org/protocol/httpbind"/>';
	}

	return XMPP;
}


/**
* Make a wait message
*/
function MESSAGE_Wait()
{
    return "";
}


/**********************************
 * MESSAGES - CONNECTION 
 **********************************/

/**
* Get an SID from bind
*/
function MESSAGE_StartConnection()
{
	var XMPP = "<body hold='1' rid='"+MainData.RID+"' to='"+MainData.Host+"' ver='1.6' wait='10' xml:lang='en' xmlns='http://jabber.org/protocol/httpbind'/>";

	return XMPP;
}

/**
* End connection
*/
function MESSAGE_EndConnection(Unavailable)
{
	var XMPP = "<body type='terminate' rid='"+MainData.RID+"' sid='"+MainData.SID+"' xmlns='http://jabber.org/protocol/httpbind' >"+Unavailable+"</body>";

	return XMPP;
}

/**
* Send username to jabber
*/
function MESSAGE_SendUsername()
{
	var XMPP;

	XMPP  = "<iq xml:lang='"+UTILS_JabberLang(MainData.Lang)+"' type='get' id='auth_1' to='"+MainData.Host+"'>";
	XMPP += "<query xmlns='jabber:iq:auth'>";
	XMPP += "<username>"+MainData.Username+"</username>";
	XMPP += "</query></iq>";

	return XMPP;
}

/**
* Send password to jabber
*/
function MESSAGE_SendPasswd()
{
	var XMPP;
	
	XMPP  = "<iq xml:lang='"+UTILS_JabberLang(MainData.Lang)+"' type='set' id='auth_2' to='"+MainData.Host+"' >";
	XMPP += "<query xmlns='jabber:iq:auth'>";
	XMPP += "<username>"+MainData.Username+"</username>";
	XMPP += "<password>"+MainData.Password+"</password>";
	XMPP += "<resource>"+MainData.Resource+"</resource></query></iq>";

	return XMPP;
}


/**********************************
 * MESSAGES - USER AND ROOM LIST
 **********************************/

/**
* Ask contact list to jabber
*/
function MESSAGE_UserList()
{
	var XMPP;

   	XMPP  = "<iq xml:lang='"+UTILS_JabberLang(MainData.Lang)+"' type='get' id='"+MainData.Const.IQ_ID_GetUserList+"'>";
	XMPP += "<query xmlns='jabber:iq:roster'/></iq>";

	return XMPP;
}

/**
* Ask room list to jabber
*/
function MESSAGE_RoomList()
{
	var XMPP;

   	XMPP  = "<iq xml:lang='"+UTILS_JabberLang(MainData.Lang)+"' type='get' id='"+MainData.Const.IQ_ID_GetRoomList+"' to='"+MainData.ConferenceComponent+"."+MainData.Host+"'>";
	XMPP += "<query xmlns='http://jabber.org/protocol/disco#items'/></iq>";

	return XMPP;
}

/**********************************
 * MESSAGES - PRESENCE
 **********************************/
	
/**
* If 'To' isn't passed, send your presence to jabber,
* otherwise send presence to 'To'
*/
function MESSAGE_Presence(To)
{
	var XMPP;

	XMPP = "<presence xml:lang='"+UTILS_JabberLang(MainData.Lang)+"' from='"+MainData.Username+"@"+MainData.Host+"' ";

	// Presence to someone
	if (To != null)
	{
		XMPP += "to='"+To+"' ";
	}
	XMPP += ">";

	// Setting user status
	if (MainData.Status != "available")
	{
		XMPP += "<show>"+MainData.Status+"</show>";
	}

	XMPP += "</presence>";

	return XMPP;
}

/**
* Change Status
*/
function MESSAGE_ChangeStatus(NewStatus, RoomName)
{
	var XMPP;

	// Message to room
	if (RoomName)
	{
		if (NewStatus == "available")
		{
			XMPP = "<presence xml:lang='"+UTILS_JabberLang(MainData.Lang)+"' to='"+RoomName+"/"+MainData.Username+"' />";
		}
		else
		{
			XMPP = "<presence xml:lang='"+UTILS_JabberLang(MainData.Lang)+"' to='"+RoomName+"/"+MainData.Username+"' ><show>"+NewStatus+"</show></presence>";
		}
	}
	
	// General status change
	else
	{
		if (NewStatus == "available")
		{
			XMPP = "<presence xml:lang='"+UTILS_JabberLang(MainData.Lang)+"' xmlns='jabber:client' />";
		}
		else
		{
			XMPP = "<presence xml:lang='"+UTILS_JabberLang(MainData.Lang)+"' xmlns='jabber:client'><show>"+NewStatus+"</show></presence>";
		}
	}
	return XMPP;
}


/**
* Set offline on jabber or exit a room
*/
function MESSAGE_Unavailable(RoomName)
{
	var XMPP = "";
	var Type = null;

	// Exit from a room
	if (RoomName)
	{
		XMPP = "<presence xml:lang='"+UTILS_JabberLang(MainData.Lang)+"' to='"+RoomName+"' xmlns='jabber:client' type='unavailable'></presence>";
	}
	else // Exit from jabber
	{
		XMPP = "<presence xml:lang='"+UTILS_JabberLang(MainData.Lang)+"' xmlns='jabber:client' type='unavailable'></presence>";
	}
	return XMPP;
}

/**********************************
 * MESSAGES - CHAT AND GROUPCHAT
 **********************************/

/**
* Create a chat message
*/
function MESSAGE_Chat(To, Message)
{
	var XMPP;

	XMPP  = "<message xml:lang='"+UTILS_JabberLang(MainData.Lang)+"' to='"+To+"@"+MainData.Host+"/"+MainData.Resource+"' type='chat'>"
	XMPP += "<body>"+Message+"</body>";
	XMPP += "</message>";

	return XMPP;
}

/**
* Create a groupchat message
*/
function MESSAGE_GroupChat(To, Message)
{
	var XMPP;

	XMPP  = "<message xml:lang='"+UTILS_JabberLang(MainData.Lang)+"' to='"+To+"' type='groupchat'>"
	XMPP += "<body>"+Message+"</body>";
	XMPP += "</message>";

	return XMPP;
}

/**********************************
 * MESSAGES - RATING AND USER TYPE
 **********************************/

/**
* Message to get users ratings
*
* @deprecated
*/
function MESSAGE_Info(User)
{
	var XMPP;

	XMPP  = "<iq xml:lang='"+UTILS_JabberLang(MainData.Lang)+"' type='get' from='"+MainData.Username+"@"+MainData.Host+"/"+MainData.Resource+"' to='"+MainData.RatingComponent+"."+MainData.Host+"' id='"+MainData.Const.IQ_ID_GetRating+"'>";
	XMPP += "<query xmlns='"+MainData.Xmlns+"/chessd#info'>";
	XMPP += "<rating jid='"+User+"@"+MainData.Host+"' />";
	XMPP += "<type jid='"+User+"@"+MainData.Host+"' />";
	XMPP += "</query></iq>";
	
	return XMPP;
}

/**
* Message to get users ratings profile
*
*/
function MESSAGE_InfoProfile(User)
{
	var XMPP;

	XMPP  = "<iq xml:lang='"+UTILS_JabberLang(MainData.Lang)+"' type='get' from='"+MainData.Username+"@"+MainData.Host+"/"+MainData.Resource+"' to='"+MainData.RatingComponent+"."+MainData.Host+"' id='"+MainData.Const.IQ_ID_GetRating+"'>";
	XMPP += "<query xmlns='"+MainData.Xmlns+"/chessd#profile'>";
	XMPP += "<profile jid='"+User+"@"+MainData.Host+"' />";
	XMPP += "</query></iq>";
	
	return XMPP;
}


/**
* Message to get users ratings
*/
function MESSAGE_UserListInfo()
{
	var XMPP, i;

	XMPP  = "<iq xml:lang='"+UTILS_JabberLang(MainData.Lang)+"' type='get' from='"+MainData.Username+"@"+MainData.Host+"/"+MainData.Resource+"' to='"+MainData.RatingComponent+"."+MainData.Host+"' id='"+MainData.Const.IQ_ID_GetRating+"'>";
	XMPP += "<query xmlns='"+MainData.Xmlns+"/chessd#info'>";
	XMPP += "<rating jid='"+MainData.Username+"@"+MainData.Host+"' />";
	XMPP += "<type jid='"+MainData.Username+"@"+MainData.Host+"' />";

	// Ask for all contact list
	for (i=0; i<MainData.UserList.length; i++)
	{
		XMPP += "<rating jid='"+MainData.UserList[i].Username+"@"+MainData.Host+"' />";
		XMPP += "<type jid='"+MainData.UserList[i].Username+"@"+MainData.Host+"' />";
	}
	XMPP += "</query></iq>"

	return XMPP;
}

/**********************************
 * MESSAGES - INVITE
 **********************************/

/**
* Send a invite to user
*/
function MESSAGE_Invite(To)
{
	 return "<presence xml:lang='"+UTILS_JabberLang(MainData.Lang)+"' type='subscribe' to='"+To+"@"+MainData.Host+"' />"; 
}

/**
* Send a subscribed to user
*/
function MESSAGE_InviteAccept(To)
{
	return "<presence xml:lang='"+UTILS_JabberLang(MainData.Lang)+"' type='subscribed' to='"+To+"@"+MainData.Host+"' />";
}

/**
* Send a unsubscribed to user
*/
function MESSAGE_InviteDeny(To)
{
	return "<presence xml:lang='"+UTILS_JabberLang(MainData.Lang)+"' type='unsubscribed' to='"+To+"@"+MainData.Host+"' />";
}

/**
* Remove user from yout contact list
*/
function MESSAGE_RemoveContact(User)
{
	var XMPP;

	XMPP  = "<iq xml:lang='"+UTILS_JabberLang(MainData.Lang)+"' type='set' id='RemoveUser'>";
	XMPP += "<query xmlns='jabber:iq:roster'>";
	XMPP += "<item subscription='remove' jid='"+User+"@"+MainData.Host+"' />";
	XMPP += "</query></iq>";

	return XMPP;
}


/**********************************
 * MESSAGES - CHALLENGE
 **********************************/

/**
* Send a Challenge message
*
* Should be extended to support Bughouse with more two players.
* The Player's structure has the following fields:
*  - Name
*  - Color
*  - Inc
*  - Time
*
* The 'Color' field must content just the first letter
*/
function MESSAGE_Challenge(ChallengeID, Category, Rated, Players, MatchID)
{
	var i;
	var Id;
	var XMPP = "";

	// Setting iq's id
	if(ChallengeID == null)
	{
		Id = MainData.Const.IQ_ID_Challenge;
	}
	else
	{
		Id = ChallengeID;
	}

	/*
	for (i=0; i < Players.length; i++)
	{
		if (Players[i].Name == MainData.Username)
		{
			continue;
		}
		Id += "_"+Players[i].Name;
	}
	*/
	// Tag the id with the challenged player's name
	XMPP  = "<iq xml:lang='"+UTILS_JabberLang(MainData.Lang)+"' type='set' to='"+MainData.MatchComponent+"."+MainData.Host+"' id='"+Id+"'>";
	XMPP += "<query xmlns='"+MainData.Xmlns+"/chessd#match#offer'>";

	// Game offer
	if (MatchID == null)
	{
		XMPP += "<match category='"+Category+"' rated='"+Rated+"' >";
	}
	// Reoffer
	else 
	{
		XMPP += "<match category='"+Category+"' rated='"+Rated+"' id='"+MatchID+"' >";
	}

	// Creating players tags
	for (i=0; i < Players.length; i++)
	{
		// If player color was chose
		if (Players[i].Color != "")
		{
			XMPP += "<player jid='"+Players[i].Name+"@"+MainData.Host+"/"+MainData.Resource+"' time='"+Players[i].Time+"' inc='"+Players[i].Inc+"' color='"+Players[i].Color+"' />";
		}
		else
		{
			XMPP += "<player jid='"+Players[i].Name+"@"+MainData.Host+"/"+MainData.Resource+"' time='"+Players[i].Time+"' inc='"+Players[i].Inc+"' />";
		}
	}
	XMPP += "</match></query></iq>";
	
	return XMPP;
}


/**
* Accept a challange 
*/
function MESSAGE_ChallengeAccept(ChallengeID)
{
	var XMPP="";

	XMPP  = "<iq xml:lang='"+UTILS_JabberLang(MainData.Lang)+"' type='set' to='"+MainData.MatchComponent+"."+MainData.Host+"' id='"+MainData.Const.IQ_ID_Challenge+"'>";
	XMPP += "<query xmlns='"+MainData.Xmlns+"/chessd#match#accept'>";
	XMPP += "<match id='"+ChallengeID+"'>";
	XMPP += "</match></query></iq>";

	return XMPP;
}


/**
* Decline a challange 
*/
function MESSAGE_ChallengeDecline(ChallengeID)
{
	var XMPP="";

	XMPP  = "<iq xml:lang='"+UTILS_JabberLang(MainData.Lang)+"' type='set' to='"+MainData.MatchComponent+"."+MainData.Host+"' id='"+MainData.Const.IQ_ID_Challenge+"'>";
	XMPP += "<query xmlns='"+MainData.Xmlns+"/chessd#match#decline'>";
	XMPP += "<match id='"+ChallengeID+"'>";
	XMPP += "</match></query></iq>";

	return XMPP;
}


function MESSAGE_ChallengeGetAdjournList(Num, Offset)
{
	var XMPP="";

	XMPP += "<iq type='get' to='"+MainData.MatchComponent+"."+MainData.Host+"' id='get_adj'>";
	XMPP += "<query xmlns='"+MainData.Xmlns+"/chessd#adjourned#list'>";
	XMPP += "<search results='"+Num+"' offset='"+Offset+"'/>";
	XMPP += "</query></iq>";

	return XMPP;
}

function MESSAGE_ChallengeResumeGame(Id)
{
	var XMPP = "";
	XMPP += "<iq to='"+MainData.MatchComponent+"."+MainData.Host+"' type='set' id='offer_adj'>";
	XMPP += "<query xmlns='"+MainData.Xmlns+"/chessd#match#offer'>";
	XMPP += "<match adjourned_id='"+Id+"'/>";
	XMPP += "</query></iq>";

	return XMPP;
}

/**********************************
 * MESSAGES - GAME
 **********************************/

/**
* Get the list of all games been played
* 
* @return 	XMPP with iq to get all games been played
* @author 	Ulysses
*/
function MESSAGE_GameRoomList()
{
	var XMPP;

   	XMPP  = "<iq xml:lang='"+UTILS_JabberLang(MainData.Lang)+"' type='get' id='"+MainData.Const.IQ_ID_GetGamesList+"' to='"+MainData.GameComponent+"."+MainData.Host+"'>";
	XMPP += "<query xmlns='http://jabber.org/protocol/disco#items'/></iq>";

	return XMPP;
}

/**
* Get the list information of all games  been played
* 
* @return 	XMPP with iq to get all games been played with respective info
* @author 	Rubens
*/
function MESSAGE_GameRoomInfoList(Room)
{
	var XMPP;

	XMPP  = "<iq type='get' to='"+Room+"@"+MainData.GameComponent+"."+MainData.Host+"' id='"+MainData.Const.IQ_ID_GameInfo+"'>";
	XMPP += "<query xmlns='http://jabber.org/protocol/disco#info'/></iq>";

	return XMPP;
}

/**
* Send a game moviment
*/
function MESSAGE_GameMove(Move, GameID, Promotion)
{
	var XMPP="";

	XMPP  = "<iq xml:lang='"+UTILS_JabberLang(MainData.Lang)+"' type='set' to='"+GameID+"@"+MainData.GameComponent+"."+MainData.Host+"' id='"+MainData.Const.IQ_ID_GameMove+"'>";
	XMPP += "<query xmlns='"+MainData.Xmlns+"/chessd#game#move'>";
	XMPP += "<move long='"+Move+Promotion+"'>";
	XMPP += "</move></query></iq>";

	return XMPP;
}

/**
* Make a draw request
*/
function MESSAGE_GameRequestDraw (GameID)
{
	return (MESSAGE_GameRequests("Draw", GameID));
}

/**
* Make a cancel request
*/
function MESSAGE_GameRequestCancel (GameID)
{
	return (MESSAGE_GameRequests("Cancel", GameID));
}

/**
* Make a adjourn request
*/
function MESSAGE_GameRequestAdjourn (GameID)
{
	return (MESSAGE_GameRequests("Adjourn", GameID));
}

/**
* Make a resign message
*/
function MESSAGE_GameResign (GameID)
{
	return (MESSAGE_GameRequests("Resign", GameID));
}

/**
* Create the game requests messages
*/
function MESSAGE_GameRequests(Action, GameID)
{
	var XMPP = "";

	switch (Action) 
	{
		case "Draw":
			XMPP  = "<iq xml:lang='"+UTILS_JabberLang(MainData.Lang)+"' type='set' to='"+GameID+"@"+MainData.GameComponent+"."+MainData.Host+"' id='"+MainData.Const.IQ_ID_GameDraw+"'>";
			XMPP += "<query xmlns='"+MainData.Xmlns+"/chessd#game#draw'>";
			break;

		case "Cancel":
			XMPP  = "<iq xml:lang='"+UTILS_JabberLang(MainData.Lang)+"' type='set' to='"+GameID+"@"+MainData.GameComponent+"."+MainData.Host+"' id='"+MainData.Const.IQ_ID_GameCancel+"'>";
			XMPP += "<query xmlns='"+MainData.Xmlns+"/chessd#game#cancel'>";
			break;

		case "Adjourn":
			XMPP  = "<iq xml:lang='"+UTILS_JabberLang(MainData.Lang)+"' type='set' to='"+GameID+"@"+MainData.GameComponent+"."+MainData.Host+"' id='"+MainData.Const.IQ_ID_GameAdjourn+"'>";
			XMPP += "<query xmlns='"+MainData.Xmlns+"/chessd#game#adjourn'>";
			break;
	
		case "Resign":
			XMPP  = "<iq xml:lang='"+UTILS_JabberLang(MainData.Lang)+"' type='set' to='"+GameID+"@"+MainData.GameComponent+"."+MainData.Host+"' id='"+MainData.Const.IQ_ID_GameResign+"'>";
			XMPP += "<query xmlns='"+MainData.Xmlns+"/chessd#game#resign'>";
			break;
	
		default:
			break;
	}

	XMPP += "</query></iq>";

	return XMPP;
}


/**
* Make a draw accept
*/
function MESSAGE_GameDrawAccept (RoomID)
{
	return (MESSAGE_GameResponse("Draw", RoomID, ""));
}

/**
* Make a draw deny
*/
function MESSAGE_GameDrawDeny (RoomID)
{
	return (MESSAGE_GameResponse("Draw", RoomID, "-decline"));
}

/**
* Make a cancel accept
*/
function MESSAGE_GameCancelAccept (RoomID)
{
	return (MESSAGE_GameResponse("Cancel", RoomID, ""));
}

/**
* Make a cancel deny
*/
function MESSAGE_GameCancelDeny (RoomID)
{
	return (MESSAGE_GameResponse("Cancel", RoomID, "-decline"));
}

/**
* Make a adjourn accept
*/
function MESSAGE_GameAdjournAccept (RoomID)
{
	return (MESSAGE_GameResponse("Adjourn", RoomID, ""));
}

/**
* Make a adjourn deny
*/
function MESSAGE_GameAdjournDeny (RoomID)
{
	return (MESSAGE_GameResponse("Adjourn", RoomID, "-decline"));
}

/**
* Create the game response messages
*/
function MESSAGE_GameResponse(Action, RoomID, Response)
{
	var XMPP="";

	switch (Action) 
	{
		case "Draw":
			XMPP  = "<iq xml:lang='"+UTILS_JabberLang(MainData.Lang)+"' type='set' to='"+RoomID+"@"+MainData.GameComponent+"."+MainData.Host+"' id='"+MainData.Const.IQ_ID_GameDraw+"'>";
			XMPP += "<query xmlns='"+MainData.Xmlns+"/chessd#game#draw"+Response+"'>";
			break;

		case "Cancel":
			XMPP  = "<iq xml:lang='"+UTILS_JabberLang(MainData.Lang)+"' type='set' to='"+RoomID+"@"+MainData.GameComponent+"."+MainData.Host+"' id='"+MainData.Const.IQ_ID_GameCancel+"'>";
			XMPP += "<query xmlns='"+MainData.Xmlns+"/chessd#game#cancel"+Response+"'>";
			break;

		case "Adjourn":
			XMPP  = "<iq xml:lang='"+UTILS_JabberLang(MainData.Lang)+"' type='set' to='"+RoomID+"@"+MainData.GameComponent+"."+MainData.Host+"' id='"+MainData.Const.IQ_ID_GameAdjourn+"'>";
			XMPP += "<query xmlns='"+MainData.Xmlns+"/chessd#game#adjourn"+Response+"'>";
			break;
	
		default:
			break;
	}

	XMPP += "</query></iq>";

	return XMPP;
}

/**
 * Search for user current games.
 */

function MESSAGE_GameSearchCurrentGame()
{
	var XMPP = "";

	XMPP += "<iq type='get' to='"+MainData.GameComponent+"."+MainData.Host+"' id='"+MainData.Const.IQ_ID_SearchCurrentGame+"'>";
	XMPP += "<search xmlns='http://c3sl.ufpr.br/chessd#game'>";
	XMPP += "<game>";

	XMPP += "<player jid='"+MainData.Username+"@"+MainData.Host+"/"+MainData.Resource+"'/>";

	XMPP += "</game></search></iq>";

	return XMPP;
}

/**********************************
 * MESSAGES - SEARCH USER
 **********************************/

/**
*
*	Make search user message
*
*	@return	String with search user message
*	@author	Danilo Kiyoshi Simizu Yorinori


function MESSAGE_SearchUser()
{
	var XMPP;
	
	XMPP = "<iq type='get' to='"+MainData.SearchComponent+"."+MainData.Host+"' id='"+MainData.Const.IQ_ID_SearchUser+"'><query xmlns='jabber:iq:search'/></iq>";

	return MESSAGE_MakeXMPP(XMPP);
}

/**
*
*	Make search user message
*
*	@return	String with search user message
*	@author	Danilo Kiyoshi Simizu Yorinori
*/
function MESSAGE_SearchUser(Username, Option)
{
	var XMPP="";
/*FIXME: not check if the Username has been added @*/	
	XMPP = "<iq type='get' to='"+MainData.GameComponent+"."+MainData.Host+"' id='"+MainData.Const.IQ_ID_SearchUser+"' >";
	XMPP +=	"<query xmlns='http://c3sl.ufpr.br/chessd#search_user'>";
	XMPP += "<user name='" + Username  + "'/>";
	XMPP +=	"</query></iq>";

	return XMPP;
}


/**********************************
 * MESSAGES - ADMIN 
 **********************************/

/**
* Moderator kicks an accupant
* 
* @param 	Room 	The room that the target user is
* @param 	To 		Nick name of the target user in room
* @param 	Role 	User's privilege in room
* @param 	Reason 	Why the user will be kicked
* @return 	string
* @author 	Ulysses
*/
function MESSAGE_KickUserRoom (Room, To, Role, Reason)
{
	var XMPP = "";

	XMPP += "<iq xml:lang='"+UTILS_JabberLang(MainData.Lang)+"' id='"+MainData.Const.IQ_ID_KickUserRoom+"' to='"+Room+"@"+MainData.ConferenceComponent+"."+MainData.Host+"' type='set' >";
	XMPP += "<query xmlns='"+MainData.Xmlns+"/protocol/muc#admin' >";
	XMPP += "<item nick='"+To+"' role='"+Role+"' />";
	XMPP += "<reason>"+Reason+"</reason>";
	XMPP += "</query></iq>";
	
	return XMPP;
}

/**
 * @brief Message to kick a user from jabber
 *
 * Create message to kick user from jabber.
 *
 * @param 	Username	User's name
 * @author 	Rubens Suguimoto
 */
function MESSAGE_KickUser(Username, Reason)
{
	var XMPP = "";
	XMPP += "<iq type='set' to='"+MainData.AdminComponent+"."+MainData.Host+"' id='"+MainData.Const.IQ_ID_KickUser+"'>";
	XMPP += "<kick xmlns='"+MainData.Xmlns+"/chessd#admin' jid='"+Username+"@"+MainData.Host+"/"+MainData.Resource+"'>";
	XMPP += "<reason>"+Reason+"</reason>";
	XMPP += "</kick></iq>";
	
	return XMPP;
}

/**
 * @brief Message to ban a user from jabber
 *
 * Create message to ban user from jabber.
 *
 * @param 	Username	User's name
 * @author 	Rubens Suguimoto
 */
function MESSAGE_BanUser(Username, Reason)
{
	var XMPP = "";
	XMPP += "<iq type='set' to='"+MainData.AdminComponent+"."+MainData.Host+"/"+MainData.Resource+"' id='"+MainData.Const.IQ_ID_BanUser+"'>";
	XMPP += "<ban xmlns='"+MainData.Xmlns+"/chessd#admin' jid='"+Username+"@"+MainData.Host+"/"+MainData.Resource+"'>";
	XMPP += "<reason>"+Reason+"</reason>";
	XMPP += "</ban></iq>";
	
	return XMPP;
}

/**
 * @brief Message to unban a user from jabber
 *
 * Create message to unban user from jabber.
 *
 * @param 	Username	User's name
 * @author 	Rubens Suguimoto
 */
function MESSAGE_UnbanUser(Username, Reason)
{
	var XMPP = "";
	XMPP += "<iq type='set' to='"+MainData.AdminComponent+"."+MainData.Host+"/"+MainData.Resource+"' id='"+MainData.Const.IQ_ID_UnbanUser+"'>";
	XMPP += "<unban xmlns='"+MainData.Xmlns+"/chessd#admin' jid='"+Username+"@"+MainData.Host+"/"+MainData.Resource+"'>";
	XMPP += "<reason>"+Reason+"</reason>";
	XMPP += "</unban></iq>";
	
	return XMPP;
}

/**
 * @brief Message to get list off banned users
 *
 * Create message to get a list of all banned users
 *
 * @author 	Rubens Suguimoto
 */
function MESSAGE_GetBanList()
{
	var XMPP = "";

	XMPP += "<iq type='get' to='"+MainData.AdminComponent+"."+MainData.Host+"' id='"+MainData.Const.IQ_ID_GetBanList+"'>"
	XMPP += "<banned-list xmlns='http://c3sl.ufpr.br/chessd#admin'/>"
	XMPP += "</iq>"

	return XMPP;
}


/**********************************
 * MESSAGES - PROFILE - vCard
**********************************/
  
function MESSAGE_GetProfile(Username, Id)
{
	var XMPP = "";

	if (Id == null)
	{
		Id = MainData.Const.IQ_ID_GetProfile;
	}

	XMPP += "<iq xml:lang='"+UTILS_JabberLang(MainData.Lang)+"' type='get' to='"+Username+"@"+MainData.Host+"' id='"+Id+"'>";
	XMPP += "<vCard xmlns='vcard-temp' prodid='-//HandGen//NONSGML vGen v1.0//EN' version='2.0' />";
	XMPP += "</iq>";

	return XMPP;
}

function MESSAGE_SetProfile(Username, FullName, Desc, ImgType, Img64)
{
	var XMPP = "";
	XMPP += "<iq xml:lang='"+UTILS_JabberLang(MainData.Lang)+"' type='set' >";
	XMPP += "<vCard xmlns='vcard-temp' prodid='-//HandGen//NONSGML vGen v1.0//EN' version='2.0'>";
	XMPP += "<FN>"+FullName+"</FN>";
	XMPP += "<DESC>"+Desc+"</DESC>";
	XMPP += "<PHOTO>"
	XMPP += "<TYPE>"+ImgType+"</TYPE>"
	XMPP += "<BINVAL>"+Img64+"</BINVAL>"
	XMPP += "</PHOTO></vCard></iq>";

	return XMPP;
}

/**********************************
 * MESSAGES - SEARCH OLDGAME
 **********************************/

function MESSAGE_GetOldGames(Id,Jid1, Jid2, NumGames, Offset, Color, To, From)
{
	var XMPP = "";
	XMPP += "<iq xml:lang='"+UTILS_JabberLang(MainData.Lang)+"' type='get' id='"+MainData.Const.IQ_ID_OldGameSearch+"-"+Id+"' to='"+MainData.RatingComponent+"."+MainData.Host+"'>";
	XMPP += "<query xmlns='"+MainData.Xmlns+"/chessd#search_game'>";
	XMPP += "<search results='"+NumGames+"' offset='"+Offset+"'>";
	if (Jid1 != "")
	{
		if (Color != undefined && Color != "") 
		{
			XMPP += "<player jid='"+Jid1+"@"+MainData.Host+"' role='"+Color+"' />";
		}
		else
		{
			XMPP += "<player jid='"+Jid1+"@"+MainData.Host+"' />";
		}
	}
	if (Jid2 != "")
	{
		XMPP += "<player jid='"+Jid2+"@"+MainData.Host+"' />";
	}
	if (((To != undefined ) && ( To != "")) ||  ((From != undefined) && (From != "")))
	{
		XMPP += "<date ";
		if ((From != undefined ) && ( From != ""))
		{
			XMPP += "begin='"+From+"' ";	
		}
		if ((To != undefined ) && ( To != ""))
		{
			XMPP += "end='"+To+"' ";	
		}
		XMPP += "/>";
	}
	XMPP += "</search></query></iq>";

	return XMPP;
}

function MESSAGE_FetchOldGame(OldGameId)
{
	var XMPP = "";
	XMPP += "<iq xml:lang='"+UTILS_JabberLang(MainData.Lang)+"' to='"+MainData.RatingComponent+"."+MainData.Host+"' type='get' id='get_rating'>";
	XMPP += "<query xmlns='"+MainData.Xmlns+"/chessd#fetch_game'>";
	XMPP += "<game id='"+OldGameId+"'/>";
	XMPP += "</query></iq>";

	return XMPP;
}

/**********************************
 * MESSAGES - ANNOUNCE CHALLENGES
 **********************************/
//Player object
function MESSAGE_AnnounceMatch(Player, Rated, Category, Min, Max, Autoflag)
{
	var XMPP = "";

	XMPP += "<iq type='set' to='"+MainData.MatchComponent+"."+MainData.Host+"' id='"+MainData.Const.IQ_ID_AnnounceMatch+"'>";
	XMPP += "<create xmlns='http://c3sl.ufpr.br/chessd#match_announcement'>";
	XMPP += "<announcement rated='"+Rated+"' category='"+Category+"' autoflag='"+Autoflag+"' ";
	if (Min != "")
	{
		XMPP += "minimum_rating='"+Min+"' ";
	}
	if (Max != "")
	{
		XMPP += "maximum_rating='"+Max+"' ";
	}
	XMPP += ">";

	if(Player.Color == "")
	{
		XMPP += "<player jid='"+Player.Name+"@"+MainData.Host+"/"+MainData.Resource+"' time='"+Player.Time+"' inc='"+Player.Inc+"' />";
	}
	else
	{
		XMPP += "<player jid='"+Player.Name+"@"+MainData.Host+"/"+MainData.Resource+"' time='"+Player.Time+"' inc='"+Player.Inc+"' color='"+Player.Color+"'/>";
	}

	XMPP += "</announcement></create></iq>";

	return XMPP;
}

function MESSAGE_GetAnnounceMatch(Offset, NumResult, MinTime, MaxTime, Category, User)
{
	var XMPP = "";

	XMPP += "<iq type='set' to='"+MainData.MatchComponent+"."+MainData.Host+"' id='"+MainData.Const.IQ_ID_GetAnnounceMatch+"'>";
	XMPP += "<search xmlns='http://c3sl.ufpr.br/chessd#match_announcement'>";
	XMPP += "<parameters offset='"+Offset+"' results='"+NumResult+"' ";
	
	if(MinTime != "")
	{
		XMPP += "minimum_time='"+MinTime+"' ";
	}

	if(MaxTime != "")
	{
		XMPP += "maximum_time='"+MaxTime+"' ";
	}

	if(Category != "")
	{
		XMPP += "category='"+Category+"' ";
	}

	if (User == true)
	{
		XMPP += "player='"+MainData.Username+"@"+MainData.Host+"/"+MainData.Resource+"' />";
	}
	else
	{
		XMPP += "/>";
	}
	XMPP += "</search></iq>";

	return XMPP;
}

function MESSAGE_RemoveAnnounceMatch(Id)
{
	var XMPP = "";

	XMPP += "<iq type='set' to='"+MainData.MatchComponent+"."+MainData.Host+"' id='"+MainData.Const.IQ_ID_RemoveAnnounceMatch+"'>";
	XMPP += "<delete xmlns='http://c3sl.ufpr.br/chessd#match_announcement'>";
	XMPP += "<announcement id='"+Id+"'/>";
	XMPP += "</delete></iq>";

	return XMPP;
}

function MESSAGE_AcceptAnnounceMatch(Id)
{
	var XMPP = "";

	XMPP += "<iq type='set' to='"+MainData.MatchComponent+"."+MainData.Host+"' id='"+MainData.Const.IQ_ID_AcceptAnnounceMatch+"'>";
	XMPP += "<accept xmlns='http://c3sl.ufpr.br/chessd#match_announcement'>";
	XMPP += "<announcement id='"+Id+"'/>";
	XMPP += "</accept></iq>";

	return XMPP;
}

