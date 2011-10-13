/*
 *   Copyright (c) 2007-2008 C3SL.
 *
 *   This file is part of Bosh.
 *
 *   Bosh is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   any later version.
 *
 *   Bosh is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 */


#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <unistd.h>
#include <sys/socket.h>

#include <signal.h>

#include <inttypes.h>

#include <iksemel.h>

#include <errno.h>

#include "http_server.h"
#include "jabber_bind.h"
#include "log.h"
#include "allocator.h"
#include "socket.h"

#define JABBER_PORT 5222

#define SESSION_TIMEOUT (30000)

#define DEFAULT_REQUEST_TIMEOUT (30000)

#define JABBER_HEADER "<stream:stream xmlns='jabber:client' xmlns:stream='http://etherx.jabber.org/streams' to='%s' xml:lang='en'>"
//#define JABBER_HEADER "<stream:stream xmlns='jabber:client' version='1.0' xmlns:stream='http://etherx.jabber.org/streams' to='%s' xml:lang='en'>"

#define MESSAGE_WRAPPER "<body xmlns:stream='http://etherx.jabber.org/streams' xmlns='http://jabber.org/protocol/httpbind'>%s</body>"

#define EMPTY_RESPONSE "<body xmlns='http://jabber.org/protocol/httpbind'/>"

#define SESSION_RESPONSE "<body sid='%" PRId64 "' ver='1.6' xmlns='http://jabber.org/protocol/httpbind'/>"

#define TERMINATE_SESSION_RESPONSE "<body type='terminate' xmlns='http://jabber.org/protocol/httpbind'/>"

#define ERROR_RESPONSE "<body type='%s' condition='%s' xmlns='http://jabber.org/protocol/httpbind'/>"

#define STATUS_HTML "<html><body>" \
                        "<p>Uptime: %s</p>" \
                        "<p>Clients: %d</p>" \
                        "<p>Maximum clients: %d</p>" \
                    "</body></html>"

static inline int compare_sid(uint64_t s1, uint64_t s2) {
    return s1 == s2;
}

static inline unsigned int hash_sid(uint64_t s) {
    return s % 4294967291u;
}

typedef uint64_t uint64;
DECLARE_HASH(uint64, hash_sid, compare_sid);
IMPLEMENT_HASH(uint64);

enum BIND_ERROR_CODE {
    SID_NOT_FOUND = 0,
    BAD_FORMAT = 1,
    CONNECTION_FAILED = 2
};

const char ERROR_TABLE[][2][64] = {
    {"terminate", "item-not-found"},
    {"terminate", "bad-request"},
    {"terminate", "host-gone"}
};

volatile int running;

typedef struct JabberClient {
    iksparser* parser;          /* jabber stream parser from iksemel          */
    Socket* sock;               /* socket of the jabber connection            */
    uint64_t sid, rid;          /* rid and sid of the BOSH session            */
    HttpConnection* connection; /* http connection of the pending request     */
    list* output_queue;         /* queue of messages to be sent to the client */
    int alive;                  /* 1 if the connection is alive, 0 otherwise  */
    time_type timestamp;        /* last activity in the session               */
    time_type wait;             /* maximum time to hold a request             */
	list_iterator it;           /* pointer to this client in the client list  */
	struct JabberBind* bind;    /* pointer to the bund struct                 */
} JabberClient;


struct JabberBind {
	list* jabber_connections;    /* list of jabber connections                */
	uint64_hash* sids;           /* hash of used sids                         */
	HttpServer* server;          /* pointer to the http server                */

    int jabber_port;             /* port to connect to the jabber server      */
    int session_timeout;         /* bosh session timeout                      */
    time_type start_time;        /* the time when the server started          */
    int client_count;            /* number of active connections              */
    int max_client_count;        /* the maximum number of clients achieved    */
};

/* Allocators */
DECLARE_ALLOCATOR(JabberClient);
IMPLEMENT_ALLOCATOR(JabberClient);

/*! \brief Handle exit signals */
void handle_signal(int signal) {
    log(INFO, "signal caught %d", signal);
    running = 0;
}

/*! \brief Return the time remaning to the nearest possible timeout  */
time_type jb_closest_timeout(JabberBind* bind) {
    list_iterator it;
    JabberClient* j_client;
    time_type closest, tmp, current;

    closest = bind->session_timeout;
    current = get_time();

    /* check each connection */
    list_foreach(it, bind->jabber_connections) {
        j_client = list_iterator_value(it);
        if(j_client->connection != NULL) {
            /* we have a request, so the timeout is the request timeout */
            tmp = (j_client->timestamp + j_client->wait) - current;
        } else {
            /* we don't have a request, so the timeout is the session timeout */
            tmp = (j_client->timestamp + bind->session_timeout) - current;
        }
        if(tmp < closest) {
            closest = tmp;
        }
    }

    return closest;
}

