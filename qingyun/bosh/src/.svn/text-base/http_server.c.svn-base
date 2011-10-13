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

#include <sys/socket.h>
#include <unistd.h>

#include <errno.h>

#include "http_server.h"
#include "socket.h"
#include "log.h"

#include "allocator.h"

#define HTTP_PORT 8080

#define HTML_ERROR "<html><head>" \
						"<title>400 Bad Request</title>" \
						"</head><body>" \
						"<h1>Bad Request</h1>" \
						"<p>%s</p>" \
						"</body></html>"

#define MAX_BUFFER_SIZE (1024*128)

struct HttpConnection {
    char buffer[MAX_BUFFER_SIZE+1];
    size_t buffer_size;
    struct HttpServer* server;
    Socket* sock;
    int rid;
	list_iterator it;
	HttpHeader* header;

    hc_close_callback close_callback;
    void* close_data;
};

struct HttpServer {
    list* http_connections;
    Socket* sock;
	hs_request_callback callback;
	void* user_data;
};

DECLARE_ALLOCATOR(HttpConnection);
IMPLEMENT_ALLOCATOR(HttpConnection);

/*! \brief Send an http error response */
static void hc_report_error(HttpConnection* connection, const char* msg) {
	char* body = NULL;
	char* header = NULL;

	asprintf(&body, HTML_ERROR, msg);
	header = make_http_head(500, strlen(body), HTTP_HTML_CONTENT);

    sock_send(connection->sock, header, strlen(header), 1);
    sock_send(connection->sock, body, strlen(body), 0);
}

/*! \brief Release the connection's resources */
static void hc_delete(HttpConnection* connection) {
    //HttpServer* server = connection->server;

    log(INFO, "Http connection closed socket=%p", connection->sock);

    /* call the close callback if there is any */
    if(connection->close_callback != NULL) {
        connection->close_callback(connection->close_data);
    }

    /* delete the socket */
    sock_delete(connection->sock);

    /* erase the connection from the list of connections */
	list_erase(connection->it);

    /* free header */
    if(connection->header != NULL) {
        http_delete(connection->header);
        connection->header = NULL;
    }

    /* free memory */
    HttpConnection_free(connection);
}

static void hc_read(void* _connection);

static void hc_handle_error(void* _connection, int error);

/*! \brief Create an http connetion */
static HttpConnection* hc_create(HttpServer* server, Socket* sock) {
    HttpConnection* connection;
    
    /* alloc memory for the struct */
    connection = HttpConnection_alloc();

    /* init values */
    connection->buffer_size = 0;
    connection->server = server;
    connection->sock = sock;
    connection->header = NULL;
    connection->close_callback = NULL;
    connection->close_data = NULL;

    /* insert the conenction into the connection list */
	connection->it = list_push_back(server->http_connections, connection);

    /* start to monitor the connection */
    sock_set_data_callback(sock, hc_read, connection);

    sock_set_error_callback(sock, hc_handle_error, connection);

    log(INFO, "Http connection created socket=%p", connection->sock);

    return connection;
}

/*! \brief Set a callback to be called when the connection is closed before the
 * request is answered. */
void hc_set_close_callback(HttpConnection* connection,
        hc_close_callback callback, void* data) {

    connection->close_callback = callback;
    connection->close_data = data;
}

/*! \brief Process an incoming message */
static void hc_process(HttpConnection* connection) {
    const char* tmp;
    const char* data;
    int content_size, header_size;
	HttpRequest hr;
    HttpServer* server = connection->server;

    /* get the content lenght */
    tmp = http_get_field(connection->header, "Content-Length");
    if(tmp == NULL) {
        content_size = 0;
    } else {
        content_size = atoi(tmp);
    }

    /* find the beginning of the content */
    data = strstr(connection->buffer, HTTP_LINE_SEP HTTP_LINE_SEP) + 4;

    header_size = data - connection->buffer;


    /* check if the message is bigger than current buffer size */
    if(content_size + header_size >= MAX_BUFFER_SIZE) {
        log(WARNING, "Message is too big");
        hc_report_error(connection, "Message is too big");
        return;
    }

    /* check if everything is here */
    if(connection->buffer_size >= header_size + content_size) {

        log(INFO, "Processing request Content-Length=%d", content_size);

        /* inform the request */
		hr.connection = connection;
		hr.header = connection->header;
		hr.data = data;
		hr.data_size = content_size;
		server->callback(server->user_data, &hr);

        /* free the header, we don't need it anymore */
		http_delete(connection->header);
		connection->header = NULL;

        /* move the buffer */
        if(connection->buffer_size > header_size + content_size) {
            memmove(connection->buffer, connection->buffer + header_size +
                    content_size, connection->buffer_size - header_size -
                    content_size);
            connection->buffer_size -= header_size + content_size;
        } else {
            connection->buffer_size = 0;
        }
    }
}

