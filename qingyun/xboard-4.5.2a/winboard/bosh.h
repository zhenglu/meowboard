#define WM_BOSH_Input                 (WM_USER + 4245)
#define WM_SUBSCRIBER_INPUT           (WM_USER + 4246)

/*XMPP message type*/
#define REQUEST_USERNAME                 1
#define REQUEST_PASSWORD                 2
#define ACK_HAVE_LOGGEDIN                3
#define REQUEST_MYPROFILE                4
#define SEND_SEARCH_USERS                5
#define SEND_USERINFO                    6
#define REQUEST_UPDATE_SEEKGRAPH         7
#define SEND_CHALLENGE                   8
#define REQUEST_FRONTEND_START_CHALLENGE          9
#define REQUEST_FRONTEND_MOVED                    10
#define ACK_TO_CONNECT                            11    
#define ACK_EXIT_CONNECT                          12


/*status of the chess client*/
#define WAITING_FOR_USERNAME_STATE       -2
#define WAITING_FOR_PASSWORD_STATE       -1
#define INITIAL_CLIENT_STATE             0
#define HAVE_CONNECTED_STATE             1
#define VERIFYING_USERNAME_STATE         2
#define VERIFYING_PASSWORD_STATE         3
#define HAVE_LOGGEDIN_STATE              4

