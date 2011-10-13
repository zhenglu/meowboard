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

extern HWND hwndMain ;
extern "C" void set_board_style(char to_play, int double_push,
                     int castle_ws, int castle_wl,
                     int castle_bs, int castle_bl,
                     int irrev_count,int gamenum,
                     char *white, char *black,
                     int relation,int basetime,
                     int increment,int white_stren,
                     int black_stren,int white_time,
                     int black_time, int moveNum,
                     char *str, char *elapsed_time,
                     char *move_str, int ics_flip,
                     int ticking, char *xmppboard_chars);
extern "C" void set_gamemode(int mode);
extern "C" void set_seekGraphUp(int flag);
extern "C" void DrawPosition(int fullRedraw, Board board);
extern "C" void set_gs_gamenum(int gamenum);
extern "C" void set_gs_kind(char * why);
extern "C" VOID EchoOn();
extern "C" Boolean MatchSoughtLine(char *line);
extern "C" void StopClocks();

extern "C" char * get_xmpp_hostname();

extern int compute_elapsed_time(char who, int current_time_remain, char *str);
extern int set_remaining_time(char who, int current_time_remain);
extern void convert_base_incr(int *base, int *inc);


const std::string XMLNS_XQ_OFFER="http://c3sl.ufpr.br/chessd#match#offer";
const std::string XMLNS_XQ_ACCEPT="http://c3sl.ufpr.br/chessd#match#accept";
std::string _g_RotName;
std::string _g_RotPwd;
int g_clientstatus = WAITING_FOR_USERNAME_STATE ;

FILE * _g_logfile = NULL;
typedef struct tagPlayer
{
    std::string Name;
    std::string Inc;
    std::string Color;
    std::string Time;
} Player_t;

/**
* Global object that stores all data needed
* by interface
*/
typedef struct tagDataEnv
{
    std::string Lang;
    std::string Username;
    std::string Host;
    std::string Resource;
    std::string RatingComponent;
    std::string MatchComponent;
    std::string Xmlns;
    int ChallengeSequence;
} DataEnv_t;

DataEnv_t MainData;

std::string IQ_ID_GetRating = "GetRating";

std::map<int,std::string> _g_mapTime;
std::string _g_current_TimeSet;
/**
* @brief An abstraction of an IQ extension used for Non-SASL authentication (XEP-0078).
*
* @author Jakob Schroeter <js@camaya.net>
* @since 1.0
*/
class Query : public StanzaExtension
{
public:
  /**
   * Creates a new object that can be used to query the server for
   * authentication filds for the given user.
   * @param user The user name to fetch authentication fields for.
   */
  Query( const std::string& user );

  /**
   * Creates a now object from the given Tag.
   * @param tag The Tag to parse.
   */
  Query( const Tag* tag = 0 );

  /**
   * Creates a new object on the heap that can be used to
   * authenticate, based on the current reply.
   * @param user The uset o authenticate as.
   * @param sid The stream's ID.
   * @param pwd The password to use.
   * @param resource The desired resource identifier.
   */
  Query* newInstance( const std::string& user, const std::string& sid,
                      const std::string& pwd, const std::string& resource ) const;

  /**
   * Virtual destructor.
   */
  virtual ~Query() {}

  // reimplemented from StanzaExtension
  virtual const std::string& filterString() const;

  // reimplemented from StanzaExtension
  virtual Query* newInstance( const Tag* tag ) const
  {
    return new Query( tag );
  }

  // reimplemented from StanzaExtension
  virtual Tag* tag() const;

  // reimplemented from StanzaExtension
  virtual StanzaExtension* clone() const
  {
    return new Query( *this );
  }

private:
  std::string m_user;
  std::string m_pwd;
  std::string m_resource;
  bool m_digest;

};


class ClientSample : public Client
{
  public:
    ClientSample( const JID& jid, const std::string& password, int port = -1 )
      : Client( jid, password, port ),m_current_status(INITIAL_CLIENT_STATE)
    {
    }
    virtual ~ClientSample() {}
    virtual void handleStartNode() 
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
    virtual bool handleNormalNode( Tag* tag )
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


    int get_status(){return m_current_status;}
    void set_status(int status){m_current_status=status;}
    std::string PARSER_ParseIq(Tag *tag);
    std::string PARSER_ParseXml(Tag *tag);
    std::string PARSER_ParsePresence(Tag *tag);
    std::string GAME_HandleGame(Tag *tag);
    std::string CHALLENGE_HandleChallenge(Tag *tag);
    std::string CHALLENGE_HandleAccept (Tag *tag);
    std::string Login_HandleUserName(Tag *tag);
    std::string Login_handlePassword(Tag *tag);
    std::string CONTACT_HandleInfo(Tag *tag);
    void send_message_username(const char *username);
    void send_message_password(const char * password);
    void send_message_get_myprofile();
    void send_message_challenge(char *Oponent, char *Color, char *TimeStr, char *IncStr, char *Category, int Rated);
    void send_message_get_userinfo(std::string username);
    void send_message_search_user();