/*! \brief Flush pending messages to the client */
void jc_flush_messages(JabberClient* j_client) {
    char* xml;
    char* body;
    char* buffer;
    char* ptr;
    iks* msg;
    list* xmls;
    int size, n;

    /* check if there is a pending request and if there is any data to send */
    if(j_client->connection != NULL && !list_empty(j_client->output_queue)) {

        size = 0;
        xmls = list_new();

        /* foreach message, convert to string and sum the size */
        while(!list_empty(j_client->output_queue)) {
            msg = list_pop_front(j_client->output_queue);
            xml = iks_string(NULL, msg);
            list_push_back(xmls, xml);
            size += strlen(xml);
            iks_delete(msg);
        }

        /* alloc a buffer */
        ptr = buffer = malloc(size + 1);

        /* put the message in the buffer */
        while(!list_empty(xmls)) {
            xml = list_pop_front(xmls);
            n = strlen(xml);
            memcpy(ptr, xml, n);
            ptr += n;
            iks_free(xml);
        }
        *ptr = 0;
        list_delete(xmls, NULL);

        /* create http content */
        asprintf(&body, MESSAGE_WRAPPER, buffer);

        log(INFO, "Request response sid=%" PRId64 " message: %s",
                j_client->sid, body);

        /* send messages */
        hs_answer_request(j_client->connection, body, strlen(body),
                HTTP_XML_CONTENT);
        j_client->connection = NULL;

        /* update last activity */
        j_client->timestamp = get_time();

        free(buffer);
    }
}

/*! \brief Answer a request with an empty body */
void jc_drop_request(JabberClient* j_client, int terminate) {
    char* body;

    /* if terminate is true, send a notification that the session
     * is been closed */
    if(terminate) {
        asprintf(&body, TERMINATE_SESSION_RESPONSE);
    } else {
        asprintf(&body, EMPTY_RESPONSE);
    }

    log(INFO, "Request response sid=%" PRId64 " message: %s", j_client->sid,
            body);

    /* answer the request */
    hs_answer_request(j_client->connection, body, strlen(body),
            HTTP_XML_CONTENT);
    j_client->connection = NULL;

    /* update last activity */
    j_client->timestamp = get_time();
}

/*! \brief Free an iks struct */
static void _iks_delete(void* _iks) {
    iks* iks = _iks;
    iks_delete(iks);
}

/*! \brief Close a connection to the jabber server */
void jb_close_client(JabberClient* j_client) {
    JabberBind* bind = j_client->bind;

    log(INFO, "Connection closed sid=%" PRId64, j_client->sid);

    /* drop the request if we have one */
    if(j_client->connection != NULL) {
        jc_drop_request(j_client, 1);
    }

    /* free iks struct */
    iks_disconnect(j_client->parser);
    iks_parser_delete(j_client->parser);

    if(j_client->sock != NULL) {
        sock_delete(j_client->sock);
    }

    /* erase the client from the list of clients */
    list_erase(j_client->it);
    bind->client_count --;

    /* erase the client's sid */
    uint64_hash_erase(bind->sids, j_client->sid);

    /* free client struct */
    list_delete(j_client->output_queue, _iks_delete);
    JabberClient_free(j_client);
}

/*! \brief Check timeouts and handle them */
void jb_check_timeout(JabberBind* bind) {
    JabberClient* j_client;
    list_iterator it;
    time_type init, idle;
    list* to_close;

    /* list of connections that are to be closed */
    to_close = list_new();

    /* get current time */
    init = get_time();

    /* check each connection for a timeout */
    list_foreach(it, bind->jabber_connections) {
        j_client = list_iterator_value(it);

        idle = init - j_client->timestamp;
        if(j_client->connection != NULL && idle >= j_client->wait) {
            /* we have a timedout request, drop it */
            jc_drop_request(j_client, 0);
        } else if(j_client->connection == NULL &&
                  idle >= bind->session_timeout) {
            /* we don't have a request and the session is idle for too long,
             * close the session */
            log(WARNING, "timeout on sid=%" PRId64, j_client->sid);
            /* don't really close it right now, because our pointer to the list
             * will became invalid if we do so, put it on a list so we can
             * close it after we check all clients */
            list_push_back(to_close, j_client);
        }
    }

    /* close innactive connections */
    while(!list_empty(to_close)) {
        j_client = list_pop_front(to_close);
        jb_close_client(j_client);
    }

    /* free local allocs */
    list_delete(to_close, NULL);
}

