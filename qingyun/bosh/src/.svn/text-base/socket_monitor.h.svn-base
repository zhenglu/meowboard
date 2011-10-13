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


#ifndef SM_MONITOR_H
#define SM_MONITOR_H

#include <poll.h>

#include "time.h"
#include "hash.h"
#include "list.h"

typedef void (*callback_t)(int events, void* user_data);

typedef struct SocketInfo SocketInfo;

/*! \brief Init the socket monitor. */
void sm_init();

/*! \brief Quit the socket monitor. */
void sm_quit();

/*! \brief Add a socket to the monitor. */
SocketInfo* sm_add_socket(int socket_fd, callback_t callback, void* user_data,
        int events);

/*! \brief Add a events to be monitored */
void sm_add_events(SocketInfo* si, int events);

/*! \brief Don't monitor the given events anymore */
void sm_del_events(SocketInfo* si, int events);

/*! \brief Remove a socket from the monitor. */
void sm_del_socket(SocketInfo* si);

/*! \brief Poll the sockets for any activity. */
void sm_poll(time_type max_time);

#endif
