#include "../client.h"
#include "../messagesessionhandler.h"
#include "../messageeventhandler.h"
#include "../messageeventfilter.h"
#include "../chatstatehandler.h"
#include "../chatstatefilter.h"
#include "../connectionlistener.h"
#include "../disco.h"
#include "../message.h"
#include "../gloox.h"
#include "../lastactivity.h"
#include "../loghandler.h"
#include "../logsink.h"
#include "../connectiontcpclient.h"
#include "../connectionsocks5proxy.h"
#include "../messagehandler.h"
#include "../connectionbosh.h"
#include "../connectionhttpproxy.h"
#include "../vcard.h"

using namespace gloox;

#include <unistd.h>
#include <stdio.h>
#include <string>

#include <cstdio> // [s]print[f]

#if defined( WIN32 ) || defined( _WIN32 )
# include <windows.h>
#endif

const std::string XMLNS_XQ_OFFER="http://c3sl.ufpr.br/chessd#match#offer";
const std::string XMLNS_XQ_ACCEPT="http://c3sl.ufpr.br/chessd#match#accept";
const std::string _g_RotName="robottest";
const std::string _g_RotPwd="robottest";
const std::string _g_DomainName = "www.meowchess.com";
FILE * _g_logfile = NULL;
typedef struct tagPlayer
{
    std::string Name;
    std::string Inc;
    std::string Color;
    std::string Time;
} Player_t;

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
      : Client( jid, password, port ),m_current_status(0)
    {
    }
    virtual ~ClientSample() {}
    virtual void handleStartNode() 
    {
        JID componentjid( _g_DomainName.c_str() );
        IQ iq(IQ::Get, componentjid,"auth_1");
        Query *q = new Query( _g_RotName );
        iq.addExtension( q );
        send(iq);


        //            m_session = new MessageSession(j,componentjid);
        //            m_session->send("hello");
        //            j->disposeMessageSession(m_session);
        m_current_status = 2;
    }
    virtual bool handleNormalNode( Tag* tag )
    {
        if(2 == m_current_status)
        {
            printf("current status=2,tag\n");
            if( tag->xmlns().empty() || tag->xmlns() == XMLNS_CLIENT )
            {
              if( tag->name() == "iq"  )
              {
                  std::string result;
                  result = "<iq type='result' from='www.meowchess.com' id='auth_1' xmlns='jabber:client'><query xmlns='jabber:iq:auth'><username>";
                  result += _g_RotName;
                  result +="</username><password/><resource/></query></iq>"; 
                  if( tag->xml() != result)
                  {
                    printf("status=1\n");
                  }
                  else
                  {
                    //send auth_2;
                    JID componentjid( _g_DomainName.c_str() );
                    IQ iq(IQ::Set, componentjid,"auth_2");
                    Tag* q = new Tag( "query" );
                    q->setXmlns( XMLNS_AUTH );
                    new Tag( q, "username" );
                    new Tag( q, "password" );
                    new Tag( q, "resource" );
                    Query n( q );
                    Query* nq = n.newInstance( _g_RotName, "sid", _g_RotPwd, "ChessD" );

                    iq.addExtension( nq );
                    send(iq);
                    m_current_status=3;

                  }
              }
            }
        }
        else if(3 == m_current_status) //I'm now online
        {
            if( tag->xmlns().empty() || tag->xmlns() == XMLNS_CLIENT )
            {
              if( tag->name() == "iq"  )
              {
                  if( tag->xml() == "<iq type='result' from='www.meowchess.com' id='auth_2' xmlns='jabber:client'/>" )
                  {

                    JID componentjid( "chessd.www.meowchess.com" );
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

                    JID generaljid( "general@conference.www.meowchess.com/"+_g_RotName);
                    Presence pres3(Presence::Available,generaljid);

                    std::string xml=iqUserList.tag()->xml()
                                    +pres.tag()->xml()
                                    +iqProfile.tag()->xml()
                                    +pres2.tag()->xml()
                                    +pres3.tag()->xml();
                    sendxml(xml);
                    m_current_status = 4;
                  }
                  else
                  {
//                    printf("status=3\n");
                  }
              }
           }

        }
        else if(4 == m_current_status)//m_current_status >=4
        {
             if( tag->xmlns().empty() )
             {
               if( tag->name() == "iq"  )
               {

                   if( tag->findAttribute("id")!=EmptyString&&tag->hasAttribute("type","set") )
                   {
                   
                     Tag *query = tag->findChild("query");
                     if(query)
                     {
                        if(query->xmlns() == XMLNS_XQ_OFFER)
                        {
                            Tag *match = query->findChild("match");
                            if(match)
                            {
                                TagList l = match->findChildren( "player" );
                                TagList::const_iterator it = l.begin();
                                if( l.size() != 2 )
                                {
                                  printf( "warning: player number should be 2\n" );
                                }
                                //send accept message.
                                
                                std::string xml_header = "<iq xml:lang='zh-cn' type='set' to='chessd.www.meowchess.com' id='Challenge'><query xmlns='http://c3sl.ufpr.br/chessd#match#accept'><match id='";
                                std::string xml_id = match->findAttribute("id"); //"0";
                                std::string xml_last = "'></match></query></iq>";
                                
                                sendxml(xml_header+xml_id+xml_last);
                                m_current_status = 5;
                            }
                        }

                     }
                   }
                   else
                   {
//                     printf("status=%d\n",m_current_status);
                   }
               }
            }

        }
        else
        {
            if( tag && tag->name() == "iq"  )
            {
                std::string buffer = PARSER_ParseIq(tag);
//                printf("PARSER_ParseIq:%s\n",buffer.c_str());
                if(!buffer.empty())
                {
                    sendxml(buffer);
                }
            }
        }
        return false;
    }

    int get_status(){return m_current_status;}
    void set_status(int status){m_current_status=status;}
    std::string PARSER_ParseIq(Tag *tag);
    std::string GAME_HandleGame(Tag *tag);
    std::string CHALLENGE_HandleChallenge(Tag *tag);
    std::string CHALLENGE_HandleAccept (Tag *tag);
  private:
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

      JID jid( _g_RotName+"@"+_g_DomainName+"/ChessD" );
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


      ConnectionTCPClient* conn0 = new ConnectionTCPClient( j->logInstance(), "www.meowchess.com", 8082 );