/*! Handle an incoming message from the jabber server */
int jc_handle_stanza(void* _j_client, int type, iks* stanza) {
    JabberClient* j_client = _j_client;
    /* no message ? */
    if(stanza == NULL) {
        /* do nothing */
    } else if(type == IKS_NODE_NORMAL) {
        /* queue up a normal message */
        list_push_back(j_client->output_queue, stanza);
    } else if(type == IKS_NODE_ERROR || type == IKS_NODE_STOP) {
        /* close the connection in case of error or stop */
        log(WARNING, "Jabber connection ended sid=%" PRId64, j_client->sid);
        iks_delete(stanza);
        j_client->alive = 0;
    } else {
        /* ignore otherwise */
        iks_delete(stanza);
    }
    return IKS_OK;
}

/*! \brief Handle activity in the jabber connection */
void jc_read_jabber(void* _j_client) {
    ssize_t bytes;
    int ret = IKS_OK;
    char buffer[8192];
    JabberClient* j_client = _j_client;

    /* read the socket and feed the parser */
    while(ret == IKS_OK &&
            (bytes = sock_recv(j_client->sock,
                          buffer, sizeof(buffer))) > 0) {
        ret = iks_parse(j_client->parser, buffer, bytes, 0);
    }

    /* flush the messages */
    jc_flush_messages(j_client);

    /* check if the connection was closed */
    if(sock_status(j_client->sock) != SOCKET_CONNECTED || ret != IKS_OK ||
            j_client->alive == 0) {
        jb_close_client(j_client);
    }
}

/*! \brief Handle error on the jabber connection */
void jc_handle_error(void* _j_client, int code) {
    JabberClient* j_client = _j_client;

    log(WARNING, "Error on jabber connection sid=%" PRId64 ": %s",
            j_client->sid, strerror(code));

    jb_close_client(j_client);
}


/* \brief Report a error to the client */
void jc_report_error(HttpConnection* connection, enum BIND_ERROR_CODE code) {
    char* body;

    /* create the message */
    asprintf(&body, ERROR_RESPONSE, ERROR_TABLE[code][0], ERROR_TABLE[code][1]);

    /* send the message */
    hs_answer_request(connection, body, strlen(body), HTTP_XML_CONTENT);
}

/*! \brief Returns a random sid */
uint64_t gen_sid() {
    return lrand48() | (((uint64_t)lrand48())<<32);
}

void jc_answer_creation(int code, void* user_data) {
    JabberClient* j_client = user_data;

    if(code != 0) {
        /* connection has failed */
        log(WARNING, "Could not connect to the jabber server sid=%" PRId64
                ": %s" , j_client->sid, strerror(code));

        /* close the client */
        jb_close_client(j_client);
    }
}

