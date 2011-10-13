#include "../gloox/client.h"
#include "../gloox/messagesessionhandler.h"
#include "../gloox/messageeventhandler.h"
#include "../gloox/messageeventfilter.h"
#include "../gloox/chatstatehandler.h"
#include "../gloox/chatstatefilter.h"
#include "../gloox/connectionlistener.h"
#include "../gloox/disco.h"
#include "../gloox/message.h"
#include "../gloox/gloox.h"
#include "../gloox/lastactivity.h"
#include "../gloox/loghandler.h"
#include "../gloox/logsink.h"
#include "../gloox/connectiontcpclient.h"
#include "../gloox/connectionsocks5proxy.h"
#include "../gloox/messagehandler.h"
#include "../gloox/connectionbosh.h"
#include "../gloox/connectionhttpproxy.h"
#include "../gloox/vcard.h"

#include "connboshms.h"

#include <algorithm>
using namespace gloox;

#include <unistd.h>
#include <stdio.h>
#include <string>

#include <cstdio> // [s]print[f]

#if defined( WIN32 ) || defined( _WIN32 )
# include <windows.h>
#endif

#include "../bosh.h"
#include "../../common.h"
#include "clientsample.h"

namespace gloox 
{

void ClientSample::handleStartNode() 
{
    MainData.Host = get_xmpp_hostname();
    MainData.Resource = "ChessD";
    MainData.RatingComponent = "chessd";
    MainData.MatchComponent = "chessd";
    MainData.Xmlns = "http://c3sl.ufpr.br";
    MainData.ChallengeSequence = 0;
//        m_current_status = HAVE_CONNECTED_STATE;
    m_handle_nr = 0;
    save_username(_g_RotName.c_str(), _g_RotName.length());
    send_message_username(_g_RotName.c_str());
    // the console will send the auth_1 message to bosh server.

}

bool ClientSample::handleNormalNode( Tag* tag )
{
    if( tag )
    {
        if(tag->name() == "iq")
        {
            std::string buffer = PARSER_ParseIq(tag);
    //      printf("PARSER_ParseIq:%s\n",buffer.c_str());
            if(!buffer.empty())
            {
                sendxml(buffer);
            }
        }
        else if(tag->name() == "presence")
        {
            std::string buffer = PARSER_ParsePresence(tag);
            if(!buffer.empty())
            {
                sendxml(buffer);
            }
        }
        else
        {
        }
    }
    return false;
}

void ClientSample::sendxml( const std::string& xml )
{
    if( m_connection && m_connection->state() == StateConnected )
    {

        m_connection->send( xml );

      logInstance().dbg( LogAreaXmlOutgoing, xml );
    }
}

std::string ClientSample::PARSER_ParsePresence(Tag *tag)
{
    std::string Jid, Show, NewStatus;
    std::string Buffer = "";

    // Get Jid
    if(!tag)
    {
        return "";
    }
    if(tag->hasAttribute("from"))
    {
        Jid = tag->findAttribute("from");
    }
    else
    {
        return Buffer;
    }
    int nfound = Jid.find("/");
    if(nfound != std::string::npos)
    {
    //get username
        std::string handle = Jid.substr(nfound+1);
        send_message_get_userinfo(handle);
    }
    return Buffer;

}
/**
* Parse XMPP received from jabber
*/

/**
* Receive and forward XML to controllers
*/
std::string  ClientSample::PARSER_ParseXml(Tag *tag)
{
#if 0
	var XMLTag, Body, i;
	var Buffer = "";


	// Find bind body
	if (XML.getElementsByTagName("body") != null)
	{
		Body = XML.getElementsByTagName("body")[0];
	}
	else 
	{
		return 0;
	}

	// If bind return a invalid sid
	if (Body.childNodes.length == 0)
	{
		if (Body.getAttribute("type") == "terminate")
		{
			MainData.ConnectionStatus = -1;
			LOGIN_Disconnected();
			alert(UTILS_GetText("error_disconnected"));

			START_Restart();
		}
	}

	// Verify all tags 
	for (i=0; i < Body.childNodes.length; i++)
	{
		XMLTag = Body.childNodes[i];

		switch(XMLTag.tagName)
		{
			case "undefined": 
				break;

			case "error": 
				Buffer += PARSER_ParseError(XMLTag);
				break;

			case "presence": 
				Buffer += PARSER_ParsePresence(XMLTag);
				break;

			case "message": 
				Buffer += PARSER_ParseChat(XMLTag);
				break;

			case "iq": 
				Buffer += PARSER_ParseIq(XMLTag);
				break;

			default: break;
		}
		/*
		if(Buffer.match("undefined"))
		{
			alert(XMLTag.tagName+"\n"+XMLTag.getAttribute("xmlns")+"\n"+Buffer);
		}
		*/
	}
	return Buffer;
#endif
}

std::string ClientSample::PARSER_ParseIq(Tag *tag)
{
    if(!tag)
        return "";
    //XML = iq tag
//    if(tag->xml()!=EmptyString)
//        printf("parser_parseiq,tag->xml=%s\n",tag->xml().c_str());

    std::string Type = tag->findAttribute("type");
    IQ::IqType iqtype;
//    std::string ID = tag->findAttribute("id");
    const TagList& l = tag->children();
    TagList::const_iterator it = l.begin();
    std::string Buffer = "";
    std::string Xmlns = "";

    for( ; it != l.end(); ++it )
    {
        Xmlns = (*it)->findAttribute("xmlns");
        break;
    }
    if(Xmlns.empty())
    {
    // This case should happen when user receive result from server.
    // Some messages come without iq child node in chess server
    // protocol.
    //    return "";
    }

    if(Type == "result")
        iqtype = IQ::Result;
    else if(Type == "set")
        iqtype = IQ::Set;
    else if(Type == "error")
        iqtype = IQ::Error;
    else
        iqtype = IQ::Invalid;
    switch (iqtype)
    {
        case IQ::Result:
            if(Xmlns.empty())
            {
                if(tag->hasAttribute("id", "auth_2"))
                {
                    Buffer += Login_handlePassword(tag);
                }            
            }
            else if(Xmlns.find("jabber:iq:auth")!=std::string::npos)
            {
                if(tag->hasAttribute("id", "auth_1"))
                {
                    Buffer += Login_HandleUserName(tag);
                }
            }
            // Receive information of user list
            else if (Xmlns.find("/chessd#info")!= std::string::npos)
            {
                // contact/info.js
                Buffer += CONTACT_HandleInfo(tag);
            }

            #if 0
        	// Receive user list
        	if (Xmlns.match(/jabber:iq:roster/))
        	{
        		Buffer += CONTACT_HandleUserList(XML);
        	}

        	// Receive room list
        	else if (Xmlns.match(/disco#items/))
        	{
        		Buffer += ROOM_HandleRoomList(XML);
        	}

        	// Receive room info -> used to running games
        	else if (Xmlns.match(/disco#info/))
        	{
        		Buffer += ROOM_HandleGameRoomInfoList(XML);
        	}


        	// Receive information of user list
        	else if (Xmlns.match(/\/chessd#info/))
        	{
        		Buffer += ADMIN_HandleInfo(XML);
        		// contact/info.js
        		Buffer += CONTACT_HandleInfo(XML);
        		Buffer += ROOM_HandleInfo(XML);
        	}

        	// Receive profile information of user
        	else if (Xmlns.match(/\/chessd#profile/))
        	{
        		Buffer += PROFILE_HandleInfoProfile(XML);
        	}
        	else if (Xmlns.match(/\/chessd#match_announcement/))
        	{
        		Buffer += ANNOUNCE_HandleAnnounce(XML);
        	}
        	// Challenge accept confirmation
        	else if (Xmlns.match(/\/chessd#match/))
        	{
        		Buffer += CHALLENGE_HandleChallenge(XML);
        	}
        	// Search user request
        	else if (Xmlns.match(/\/chessd#search_user/))
        	{
        		Buffer += CONTACT_HandleSearchUser(XML);
        	}
        	// Search Old Game
        	else if (Xmlns.match(/\/chessd#search_game/))
        	{
        		Buffer += OLDGAME_HandleSearchOldGame(XML);
        	}
        	else if (Xmlns.match(/\/chessd#fetch_game/))
        	{
        		Buffer += OLDGAME_FetchOldGame(XML);
        	}
        	else if (Xmlns.match(/\/chessd#adjourned#list/))
        	{
        		Buffer += CHALLENGE_HandleAdjourn(XML);
        	}
        	// Game messages
        	else if (Xmlns.match(/\/chessd#game/))
        	{
        		Buffer += GAME_HandleGameResult(XML);
        	}
        	else if (Xmlns.match(/vcard-temp/))
        	{
        		if (ID == MainData.Const.IQ_ID_GamePhoto)
        		{
        			Buffer += GAME_HandleVCardPhoto(XML);
        		}
        		else if (ID == MainData.Const.IQ_ID_OldGamePhoto)				{
        			Buffer += OLDGAME_HandleVCardPhoto(XML);
        		}
        		else
        		{
        			Buffer += PROFILE_HandleVCardProfile(XML);
        		}
        	}
        	else if (Xmlns.match(/\/chessd#admin/))
        	{
        		Buffer += ADMIN_HandleAdmin(XML);
        	}
        	else if (Xmlns == "")
        	{
        		Buffer += PARSER_ParseIqById(XML);
        	}
            #endif
        	break;

        case IQ::Set:
        	if (Xmlns.find("chessd#match_announcement")!=std::string::npos)
        	{
//        		Buffer += ANNOUNCE_HandleAnnounceGame(XML);
        	}
        	// Challenge messages
        	else if (Xmlns.find("chessd#match")!=std::string::npos)
        	{
//                printf("Ifoundachessdmatch info\n");
        		// Quick fix to avoid error when login in
        		// enviroment and has a pendent challenge;
        		try
        		{
        			Buffer += CHALLENGE_HandleChallenge(tag);
        		}
        		catch(...)
        		{
        		}

        	}
        	// Game messages
        	else if (Xmlns.find("chessd#game")!=std::string::npos)
        	{
        		Buffer += GAME_HandleGame(tag);
//            printf("GAME_HandleGame:%s\n",Buffer.c_str());

        	}
        	// Admin messages
        	else if (Xmlns.find("muc#admin")!=std::string::npos)
        	{
//        		Buffer += ADMIN_HandleRoomAdmin(XML);
        	}

        	// Admin notification
        	else if (Xmlns.find("chessd#admin")!=std::string::npos)
        	{
//        		Buffer += ADMIN_HandleUserNotification(XML);
        	}

        	// Search user request
        	else if (Xmlns.find("jabber:iq:roster")!=std::string::npos)
        	{
//        		Buffer += CONTACT_HandleSetSubscribe(XML);
        	}
        	break;

        case IQ::Error:
#if 0
        	if (Xmlns.match(/\/chessd#match_announcement/))
        	{
        		Buffer += ANNOUNCE_HandleAnnounceError(XML);
        	}
        	// Challenge messages
        	else if (Xmlns.match(/\/chessd#match/))
        	{
        		Buffer += CHALLENGE_HandleErrorChallenge(XML);
        	}
        	// Game messages
        	else if (Xmlns.match(/\/chessd#game/))
        	{
        		Buffer += GAME_HandleGameError(XML);
        	}
#endif
        	break;

        default: break;
    }

    return Buffer;

}



/**
* Handle Game Messages
*
* @param 	XML The xml that contains the string 'match' in xmlns attribute
* @return 	Buffer with the messages that must be send
* @author 	zhuzhenglu
*/
std::string ClientSample::GAME_HandleGame(Tag *tag)
{
    Tag *Query = NULL;
    std::string Xmlns="";
    std::string Buffer = "";
    const TagList& l = tag->children();
    TagList::const_iterator it = l.begin();
    for( ; it != l.end(); ++it )
    {
      if( (*it)->name() == "query" )
      {
        Query = *it;
      }
    }
    if(!Query)
        return "";

    // Getting query xmlns
    Xmlns = Query->findAttribute("xmlns");
    if(Xmlns==EmptyString)
        return "";


	if (Xmlns.find("chessd#game#state")!=std::string::npos)
	{
		Buffer += GAME_State(tag);

	}
	else if (Xmlns.find("chessd#game#move")!=std::string::npos)
	{
		Buffer += GAME_Move(tag);
	}
#if 0
	else if (Xmlns.match(/\/chessd#game#canceled/))
	{
		Buffer += GAME_End(XML);
	}
	else if (Xmlns.match(/\/chessd#game#end/))
	{
		Buffer += GAME_End(XML);
	}
	else if (Xmlns.match(/\/chessd#game#cancel/))
	{
		Buffer += GAME_HandleCancel(XML, Xmlns);
	}
	else if (Xmlns.match(/\/chessd#game#draw/))
	{
		Buffer += GAME_HandleDraw (XML, Xmlns);
	}
	else if (Xmlns.match(/\/chessd#game#adjourn/))
	{
		Buffer += GAME_HandleAdjourn(XML, Xmlns);
	}
#endif
	return Buffer ;
}
/**
 * @brief	Handle challenge messages from server
 *
 * @param	XML	XML with challenge parameters
 * @return	Buffer with XMPP to send
 * @author	Ulysses Bomfim
 */
std::string ClientSample::CHALLENGE_HandleChallenge (Tag *tag)
{
    Tag *Query = NULL;
    std::string Xmlns="";
    std::string Buffer = "";
    const TagList& l = tag->children();
    TagList::const_iterator it = l.begin();
    for( ; it != l.end(); ++it )
    {
      if( (*it)->name() == "query" )
      {
        Query = *it;
      }
    }
    if(!Query)
        return "";

    // Getting query xmlns
    Xmlns = Query->findAttribute("xmlns");
    if(Xmlns==EmptyString)
        return "";

//    if (Xmlns.match(/\/chessd#match#offer/))
//    {
//    	Buffer = CHALLENGE_HandleOffer(XML);
//    }
    if (Xmlns.find("chessd#match#accept")!=std::string::npos)
    {
    	Buffer = CHALLENGE_HandleAccept(tag);
    }
#if 0
    else if (Xmlns.match(/\/chessd#match#decline/))
    {
    	Buffer = CHALLENGE_HandleDecline(XML);
    }
    else if (Xmlns.match(/\/chessd#match#error/))
    {
    	Buffer = CHALLENGE_ChallengeError(XML);
    }
#endif
    return Buffer;
}

std::string ClientSample::Login_handlePassword(Tag *tag)
{

    SendMessage(hwndMain, WM_BOSH_Input, 0, ACK_HAVE_LOGGEDIN);

    return "";
}

std::string ClientSample::Login_HandleUserName(Tag *tag)
{

    std::string result;
    result = "<query xmlns='jabber:iq:auth'><username>";
    result += _g_RotName;
    result +="</username><password/><resource/></query></iq>"; 
    std::string tmp = tag->xml();

    if( tmp.find(result)!= std::string::npos)
    {
      send_message_password(_g_RotName.c_str());
    }
    else
    {
    }

    return "";
}

/**
* Receive a info message and set it in user list
*/

std::string ClientSample::CONTACT_HandleInfo(Tag *tag)
{
    Tag *Rating = NULL;
    Tag *Query = NULL;
    Tag *UserTypeTag = NULL;

    std::string Buffer = "";
    const TagList& l = tag->children();
    TagList::const_iterator it = l.begin();
    for( ; it != l.end(); ++it )
    {
      if( (*it)->name() == "query" )
      {
        Query = *it;
        break;
      }
    }
    if(Query)
    {
        const TagList& lsub = Query->children();
        TagList::const_iterator itsub = lsub.begin();
        for( ; itsub != lsub.end(); ++itsub )
        {
          if( (*itsub)->name() == "rating" )
          {
            Rating = *itsub;
          }
          else if( (*itsub)->name() == "type")
          {
            UserTypeTag = *itsub;
          }
        }
        if(UserTypeTag)
        {
            std::string jid = UserTypeTag->findAttribute("jid");
            std::string simplejid = jid;
            if(!jid.empty())
            {
                int nfound = jid.find("@");
                simplejid = jid.substr(0, nfound);
                // ignore myself
                if(MainData.Username != simplejid)
                {
                    if(!Rating)
                    {
                        //unrated
                        m_handle_nr = m_handle_nr + 1;
                        char str_nr[255];
                        sprintf(str_nr,"%d", m_handle_nr);
                        std::string stdstr_nr = str_nr;
                /*      nr, rating, handle, base, inc, rated?, type */
                        std::string composite=stdstr_nr+" "+"0"+" "+simplejid+" 1 0 unrated normal";
                        char *line;
                        line = (char *)malloc(composite.size()+1);
                        strcpy(line, composite.c_str());
                        //note: you must delete the memory in the message.
                        MatchSoughtLine(line);
                        free(line);
                        SendMessage(hwndMain, WM_BOSH_Input, (WPARAM)NULL, REQUEST_UPDATE_SEEKGRAPH);
                    }
                    else
                    {
                        std::string rating = Rating->findAttribute("rating");
                        std::string category = Rating->findAttribute("category");

                        m_handle_nr = m_handle_nr + 1;
                        char str_nr[255];
                        sprintf(str_nr,"%d", m_handle_nr);
                        std::string stdstr_nr = str_nr;
                        std::string base;
                        if(category == "standard")
                            base = "11";
                            
                /*      nr, rating, handle, base, inc, rated?, type */
                        std::string composite=stdstr_nr+" "+rating+" "+simplejid+" "+base+" 0 rated "+category;
                        char *line;
                        line = (char *)malloc(composite.size()+1);
                        strcpy(line, composite.c_str());
                        MatchSoughtLine(line);
                        free(line);
                       //note: you must delete the memory in the message.
                        SendMessage(hwndMain, WM_BOSH_Input, (WPARAM)NULL, REQUEST_UPDATE_SEEKGRAPH);
                    }
                }
            }
           
        }


    }
    // Try to get the Match tag

    return "";
}

/**
 * @brief	Parse challenge accept
 *
 * @param	XML	XML with challenge parameters
 * @return	Buffer with XMPP to send
 * @author	Ulysses Bomfim
 */
std::string ClientSample::CHALLENGE_HandleAccept (Tag *tag)
{
    std::string GameRoom;
    Tag *MatchTag = NULL;
    Tag *Query = NULL;

    std::string Xmlns="";
    std::string Buffer = "";
    const TagList& l = tag->children();
    TagList::const_iterator it = l.begin();
    for( ; it != l.end(); ++it )
    {
      if( (*it)->name() == "query" )
      {
        Query = *it;
        break;
      }
    }
    if(!Query)
        return "";
    // Try to get the Match tag

    const TagList& qsubl = Query->children();
    TagList::const_iterator itsub = qsubl.begin();
    for( ; itsub != qsubl.end(); ++itsub )
    {
      if( (*itsub)->name() == "match" )
      {
        MatchTag= *itsub;
        break;
      }
    }
    if(!MatchTag)
        return "";

	// Get the game room name
	GameRoom = MatchTag->findAttribute("room");
     std::string ChallengeID = MatchTag->findAttribute("id");
     int id = atoi(ChallengeID.c_str());
     std::map<int,std::string>::iterator itTime = _g_mapTime.find(id);
     if(itTime!=_g_mapTime.end())
     {
        _g_current_TimeSet = itTime->second;
     }
	// Remove all challanges on structure
	//MainData.ClearChallenges();
//    CHALLENGE_ClearChallenges();

	// TODO
	// Warn the player's interface

	// Send a presence to GameRoom with playing status
	Buffer += MESSAGE_ChangeStatus("playing",GameRoom);

	return Buffer;
}

void ClientSample::send_message_username(const char * username)
{
        std::string hostname = get_xmpp_hostname();
        JID componentjid( hostname.c_str() );
        IQ iq(IQ::Get, componentjid,"auth_1");
        Query *q = new Query( username );
        iq.addExtension( q );
        send(iq);
//        m_current_status = VERIFYING_USERNAME_STATE;

}

void ClientSample::save_username(const char *username,int count)
{
    m_saved_username = username;
    MainData.Username = username;
}

void ClientSample::send_message_password(const char * password)
{
   //send auth_2;
    std::string hostname = get_xmpp_hostname();
    JID componentjid( hostname.c_str() );
    IQ iq(IQ::Set, componentjid,"auth_2");
    Tag* q = new Tag( "query" );
    q->setXmlns( XMLNS_AUTH );
    new Tag( q, "username" );
    new Tag( q, "password" );
    new Tag( q, "resource" );
    Query n( q );
    Query* nq = n.newInstance( m_saved_username, "sid", password, "ChessD" );

    iq.addExtension( nq );
    send(iq);
//    m_current_status=VERIFYING_PASSWORD_STATE;
}

void ClientSample::send_message_get_myprofile()
{
    std::string hostname = get_xmpp_hostname();
    std::string chessdname ="chessd."+hostname ;
    JID componentjid( chessdname);
    /*FIXME: should remove a "to" clause from IQ*/
    IQ iqUserList(IQ::Get, componentjid,"GetUserList");
    
    Tag* t = new Tag( "query" );
    t->setXmlns( XMLNS_ROSTER );
    iqUserList.addExtension(  new Query( t ) );

    Presence pres(Presence::Available,componentjid);

    /*FIXME: remove a to option from IQ*/

    IQ iqProfile(IQ::Get, componentjid,"GetMyProfile");

    iqProfile.addExtension( new VCard() );

    /*FIXME: remove a to option from pres2*/
    Presence pres2(Presence::Available,componentjid);
    JID generaljid( "general@conference."+hostname+"/"+_g_RotName);
    Presence pres3(Presence::Available,generaljid);

    std::string xml=iqUserList.tag()->xml()
                    +pres.tag()->xml()
                    +iqProfile.tag()->xml()
                    +pres2.tag()->xml()
                    +pres3.tag()->xml();
    sendxml(xml);
}

void ClientSample::send_message_challenge(char *Oponent, char *Color, char *TimeStr, char *IncStr, char *Category, int Rated)
{
    std::string XMPP = "";

    // Set ChallengeID, this id is used temporary to identify challenge
    // in challenge list when match id is not defined yet
    std::string ChallengeID = "Challenge_";
    char id[1024]={0};
    sprintf(id,"%d",MainData.ChallengeSequence);
    ChallengeID += id;


    // Tag the id with the challenged player's name
    XMPP  = "<iq type='set' to='"+MainData.MatchComponent+"."+MainData.Host+"' id='"+ChallengeID+"'>";
    XMPP += "<query xmlns='"+MainData.Xmlns+"/chessd#match#offer'>";

    // Game offer
    XMPP += "<match category='";
    XMPP += Category;
    if(Rated==1)
        XMPP += "' rated='1' >";
    else
        XMPP += "' rated='0' >";

    // Setting attributes
    std::string NameArr[2];
    std::string ColorArr[2];
    std::string CurrentInc = IncStr;
    std::string CurrentTime;
    NameArr[0] = MainData.Username;
    NameArr[1] = Oponent;
    ColorArr[0] = Color;
     
    // Setting oponent's color
    if (strcmp(Color,"white") == 0)
    {
        ColorArr[1] = "black";
    }
    else if (strcmp(Color, "black") == 0)
    {
        ColorArr[1] = "white";
    }
    else // random color 
    {
    }

    // Convert time in seconds
    if(TimeStr != "untimed")
    {
        int Time;
        sscanf(TimeStr,"%d",&Time);
        Time *= 60;
        char NewTimeStr[1024]={0};
        sprintf(NewTimeStr,"%d",Time);
        CurrentTime = NewTimeStr;
    }
    else
    {
        CurrentTime = TimeStr;
    }
    // Creating players tags
    for (int i=0; i < 2; i++)
    {
        // If player color was chose
        if (Color[0] != 0)
        {
            XMPP += "<player jid='"+NameArr[i]+"@"+MainData.Host+"/"+MainData.Resource+"' time='"+CurrentTime+"' inc='"+CurrentInc+"' color='"+ColorArr[i]+"' />";
        }
        else
        {
            XMPP += "<player jid='"+NameArr[i]+"@"+MainData.Host+"/"+MainData.Resource+"' time='"+CurrentTime+"' inc='"+CurrentInc+"' />";
        }
    }
    XMPP += "</match></query></iq>";
    _g_mapTime.insert(std::pair<int,std::string>(MainData.ChallengeSequence,CurrentTime+":"+CurrentInc));
    MainData.ChallengeSequence++;

    sendxml(XMPP);
}

void ClientSample::send_message_get_userinfo(std::string str_user)
{
    std::string XMPP;

    XMPP  = "<iq type='get' from='"+MainData.Username+"@"+MainData.Host+"/"+MainData.Resource+"' to='"+MainData.RatingComponent+"."+MainData.Host+"' id='"+IQ_ID_GetRating+"'>";
    XMPP += "<query xmlns='"+MainData.Xmlns+"/chessd#info'>";
    XMPP += "<rating jid='"+str_user+"@"+MainData.Host+"' />";
    XMPP += "<type jid='"+str_user+"@"+MainData.Host+"' />";
    XMPP += "</query></iq>";

    sendxml(XMPP);

}


void ClientSample::send_message_search_user()
{

}
void ClientSample::send_message_mymove( std::string movestr)
{
    std::string Buffer;
    std::string hostname = get_xmpp_hostname();
    Buffer = "<iq xml:lang='zh-cn' type='set' to='";
    Buffer += g_gameID;
    Buffer += "@chessd.";
    Buffer += hostname;
    Buffer += "' id='GameMove'><query xmlns='http://c3sl.ufpr.br/chessd#game#move'><move long='";
    Buffer += movestr;
    Buffer += "'></move></query></iq>";
    sendxml(Buffer);

}

}