    void send_message_mymove(std::string movestr);
    void save_username(const char * username, int count);
    std::string get_username()
    {
        return m_saved_username;
    }
  private:
    std::string m_saved_username;
    int m_handle_nr;
    int m_current_status;
    void sendxml( const std::string& xml )
    {
        if( m_connection && m_connection->state() == StateConnected )
        {

            m_connection->send( xml );

          logInstance().dbg( LogAreaXmlOutgoing, xml );
        }
    }

};


Query::Query( const std::string& user )
  : StanzaExtension( ExtNonSaslAuth ), m_user( user ), m_digest( true )
{
}

Query::Query( const Tag* tag )
  : StanzaExtension( ExtNonSaslAuth )
{
  if( !tag || tag->name() != "query" || tag->xmlns() != XMLNS_AUTH )
    return;

  m_digest = tag->hasChild( "digest" );
}

Query* Query::newInstance( const std::string& user,
                                                     const std::string& sid,
                                                     const std::string& pwd,
                                                     const std::string& resource ) const
{
  Query* q = new Query( user );
  if( m_digest && !sid.empty() )
  {
//FIXME
//    SHA sha;
//    sha.feed( sid );
//    sha.feed( pwd );
//    q->m_pwd = sha.hex();
  }
  else
    q->m_pwd = pwd;

  q->m_resource = resource;
  q->m_digest = m_digest;
  return q;
}

const std::string& Query::filterString() const
{
  static const std::string filter = "/iq/query[@xmlns='" + XMLNS_AUTH + "']";
  return filter;
}

Tag* Query::tag() const
{
  if( m_user.empty() )
    return 0;

  Tag* t = new Tag( "query" );
  t->setXmlns( XMLNS_AUTH );
  new Tag( t, "username", m_user );

  if( !m_pwd.empty() && !m_resource.empty() )
  {
    new Tag( t, m_digest ? "digest" : "password", m_pwd );
    new Tag( t, "resource", m_resource );
  }

  return t;
}