/*! \brief Create a new connection to the jabber server */
void jb_connect_client(JabberBind* bind, HttpConnection* connection,
        iks* body) {

    char* tmp;
    char* host;
    JabberClient* j_client;
    uint64_t rid;

    /* alloc memory */
    j_client = JabberClient_alloc();

    /* get wait parameter */
    tmp = iks_find_attrib(body, "wait");
    if(tmp == NULL) {
        log(WARNING, "No wait attribute in the header");
        JabberClient_free(j_client);
        jc_report_error(connection, BAD_FORMAT);
        return;
    }
    j_client->wait = atoi(tmp) * 1000;

    /* get to parameter */
    host = iks_find_attrib(body, "to");
    if(host == NULL) {
        log(WARNING, "No to attribute in the header");
        JabberClient_free(j_client);
        jc_report_error(connection, BAD_FORMAT);
        return;
    }

    /* get rid parameter */
    tmp = iks_find_attrib(body, "rid");
    if(tmp == NULL) {
        log(WARNING, "Wrong header");
        JabberClient_free(j_client);
        jc_report_error(connection, BAD_FORMAT);
        return;
    }
    sscanf(tmp, "%" PRId64, &rid);

    /* create the parser */
    j_client->parser = iks_stream_new("jabber:client", j_client,
            jc_handle_stanza);
    if(j_client->parser == NULL) {
        log(WARNING, "Could not create the jabber parser");
        JabberClient_free(j_client);
        jc_report_error(connection, CONNECTION_FAILED);
        return;
    }

    /* connect to host */
    j_client->sock = sock_new();
    if(sock_connect(j_client->sock, host, bind->jabber_port) == 0) {
        log(WARNING, "Could not connect to the jabber server");
        sock_delete(j_client->sock);
        JabberClient_free(j_client);
        jc_report_error(connection, CONNECTION_FAILED);
        return;
    }
    iks_connect_fd(j_client->parser, sock_fd(j_client->sock));

    /* pick a random sid */
    do {
        j_client->sid = gen_sid();
    } while(uint64_hash_has_key(bind->sids, j_client->sid));

    /* insert the sid value into the hash */
    uint64_hash_insert(bind->sids, j_client->sid, j_client);

    /* init client values */
    j_client->output_queue = list_new();
    j_client->bind = bind;
    j_client->connection = NULL;
    j_client->alive = 1;
    j_client->timestamp = get_time();
    j_client->it = list_push_back(bind->jabber_connections, j_client);
    bind->client_count ++;
    if(bind->client_count > bind->max_client_count) {
        bind->max_client_count = bind->client_count;
    }

    /* send jabber header */
    asprintf(&tmp, JABBER_HEADER, host);
    sock_send(j_client->sock, tmp, strlen(tmp), 0);

    /* set callbacks */
    sock_set_data_callback(j_client->sock, jc_read_jabber, j_client);
    sock_set_connect_callback(j_client->sock, jc_answer_creation, j_client);
    sock_set_error_callback(j_client->sock, jc_handle_error, j_client);

    /* send response */
    asprintf(&tmp, SESSION_RESPONSE, j_client->sid);
    hs_answer_request(connection, tmp, strlen(tmp), HTTP_XML_CONTENT);

    log(INFO, "New bosh session: sid=%" PRId64 " socket=%p",
            j_client->sid, j_client->sock);
}


/*! \brief Remove the http connection.
 *
 * This function is called by the http server to notify that the connection was
 * closed before sending a response */
void jc_clear_http(void* _j_client) {

    JabberClient* j_client = _j_client;

    log(WARNING, "Cleared http connection on sid=%" PRId64, j_client->sid);

    j_client->connection = NULL;
    j_client->timestamp = get_time();
}

/*! \brief Set the client request */
void jc_set_http(JabberClient* j_client, HttpConnection* connection, uint64_t rid) {
    /* if we already have one, drop it */
    if(j_client->connection != NULL) {
        jc_drop_request(j_client, 0);
    }

    /* update values */
    j_client->connection = connection;
    j_client->timestamp = get_time();
    j_client->rid = rid;

    /* set thew close callback */
    hc_set_close_callback(connection, jc_clear_http, j_client);

    /* flush messages */
    jc_flush_messages(j_client);
}

/*! \brief Handle an incoming http post */
void jb_handle_http_post(JabberBind* bind, const HttpRequest* request) {
    JabberClient* j_client;
    iks* message, *stanza;
    char* tmp;
    uint64_t sid, rid;

    /* parse the content */
    message = iks_tree(request->data, request->data_size, NULL);

    /* return an error if the xml is malformed */
    if(message == NULL) {
        log(WARNING, "Malformed xml");
        jc_report_error(request->connection, BAD_FORMAT);
        return;
    }

    /* get the sid */
    tmp = iks_find_attrib(message, "sid");
    if(tmp != NULL) {
        /* TODO, improve error handling to prevent repeating error hangling code */

        /* parse the sid */
        if(sscanf(tmp, "%" PRId64, &sid) != 1) {
            log(WARNING, "Invalid sid string %s", tmp);
            jc_report_error(request->connection, BAD_FORMAT);
            iks_delete(message);
            return;
        }

        log(INFO, "Incoming request sid=%s %s", tmp, request->data);

        /* get the rid */
        tmp = iks_find_attrib(message, "rid");
        if(tmp == NULL) {
            log(WARNING, "No rid in the header");
            jc_report_error(request->connection, BAD_FORMAT);
            iks_delete(message);
            return;
        }
        sscanf(tmp, "%" PRId64, &rid);

        /* get the client */
        j_client = uint64_hash_find(bind->sids, sid);
        if(j_client == NULL) {
            log(WARNING, "Sid not found: %" PRId64, sid);
            jc_report_error(request->connection, SID_NOT_FOUND);
            iks_delete(message);
            return;
        }
        jc_set_http(j_client, request->connection, rid);

        /* send stanzas to the client */
        for(stanza = iks_first_tag(message);
                stanza != NULL; stanza = iks_next_tag(stanza)) {
            tmp = iks_string(NULL, stanza);
            sock_send(j_client->sock, tmp, strlen(tmp), 1);
        }
        sock_send(j_client->sock, NULL, 0, 0);

        /* close the connection if the type is terminate */
        if(iks_strcmp(iks_find_attrib(message, "type"), "terminate") == 0) {
            jb_close_client(j_client);
        }

    } else {
        /* if there is no sid, than it is a request to create a connection */
        jb_connect_client(bind, request->connection, message);
    }
    iks_delete(message);
}