/*! \brief Read the header of a request */
static void hc_read(void* _connection) {
    HttpConnection* connection = _connection;
    int remaining_buffer;
    ssize_t ret;

    /* compute the remaining buffer space */
    remaining_buffer = MAX_BUFFER_SIZE - connection->buffer_size;

    /* receive some data */
    ret = sock_recv(connection->sock,
                 connection->buffer + connection->buffer_size,
                 remaining_buffer);

    if(ret > 0) {
        /* update the buffer */
        connection->buffer_size += ret;
        connection->buffer[connection->buffer_size] = 0;

        /* parse the header */
        if(connection->header == NULL) {
            connection->header = http_parse(connection->buffer);
        }

        /* if the header is complete, parser the content */
        if(connection->header != NULL) {
            hc_process(connection);
        }
    } else {
        log(INFO, "No data in socket\n");
    }
    if(sock_status(connection->sock) != SOCKET_CONNECTED) {
        hc_delete(connection);
    }
}

/*! \brief Handle an error on the socket */
static void hc_handle_error(void* _connection, int code) {
    HttpConnection* connection = _connection;

    log(WARNING, "Error on http connection: %s", strerror(code));

    hc_delete(connection);
}

/*! \breif Receive an incoming connection */
static void hs_accept(void* _server) {
    Socket* client;
    HttpServer* server = _server;

    /* accept the conenction */
    client = sock_accept(server->sock);

    /* check for error */
    if(client == NULL) {
        return;
    }

    log(INFO, "New connection accepted %p", server->sock);

    /* create the http connection */
    hc_create(server, client);

}

/*! \brief Create a new HTTP server
 *
 * \param config the configuratin from config file
 * \param monitor the monitor to be used
 * \param callback the function to be called when a request arrives
 * \param user_data the parameter to the callback
 *
 * \return A instance of the server
 */
HttpServer* hs_new(iks* config, hs_request_callback callback,
        void* user_data) {
    HttpServer* server;
    Socket* sock;
    const char* str;
    int port, ret;

    /* get the port to listen */
    if((str = iks_find_attrib(config, "port")) != NULL) {
        port = atoi(str);
    } else {
        port = HTTP_PORT;
    }

    /* create the socket */
    sock = sock_new();
    ret = sock_listen(sock, port);
    if(ret == 0) {
        log(ERROR, "Failed to listen http server port %d", port);
        return NULL;
    }

    /* alloc memomry for the server struct */
    server = malloc(sizeof(HttpServer));

    /* init values */
    server->sock = sock;
    server->http_connections = list_new();
	server->callback = callback;
	server->user_data = user_data;

    /* monitor the server socket for conenctions */
    sock_set_accept_callback(sock, hs_accept, server);

    return server;
}

/*! \brief Close an http server */
void hs_delete(HttpServer* server) {

    /* close ann connections */
    while(!list_empty(server->http_connections)) {
        hc_delete(list_front(server->http_connections));
    }

    /* close the server socket */
    sock_delete(server->sock);

    /* free the list */
    list_delete(server->http_connections, NULL);

    /* free the serverstruct */
    free(server);
}

/*! \brief Answer a pending request
 *
 * \param connection the connection of the request
 * \param msg is the content of the message
 * \param size is the size of the content
 */
void hs_answer_request(HttpConnection* connection,
                       char* msg, size_t size, const char* content_type) {
    char* header;

    /* create the header */
    header = make_http_head(200, size, content_type);

    /* send the header and the content */
    sock_send(connection->sock, header, strlen(header), 1);
    sock_send(connection->sock, msg, size, 0);

    /* clear the callback */
    connection->close_callback = NULL;
    connection->close_data = NULL;
}

