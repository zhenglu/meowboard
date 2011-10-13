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

#ifndef SOCKET_H
#define SOCKET_H

#include <sys/types.h>

typedef enum SocketStatus {
    SOCKET_IDLE,
    SOCKET_CONNECTING,
    SOCKET_CONNECTED,
    SOCKET_LISTENING
} SocketStatus;

struct Socket;
typedef struct Socket Socket;

typedef void(*DataCallback)(void* user_data);
typedef void(*AcceptCallback)(void* user_data);
typedef void(*ErrorCallback)(void* user_data, int code);
typedef void(*ConnectCallback)(int success, void* user_data);

Socket *sock_new();

void sock_delete(Socket* sock);

int sock_connect(Socket* sock, const char* host, int port);

ssize_t sock_recv(Socket* sock, void *buffer, size_t len);

void sock_send(Socket* sock, void* buffer, size_t len, int more);

int sock_listen(Socket* sock, int port);

Socket* sock_accept(Socket* sock);

SocketStatus sock_status(Socket* sock);

int sock_fd(Socket* sock);

void sock_set_data_callback(Socket* sock, DataCallback callback,
        void* user_data);

void sock_set_accept_callback(Socket* sock, AcceptCallback callback,
        void* user_data);

void sock_set_connect_callback(Socket* sock, ConnectCallback callback,
        void* user_data);

void sock_set_error_callback(Socket* sock, ErrorCallback callback,
        void* user_data);

#endif