/*! \brief Handle an incoming http get */
void jb_handle_http_get(JabberBind* bind, const HttpRequest* request) {
    char* html;
    char* uptime_str;
    int days, hours, minutes, seconds;
    time_type uptime;

    uptime = (get_time() - bind->start_time)/1000;
    seconds = uptime % 60;
    uptime /= 60;
    minutes = uptime % 60;
    uptime /= 60;
    hours = uptime % 24;
    uptime /= 24;
    days = uptime;

    asprintf(&uptime_str, "%d days, %02d:%02d:%02d", days, hours, minutes,
            seconds);

    asprintf(&html, STATUS_HTML, uptime_str, bind->client_count,
            bind->max_client_count);

    hs_answer_request(request->connection, html, strlen(html),
            HTTP_HTML_CONTENT);

    free(uptime_str);
}

/*! \brief Handle an incoming request */
void jb_handle_request(void* _bind, const HttpRequest* request) {
    JabberBind* bind;

    bind = _bind;

    if(strcmp(request->header->type, "POST") == 0) {
        jb_handle_http_post(bind, request);
    } else if(strcmp(request->header->type, "GET") == 0) {
        jb_handle_http_get(bind, request);
    } else {
        log(WARNING, "Unknown http request");
        jc_report_error(request->connection, BAD_FORMAT);
        return;
    }
}

/*! \brief Run the server until a SIGINT or SIGTERM signal is caught */
void jb_run(JabberBind* bind) {
    time_type max_time;

    /* init running */
    running = 1;

    /* set signal handlers */
    signal(SIGINT, handle_signal);
    signal(SIGTERM, handle_signal);

    log(INFO, "Server is running");

    /* keep running until we receive a signal */
    while(running == 1) {
        /* take the nearest timeout */
        max_time = jb_closest_timeout(bind);

        /* some sanity test */
        if(max_time < 0)
            max_time = 0;

        /* wait for any socket activity, don't wait more than max_time */
        sm_poll(max_time);

        /* check if any timeout went off */
        jb_check_timeout(bind);
    }
}

/*! \brief crete a new bind server */
JabberBind* jb_new(iks* config) {
    JabberBind* jb;
    iks* bind_config;
    iks* http_config;
    iks* log_config;
    const char* str;

    jb = malloc(sizeof(JabberBind));

    /* Load config */
    bind_config = iks_find(config, "bind");
    http_config = iks_find(config, "http_server");
    log_config = iks_find(config, "log");

    if(bind_config == NULL || http_config == NULL || log_config == NULL) {
        fprintf(stderr, "Incomplete config file.\n");
        free(jb);
        return NULL;
    }

    /* set jabber port */
    if((str = iks_find_attrib(bind_config, "jabber_port")) != NULL) {
        jb->jabber_port = atoi(str);
    } else {
        jb->jabber_port = JABBER_PORT;
    }

    /* set session timeout */
    if((str = iks_find_attrib(bind_config, "session_timeout")) != NULL) {
        jb->session_timeout = atoi(str);
    } else {
        jb->session_timeout = SESSION_TIMEOUT;
    }

    /* init log */
    log_init(log_config);

    /* create the http server */
    jb->server = hs_new(http_config, jb_handle_request, jb);

    if(jb->server == NULL) {
        log(ERROR, "Failed to start HTTP server");
        free(jb);
        return NULL;
    }

    /* set other values */
    jb->jabber_connections = list_new();
    jb->sids = uint64_hash_new();
    jb->start_time = get_time();
    jb->client_count = 0;
    jb->max_client_count = 0;

    /* seed the random generator */
    srand48(get_time());

    return jb;
}

/*! \brief destroy a bind server*/
void jb_delete(JabberBind* bind) {
    JabberClient* j_client;

    /* close all jabber connections */
    while(!list_empty(bind->jabber_connections)) {
        j_client = list_front(bind->jabber_connections);
        jb_close_client(j_client);
    }

    /* free all data structures */
    list_delete(bind->jabber_connections, NULL);
    uint64_hash_delete(bind->sids);

    /* delete the http server */
    hs_delete(bind->server);

    free(bind);
}