class MessageTest : public MessageSessionHandler, ConnectionListener, LogHandler,
                    MessageEventHandler, MessageHandler, ChatStateHandler
{
  public:
    MessageTest() : m_session( 0 ), m_messageEventFilter( 0 ), m_chatStateFilter( 0 ) {}

    ~MessageTest() {}
    void log_set_file(const char* file_name) {
        _g_logfile = fopen(file_name, "aw");
        if(_g_logfile == NULL) {
            fprintf(stderr, "Could not open %s for logging\n", file_name);
            fprintf(stderr, "Switching log output to standard output.\n");
            _g_logfile = stderr;
        }
    }

    void start()
    {
      std::string hostname = get_xmpp_hostname();
      JID jid( _g_RotName+"@"+hostname+"/ChessD" );
      log_set_file("log/robot.log");

//password,
//port:?
      j = new ClientSample( jid, _g_RotPwd );
      j->registerConnectionListener( this );
      j->registerMessageSessionHandler( this, 0 );
      j->disco()->setIdentity( "client", "bot" );
      j->disco()->addFeature( XMLNS_CHAT_STATES );
      j->setCompression(false);
      //StringList ca;
      //ca.push_back( "/path/to/cacert.crt" );
      //j->setCACerts( ca );

      j->logInstance().registerLogHandler( LogLevelDebug, LogAreaAll, this );

      ConnectionTCPClient* conn0 = new ConnectionTCPClient( j->logInstance(), hostname, 8082 );
//      ConnectionHTTPProxy* conn1 = new ConnectionHTTPProxy( conn0, j->logInstance(), "www.meowchess.com", 8082 );
      ConnBoshMultStat* conn2 = new ConnBoshMultStat( j, conn0, j->logInstance(), hostname, hostname );
//      conn2->setMode( ConnectionBOSH::ModeLegacyHTTP );
        conn2->setMode( ConnBoshMultStat::ModePersistentHTTP );
      j->setConnectionImpl( conn2 );

//      FakeClientBase* fcb = new FakeClientBase( conn2 );

      // this code connects to a jabber server through a BOSH connection...
//      ConnectionTCPClient* conn0 = new ConnectionTCPClient( j->logInstance(), "www.meowchess.com", 80 );
 //     ConnectionBOSH* conn1 = new ConnectionBOSH( j, conn0, j->logInstance(), "www.meowchess.com", "www.meowchess.com" );
//       conn1->setMode( ConnectionBOSH::ModeLegacyHTTP );
//        conn1->setMode( ConnectionBOSH::ModePersistentHTTP );
//      j->setConnectionImpl( conn0 );

      j->setForceNonSasl( true );


      if( j->connect( false ) )
      {
        ConnectionError ce = ConnNoError;
        while( ce == ConnNoError )
        {
            if(g_clientstatus == WAITING_FOR_USERNAME_STATE)
            {
                break;
            }
            ce = j->recv();
        }
        printf( "ce: %d\n", ce );
      }
      delete( j );
    }

    void onConnect()
    {
      printf( "connected!!!\n" );
    }

    void onDisconnect( ConnectionError e )
    {
      g_clientstatus = WAITING_FOR_USERNAME_STATE;
      printf( "message_test: disconnected: %d\n", e );
      if( e == ConnAuthenticationFailed )
        printf( "auth failed. reason: %d\n", j->authError() );
    }

    bool onTLSConnect( const CertInfo& info )
    {
      time_t from( info.date_from );
      time_t to( info.date_to );

      printf( "status: %d\nissuer: %s\npeer: %s\nprotocol: %s\nmac: %s\ncipher: %s\ncompression: %s\n"
              "from: %s\nto: %s\n",
              info.status, info.issuer.c_str(), info.server.c_str(),
              info.protocol.c_str(), info.mac.c_str(), info.cipher.c_str(),
              info.compression.c_str(), ctime( &from ), ctime( &to ) );
      return true;
    }

    void handleMessage( const Message& msg, MessageSession * /*session*/ )
    {
      printf( "type: %d, subject: %s, message: %s, thread id: %s\n", msg.subtype(),
              msg.subject().c_str(), msg.body().c_str(), msg.thread().c_str() );

      std::string re = "You said:\n> " + msg.body() + "\nI like that statement.";
      std::string sub;
      if( !msg.subject().empty() )
        sub = "Re: " +  msg.subject();

      m_messageEventFilter->raiseMessageEvent( MessageEventDisplayed );
      m_messageEventFilter->raiseMessageEvent( MessageEventComposing );
      m_chatStateFilter->setChatState( ChatStateComposing );
      m_session->send( re, sub );

      if( msg.body() == "quit" )
        j->disconnect();
    }

    void handleMessageEvent( const JID& from, MessageEventType event )
    {
      printf( "received event: %d from: %s\n", event, from.full().c_str() );
    }

    void handleChatState( const JID& from, ChatStateType state )
    {
      printf( "received state: %d from: %s\n", state, from.full().c_str() );
    }

    void handleMessageSession( MessageSession *session )
    {
      printf( "got new session\n");
      // this example can handle only one session. so we get rid of the old session
      j->disposeMessageSession( m_session );
      m_session = session;
      m_session->registerMessageHandler( this );
      m_messageEventFilter = new MessageEventFilter( m_session );
      m_messageEventFilter->registerMessageEventHandler( this );
      m_chatStateFilter = new ChatStateFilter( m_session );
      m_chatStateFilter->registerChatStateHandler( this );
    }

    void handleLog( LogLevel level, LogArea area, const std::string& message )
    {
        char time_str[256];
        time_t t;
        /* check output file */
        if(_g_logfile == NULL) {
          fprintf(stderr, "Log output not set.\n");
          _g_logfile = stderr;
        }
        /* create timestamp */
        t = time(NULL);
        strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", localtime(&t));

        fprintf(_g_logfile, "%s: ", time_str );
        switch(area)
        {
          case LogAreaXmlIncoming:
           fprintf(_g_logfile,"Received XML: ");
           break;
          case LogAreaXmlOutgoing:
           fprintf(_g_logfile,"Sent XML: ");
           break;
          case LogAreaClassConnectionBOSH:
#ifdef DEBUG_BOSH
           fprintf(_g_logfile,"BOSH: ");
#endif
           break;
          case LogAreaClassClientbase:
           fprintf(_g_logfile,"Clientbase: ");
           break;
          case LogAreaClassConnectionTCPClient:
           fprintf(_g_logfile,"TCPClient:");
           break;
          default:
               fprintf(_g_logfile,"log: level: %d, area: %d, ", level, area);
        }
          fprintf(_g_logfile,"%s\n", message.c_str() );
    }
    ClientSample * get_client()
    {
        return j;
    }
  private:
    ClientSample *j;
    MessageSession *m_session;
    MessageEventFilter *m_messageEventFilter;
    ChatStateFilter *m_chatStateFilter;
};
extern void bitstronger_exit();
extern void bitstronger_init();
extern void bitstronger_setposition(std::string fen, char whoseturn);
extern std::string bitstronger_move();
/**
* @brief		Find game in 'GameList' by game id
* @param		Id 	  	   Game Id
* @author 		Rubens Sugimoto
* @return 		Game index
* @see			DATA_FindGame DATA_SetCurrentGame
*/
std::string g_gameID;
int DATA_FindGame(std::string ID)
{
    if(ID == g_gameID)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/**
* @brief		Add a game in 'GameList'
* @param		Id 	  	   Game Id
* @param		Player1	   Player name
* @param		Player2	   Player name
* @param		Color	   Your color on game
* @param		GameDiv	   Interface information
* @author 		Rubens Sugimoto
* @return 		New Game structure
* @see			DATA_SetCurrentGame
*/
void DATA_AddGame(std::string Id, Player_t Player1, Player_t Player2, std::string Color)
{
    g_gameID = Id;
}


/**
* Change Status
*/
std::string MESSAGE_ChangeStatus(std::string NewStatus, std::string RoomName)
{
	std::string XMPP;

	// Message to room
	if (RoomName!=EmptyString)
	{
		if (NewStatus == "available")
		{
//			XMPP = "<presence to='"+RoomName+"/"+MainData.Username+"' />";
		}
		else
		{
			XMPP = "<presence to='"+RoomName+"/"+_g_RotName+"' ><show>"+NewStatus+"</show></presence>";
		}
	}

	// General status change
	else
	{
		if (NewStatus == "available")
		{
//			XMPP = "<presence xml:lang='"+UTILS_JabberLang(MainData.Lang)+"' xmlns='jabber:client' />";
		}
		else
		{
//			XMPP = "<presence xml:lang='"+UTILS_JabberLang(MainData.Lang)+"' xmlns='jabber:client'><show>"+NewStatus+"</show></presence>";
		}
	}
	return XMPP;
}


/**
* Start Game
*
* @param 	GameId = Game number
* @param 	P1 = Player 1 Object (Name, Time, Color, Inc)
* @param 	P2 = Player 2 Object (Name, Time, Color, Inc)
* @return 	void
* @author 	Rubens
*/
void GAME_StartGame(std::string GameId, Player_t P1, Player_t P2)
{
    std::string YourColor;
	if (P1.Name == _g_RotName)
	{
		YourColor = P1.Color;
	}
	else
	{
		YourColor = P2.Color;
	}

	// Add game to data struct and set it to current game
	DATA_AddGame(GameId, P1, P2, YourColor);
     char *p = (char *)malloc(YourColor.length()+1);
     strcpy(p,YourColor.c_str());
/*     SendMessage(hwndMain, WM_BOSH_Input, (WPARAM)p, REQUEST_FRONTEND_START_CHALLENGE);*/
     if(strcmp(p,"white") == 0)
     {
         set_gamemode(IcsPlayingWhite);
     }
     else if(strcmp(p,"black") == 0)
     {
         set_gamemode(IcsPlayingBlack);
     }
     else
     {
         // wrong?
     }
     free(p);
     /* end sought*/
     set_seekGraphUp(FALSE);
     DrawPosition(TRUE, NULL);
     /*cut "game_" */
     int n = atoi(GameId.substr(5).c_str());
     set_gs_gamenum(n);
     set_gs_kind("xiangqi");
/*     set_board_style();*/
     return;

}


/**
* Handle Game Move
* It's a good ideia to read the server's documentation before reading the code above
*
* @param 	XML The xml that contains the game move
* @return 	void
* @author 	Ulysses
*/
std::string GAME_Move(Tag *tag)
{
    std::string GameID;
    Tag *StatusPlayerTag[2];
    Tag *Query=NULL;
    Tag *CurrStatusTag=NULL;
    Tag *MoveTag = NULL;
    Player_t Player1, Player2;
    std::string Buffer = "";

    StatusPlayerTag[0]=NULL;
    StatusPlayerTag[1]=NULL;
//game_room
//printf("youve entered this gameroom\n");
    std::string from = tag->findAttribute("from");
    if(!from.empty())
    {
        size_t nfound = from.find("@");
        if(nfound!=std::string::npos)
        {
            GameID = from.substr(0,nfound);
        }
    }
//query tag
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
//        printf("NewIamhere\n");

    // Try to get the current state tag

    const TagList& cur_state_list = Query->children();
    TagList::const_iterator it_cur_state = cur_state_list.begin();
    for( ; it_cur_state != cur_state_list.end(); ++it_cur_state )
    {
      if( (*it_cur_state)->name() == "state" )
      {
        CurrStatusTag= *it_cur_state;
        break;
      }
    }
    if(!CurrStatusTag)
        return "";

    it_cur_state = cur_state_list.begin();
    for( ; it_cur_state != cur_state_list.end(); ++it_cur_state )
    {
      if( (*it_cur_state)->name() == "move" )
      {
        MoveTag= *it_cur_state;
        break;
      }
    }
    // Get players 
    int i_stateplayer_num = 0;
    const TagList& statusl = CurrStatusTag->children();
    TagList::const_iterator it_stat = statusl.begin();
    for( ; it_stat != statusl.end(); ++it_stat )
    {
      if( (*it_stat)->name() == "player" )
      {
        StatusPlayerTag[i_stateplayer_num++]= *it_stat;
        if(i_stateplayer_num>1)
        {
            break;
        }
      }
    }
//        printf("num=%d\n",i_stateplayer_num);
    if(!StatusPlayerTag[0])
        return "";
    if(!StatusPlayerTag[1])
        return "";


    JID jid_p1(StatusPlayerTag[0]->findAttribute("jid") );

    Player1.Name = jid_p1.username();
//        printf("jid_p1=%s\n",Player1.Name.c_str());
    Player1.Inc = StatusPlayerTag[0]->findAttribute("inc");
    Player1.Color = StatusPlayerTag[0]->findAttribute("color");
    Player1.Time = StatusPlayerTag[0]->findAttribute("time");
    
    JID jid_p2(StatusPlayerTag[1]->findAttribute("jid") );

    Player2.Name = jid_p2.username();
    Player2.Inc = StatusPlayerTag[1]->findAttribute("inc");
    Player2.Color = StatusPlayerTag[1]->findAttribute("color");
    Player2.Time = StatusPlayerTag[1]->findAttribute("time");

    if (DATA_FindGame(GameID) == 0)// you should start it now...
    {
        GAME_StartGame(GameID, Player1, Player2);
    }
//AI moves.
//AI color
    std::string ai_color;
    if (Player1.Name == _g_RotName)
    {
        ai_color = Player1.Color;
    }
    else
    {
        ai_color = Player2.Color;
    }
    char whoseturn;
    if(ai_color == "white")
        whoseturn = 'w';
    else
        whoseturn = 'b';
    
    Tag *BoardTag=NULL;
//                printf("I am here\n");
//                const TagList& statusl = CurrStatusTag->children();
    TagList::const_iterator it_stat_2 = statusl.begin();
    for( ; it_stat_2 != statusl.end(); ++it_stat_2 )
    {
      if( (*it_stat_2)->name() == "board" )
      {
        BoardTag= *it_stat_2;
      }
    }
    if(!BoardTag)
        return "";
    std::string fen = BoardTag->findAttribute("state");
    
#ifdef USE_BITSTRONGER    
    bitstronger_setposition(fen,whoseturn);
//    printf("move:aiqin is now calculating:%s\n",ai_color.c_str());
    std::string ai_move = bitstronger_move();
//                          printf("substr=%s",ai_move.substr(0,9).c_str());
    if(ai_move == "nobestmove")
    {
        //Buffer = "";
    }
    else if(ai_move.substr(0,9)== "bestmove ")
    {
        std::string hostname = get_xmpp_hostname();
        Buffer = "<iq xml:lang='zh-cn' type='set' to='";
        Buffer += GameID;
        Buffer += "@chessd.";
        Buffer += hostname;
        Buffer += "' id='GameMove'><query xmlns='http://c3sl.ufpr.br/chessd#game#move'><move long='";
        Buffer += ai_move.substr(9);
        Buffer += "'></move></query></iq>";
    }
    else
    {
        //do nothing?
    }
#endif    

#if 0
    char *sendstr = (char *)malloc(20000);
    strcpy(sendstr,
        "rheakaehr --------- --c----c- p-p-p-p-p --------- --------- P-P-P-P-P ----C--C- --------- RHEAKAEHR W -1 0 0 0 0 0 0 paf MaxII -1 2 12 21 25 234 174 1 c/b7-c7 (0:06) cc7 0 0");
    //convert the whole package to style 12 format
    
    SendMessage(hwndMain, WM_BOSH_Input, (WPARAM)sendstr, REQUEST_FRONTEND_MOVED);
#endif
    char to_play = 'W';
    std::string turn = BoardTag->findAttribute("turn");
    std::string fullmoves = BoardTag->findAttribute("fullmoves");
    std::string long_str = MoveTag->findAttribute("long");
    std::string short_str = MoveTag->findAttribute("short");
    int gamenum;
    if(turn=="black")
        to_play = 'B';

    int relation = -1;
    if(ai_color == turn)
    {
        relation = 1;
    }
    int moveNum = atoi(fullmoves.c_str());
    gamenum = atoi(GameID.substr(5).c_str());
    int irrev_count = 0;
    char *nameW,*nameB;
    char *timeW,*timeB;
    if(Player1.Color == "white")
    {
        nameW = (char *)Player1.Name.c_str();
        nameB = (char *)Player2.Name.c_str();
        timeW = (char *)Player1.Time.c_str();
        timeB = (char *)Player2.Time.c_str();
    }
    else
    {
        nameB = (char *)Player1.Name.c_str();
        nameW = (char *)Player2.Name.c_str();
        timeB = (char *)Player1.Time.c_str();
        timeW = (char *)Player2.Time.c_str();

    }
    int nTimeBlack = atoi(timeB);/*still seconds */
    int nTimeWhite = atoi(timeW);
    char *stzlong =(char *)long_str.c_str();
    char *stzshort =(char *)short_str.c_str();
    char *stzfen = (char *)fen.c_str();
    int basetime, increment;
    char str_elapsed_time[512]={0};
    convert_base_incr(&basetime,&increment);
    if(to_play == 'B')
    {
        compute_elapsed_time('W',nTimeWhite,str_elapsed_time);
        set_remaining_time('W',nTimeWhite);
    }
    else if(to_play == 'W')
    {
        compute_elapsed_time('B', nTimeBlack,str_elapsed_time);
        set_remaining_time('B', nTimeBlack);
    }
    
    set_board_style(to_play,-1,
        1,1,1,1,
        irrev_count,gamenum,
        nameW,nameB,
        relation,basetime,
        increment,39,39,nTimeWhite,nTimeBlack,
        moveNum,stzlong,str_elapsed_time,stzshort,1,1,stzfen);
    if(moveNum == 1)
    {
        StopClocks();
    }
//    printf("GAME_Move755:%s\n",Buffer.c_str());

    return Buffer;
}

/**
* Handle Game State
* It's a good ideia to read the server's documentation before reading the code above
*
* @param 	XML The xml that contains the game state
* @return 	void
* @author 	Ulysses and Rubens
*/
std::string GAME_State(Tag *tag)
{
    std::string GameID;
    Tag *HistoryPlayerTag[2];
    Tag *StatusPlayerTag[2];
    Tag *Query=NULL;
    Tag *HistoryTag=NULL;
    Tag *CurrStatusTag=NULL;
    Tag *HistoryStatusTag=NULL;
    int inum = 0;
    Player_t Player1, Player2;
    std::string Buffer = "";

    std::string WTime, BTime;
    HistoryPlayerTag[0]=NULL;
    HistoryPlayerTag[1]=NULL;
    StatusPlayerTag[0]=NULL;
    StatusPlayerTag[1]=NULL;
//game_room
//printf("youve entered this gameroom\n");
    std::string from = tag->findAttribute("from");
    if(!from.empty())
    {
        size_t nfound = from.find("@");
        if(nfound!=std::string::npos)
        {
            GameID = from.substr(0,nfound);
        }
    }
//query tag
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
    // Try to get the history tag

    const TagList& qsubl = Query->children();
    TagList::const_iterator itsub = qsubl.begin();
    for( ; itsub != qsubl.end(); ++itsub )
    {
      if( (*itsub)->name() == "history" )
      {
        HistoryTag= *itsub;
        break;
      }
    }
    if(!HistoryTag)
        return "";

//get the player tag inside history tag
    const TagList& hisl = HistoryTag->children();
    TagList::const_iterator ithis = hisl.begin();
    for( ; ithis != hisl.end(); ++ithis )
    {
      if( (*ithis)->name() == "player" )
      {
        HistoryPlayerTag[inum++]= *ithis;
        if(inum>1)
        {
            break;
        }
      }
    }
    if(!HistoryPlayerTag[0])
        return "";
    if(!HistoryPlayerTag[1])
        return "";
//get the status tag inside the history tag
//    const TagList& hisl = HistoryTag->children();
    TagList::const_iterator it_hist = hisl.begin();
    for( ; it_hist != hisl.end(); ++it_hist )
    {
      if( (*it_hist)->name() == "state" )
      {
        HistoryStatusTag= *it_hist;
        break;
      }
    }

// First Board State (without history tag)
    if(!HistoryStatusTag)
    {
      Buffer = GAME_Move(tag);
    }
    else
    {
//        printf("NewIamhere\n");

        // Try to get the current state tag

        const TagList& cur_state_list = Query->children();
        TagList::const_iterator it_cur_state = cur_state_list.begin();
        for( ; it_cur_state != cur_state_list.end(); ++it_cur_state )
        {
          if( (*it_cur_state)->name() == "state" )
          {
            CurrStatusTag= *it_cur_state;
            break;
          }
        }
        if(!CurrStatusTag)
            return "";

        // Get players time left
        int i_stateplayer_num = 0;
        const TagList& statusl = CurrStatusTag->children();
        TagList::const_iterator it_stat = statusl.begin();
        for( ; it_stat != statusl.end(); ++it_stat )
        {
          if( (*it_stat)->name() == "player" )
          {
            StatusPlayerTag[i_stateplayer_num++]= *it_stat;
            if(i_stateplayer_num>1)
            {
                break;
            }
          }
        }
//        printf("num=%d\n",i_stateplayer_num);
        if(!StatusPlayerTag[0])
            return "";
        if(!StatusPlayerTag[1])
            return "";
//        printf("line826\n");
        if(StatusPlayerTag[0]->hasAttribute("color","white"))
        {
        	WTime = StatusPlayerTag[0]->findAttribute("time");
        	BTime = StatusPlayerTag[1]->findAttribute("time");
        }
        else
        {
        	WTime = StatusPlayerTag[1]->findAttribute("time");
        	BTime = StatusPlayerTag[0]->findAttribute("time");
        }

        JID jid_p1(HistoryPlayerTag[0]->findAttribute("jid") );

        Player1.Name = jid_p1.username();
//        printf("jid_p1=%s\n",Player1.Name.c_str());
        Player1.Inc = HistoryPlayerTag[0]->findAttribute("inc");
        Player1.Color = HistoryPlayerTag[0]->findAttribute("color");
        Player1.Time = HistoryPlayerTag[0]->findAttribute("time");

        JID jid_p2(HistoryPlayerTag[1]->findAttribute("jid") );

        Player2.Name = jid_p2.username();
        Player2.Inc = HistoryPlayerTag[1]->findAttribute("inc");
        Player2.Color = HistoryPlayerTag[1]->findAttribute("color");
        Player2.Time = HistoryPlayerTag[1]->findAttribute("time");
        // History moves
//        printf("Line853\n");
        if (DATA_FindGame(GameID) == 0)// you should start it now...
        {
            GAME_StartGame(GameID, Player1, Player2);

            /* remove "game_" */
            int gamenum = atoi(GameID.substr(5).c_str());
            int basetime, increment;
            basetime = atoi(Player2.Time.c_str())/60;
            increment = atoi(Player2.Inc.c_str());
            int time_init = basetime*60+increment;
            char *nameW,*nameB;
            char *timeW,*timeB;
            if(Player1.Color == "white")
            {
                nameW = (char *)Player1.Name.c_str();
                nameB = (char *)Player2.Name.c_str();
                timeW = (char *)Player1.Time.c_str();
                timeB = (char *)Player2.Time.c_str();
            }
            else
            {
                nameB = (char *)Player1.Name.c_str();
                nameW = (char *)Player2.Name.c_str();
                timeB = (char *)Player1.Time.c_str();
                timeW = (char *)Player2.Time.c_str();

            }
            set_remaining_time('W',time_init);
            set_remaining_time('B',time_init);
            set_board_style('W',-1,
                            1,1,1,1,
                            0,gamenum,
                            nameW,nameB,
                            1,basetime,increment,
                            39,39,
                            time_init,time_init,
                            1,"none",
                            "(0:00)","none",1,2,
                            "rnbakabnr/9/1c5c1/p1p1p1p1p/9/9/P1P1P1P1P/1C5C1/9/RNBAKABNR");
            StopClocks();


#ifdef USE_BITSTRONGER
//if the AI color is "white", AI will move first.
            int ShouldIMove = 0;
            if (Player1.Name == _g_RotName)
            {
                if(Player1.Color == "white")
                {
                    //AI moves first
                    ShouldIMove = 1;
                }
            }
            else
            {
                if(Player2.Color == "white")
                {
                    ShouldIMove = 1;
                }
            }
            if(1 == ShouldIMove)
            {
                Tag *BoardTag=NULL;
//                printf("I am here\n");
//                const TagList& statusl = CurrStatusTag->children();
                TagList::const_iterator it_stat_2 = statusl.begin();
                for( ; it_stat_2 != statusl.end(); ++it_stat_2 )
                {
                  if( (*it_stat_2)->name() == "board" )
                  {
                    BoardTag= *it_stat_2;
                  }
                }
                if(!BoardTag)
                    return "";
                std::string fen = BoardTag->findAttribute("state");
                bitstronger_setposition(fen,'w');
                printf("state:aiqin is now calculating:\n");
                std::string ai_move = bitstronger_move();
//                          printf("substr=%s",ai_move.substr(0,9).c_str());
                if(ai_move == "nobestmove")
                {
                    //Buffer = "";
                }
                else if(ai_move.substr(0,9)== "bestmove ")
                {
                    std::string hostname = get_xmpp_hostname();
                    Buffer = "<iq xml:lang='zh-cn' type='set' to='";
                    Buffer += GameID;
                    Buffer += "@chessd."
                    Buffer += hostname;
                    Buffer += "' id='GameMove'><query xmlns='http://c3sl.ufpr.br/chessd#game#move'><move long='";
                    Buffer += ai_move.substr(9);
                    Buffer += "'></move></query></iq>";
                }
                else
                {
                    //do nothing?
                }
            }
#endif
        }
        else //I've started!
        {
            //do nothing
        }
    }

    return Buffer;
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
bool ci_equal( char ch1, char ch2 )
{
    return std::toupper( (unsigned char)ch1 ) == std::toupper( (unsigned char)ch2 );
}
std::string::size_type ci_find( const std::string& str1, const std::string& str2 )
{
    std::string::const_iterator pos = std::search( str1.begin(), str1.end(),
                                                   str2.begin(), str2.end(), ci_equal );
    if( pos == str1.end() )
      return std::string::npos;
    else
      return std::distance( str1.begin(), pos );
}

MessageTest *_g_msginstance;
static int trythreetime = 0;

extern "C" int bosh_main( int /*argc*/, char** /*argv*/ )
{
  for(;trythreetime < 3;trythreetime++)
  {
      SendMessage(hwndMain, WM_BOSH_Input, 0, REQUEST_USERNAME);
      while(1)
      {
          if(g_clientstatus == INITIAL_CLIENT_STATE)
          {
            break;
          }
          else
          {
              Sleep(500);
          }
      }
      if(g_clientstatus == INITIAL_CLIENT_STATE)
      {
          SendMessage(hwndMain, WM_BOSH_Input, 0, ACK_TO_CONNECT);
#ifdef USE_BITSTRONGER
          bitstronger_init();
#endif
          _g_msginstance = new MessageTest();
          _g_msginstance->start();
#ifdef USE_BITSTRONGER
          bitstronger_exit();
#endif
          SendMessage(hwndMain, WM_BOSH_Input, 0, ACK_EXIT_CONNECT);

          delete( _g_msginstance );
          _g_msginstance = NULL;
      }
  }
  return 0;
}


extern "C" int send_bosh_message(char *message, int count)
{
    char *info=NULL;
    int infocount=0;
    
    /*trim enter key */
    if(count<=2||message == NULL)
    {
        //do nothing
    }
    else if(message[count-2] == '\r'&& message[count-1] == '\n')
    {
        info = new char[count-1];
        strncpy(info, message, count-2);
        info[count-2]=0;
        infocount = count-2;
    }
    else
    {
        info = new char[count+1];
        strncpy(info, message, count);
        info[count]=0; 
        infocount = count;
    }
    if(_g_msginstance == NULL)
    {
        if( g_clientstatus == WAITING_FOR_USERNAME_STATE)
        {
            _g_RotName.append((const char *) info);
            g_clientstatus = WAITING_FOR_PASSWORD_STATE;
            SendMessage(hwndMain, WM_BOSH_Input, 0, REQUEST_PASSWORD);

        }
        else if( g_clientstatus == WAITING_FOR_PASSWORD_STATE) 
        {
            _g_RotPwd.append((const char *)info);
            g_clientstatus = INITIAL_CLIENT_STATE;
            EchoOn();
        }
    }
    else 
    {
    }
    if(info)
        delete info;
    return count;
}
extern "C" void send_xmpp_request(int type, char *content)
{
    ClientSample *j = _g_msginstance->get_client();
    if(type == SEND_SEARCH_USERS)
    {
        j->send_message_get_myprofile();
    }
    else if(type == SEND_USERINFO)
    {
    // user name
//        j->send_message_get_userinfo(content);
    }
}
extern "C" void send_xmpp_challenge_message(char *Oponent, char *Color, char *TimeStr, char *IncStr, char *Category, int Rated)
{
    ClientSample *j = _g_msginstance->get_client();
    j->send_message_challenge(Oponent, Color, TimeStr, IncStr, Category, Rated);
}
extern "C" void send_xmpp_mymove_message(char *move)
{
    ClientSample *j = _g_msginstance->get_client();
    std::string movestr;
    // clear enter key
    if(move)
    {
        int n=0;
        for(int i=0;i<=strlen(move);i++)
        {
            if('\n' == move[i])
            {
                n = i;
                break;
            }
        }
        movestr.assign(move,n);
    }
    j->send_message_mymove(movestr);

}
/*
base- unit: minute
inc - unit: second
*/
void convert_base_incr(int *base, int *inc)
{
    if(base)
    {
        if(inc)
        {
            if(!_g_current_TimeSet.empty())
            {
                int pos = 0;
                for(int k=0;k<_g_current_TimeSet.length();k++)
                {
                    if(_g_current_TimeSet[k] == ':')
                    {
                        pos = k;
                    }
                }
                std::string basestr = _g_current_TimeSet.substr(0,pos-1);
                std::string incstr = _g_current_TimeSet.substr(pos+1);
                *base = atoi(basestr.c_str());
                *base /= 60;
                *inc = atoi(incstr.c_str());
            }
        }
    }
}

/* unit: second */
static int _g_blackmove_remaining_time;
static int _g_whitemove_remaining_time;
int set_remaining_time(char who, int current_time_remain)
{
    if(who == 'B')
    {
        _g_blackmove_remaining_time = current_time_remain;
    }
    else if(who == 'W')
    {
        _g_whitemove_remaining_time = current_time_remain;
    }

}

/*
who -- who has moved?
current_time_remain -- the current time remaining of 'who'
return - return the elapsed time with its unit 'second'
note :whiteTimeRemaining is in ms
*/


int compute_elapsed_time(char who, int current_time_remain, char *str)
{
    int elapsed = -1;
    long previous_time_remain;
    if(str)
    {
        if(who == 'B')
        {
            previous_time_remain = _g_blackmove_remaining_time;
        }
        else if(who == 'W')
        {
            previous_time_remain = _g_whitemove_remaining_time ;
        }
        else
        {
            strcpy(str,"(0:00)");
            return 0;
        }

        elapsed = previous_time_remain - current_time_remain;
        if(elapsed<=0)
        {
            strcpy(str,"(0:00)");
            elapsed = 0;
        }
        else
        {
            int hours=0,minutes = 0, seconds=0;
            char strh[512], strm[512],strs[512];
            hours = elapsed/3600;
            minutes = (elapsed-hours*3600)/60;
            seconds = elapsed-hours*3600-minutes*60;
            str[0]='(';
            sprintf(strh,"%d",hours);
            sprintf(strm,"%d",minutes);
            sprintf(strs,"%d",seconds);
            if(hours !=0)
            {
                strcat(str,strh);
                strcat(str,":");
            }
            if(minutes<10)
            {
                strcat(str,"0");
            }
            strcat(str,strm);
            strcat(str,":");
            if(seconds<10)
            {
                strcat(str,"0");
            }
            strcat(str,strs);
            strcat(str,")");
        }
    }
    return elapsed;
}