//      ConnectionHTTPProxy* conn1 = new ConnectionHTTPProxy( conn0, j->logInstance(), "www.meowchess.com", 8082 );
      ConnectionBOSH* conn2 = new ConnectionBOSH( j, conn0, j->logInstance(), "www.meowchess.com", "www.meowchess.com" );
//      conn2->setMode( ConnectionBOSH::ModeLegacyHTTP );
        conn2->setMode( ConnectionBOSH::ModePersistentHTTP );
      j->setConnectionImpl( conn2 );


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
           fprintf(_g_logfile,"BOSH: ");
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
        Buffer = "<iq xml:lang='zh-cn' type='set' to='";
        Buffer += GameID;
        Buffer += "@chessd.www.meowchess.com' id='GameMove'><query xmlns='http://c3sl.ufpr.br/chessd#game#move'><move long='";
        Buffer += ai_move.substr(9);
        Buffer += "'></move></query></iq>";
    }
    else
    {
        //do nothing?
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
                    Buffer = "<iq xml:lang='zh-cn' type='set' to='";
                    Buffer += GameID;
                    Buffer += "@chessd.www.meowchess.com' id='GameMove'><query xmlns='http://c3sl.ufpr.br/chessd#game#move'><move long='";
                    Buffer += ai_move.substr(9);
                    Buffer += "'></move></query></iq>";
                }
                else
                {
                    //do nothing?
                }
            }

        }
        else //I've started!
        {
            //do nothing
        }
    }

    return Buffer;
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
        return "";
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

	// Remove all challanges on structure
	//MainData.ClearChallenges();
//    CHALLENGE_ClearChallenges();

	// TODO
	// Warn the player's interface

	// Send a presence to GameRoom with playing status
	Buffer += MESSAGE_ChangeStatus("playing",GameRoom);	

	return Buffer;	
}

int main( int /*argc*/, char** /*argv*/ )
{
  bitstronger_init();
  MessageTest *r = new MessageTest();
  r->start();
  bitstronger_exit();

  delete( r );
  return 0;
}
