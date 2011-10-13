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


class ClientSample : public Client
{
  public:
    ClientSample( const JID& jid, const std::string& password, int port = -1 )
      : Client( jid, password, port ),m_current_status(INITIAL_CLIENT_STATE)
    {
    }
    virtual ~ClientSample() {}
    virtual void handleStartNode(); 
    virtual bool handleNormalNode( Tag* tag );
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
    void sendxml( const std::string& xml );
};


