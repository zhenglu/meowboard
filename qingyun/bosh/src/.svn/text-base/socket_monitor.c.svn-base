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


#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/epoll.h>

#include "socket_monitor.h"
#include "allocator.h"
#include "log.h"

#define MAX_SOCKETS (1024*16)
#define MAX_EVENTS 1024

static inline unsigned int hash_int(int i) {
    return i;
}

static inline int cmp_int(int i1, int i2) {
    return i1 == i2;
}

DECLARE_HASH(int, hash_int, cmp_int);
IMPLEMENT_HASH(int);

typedef struct SocketMonitor {
    int epoll_fd;
    int_hash* socket_hash;
    int socket_count;
} SocketMonitor;

struct SocketInfo {
    callback_t callback;
    void* user_data;
    int socket_fd;
    int events;
};

DECLARE_ALLOCATOR(SocketInfo);
IMPLEMENT_ALLOCATOR(SocketInfo);

static SocketMonitor* monitor = NULL;

SocketMonitor* sm_new() {
    SocketMonitor* monitor = malloc(sizeof(SocketMonitor));
    monitor->socket_count = 0;
    monitor->socket_hash = int_hash_new();

    /* according to the manual, the first argument is ignored,
     * so it doesn't really mater the value of MAX_SOCKETS */
    monitor->epoll_fd = epoll_create(MAX_SOCKETS);

    return monitor;
}

/*! \brief hash delete callback */
void socket_free_callback(int k, void* si) {
    SocketInfo_free(si);
}

void sm_delete(SocketMonitor* monitor) {
    /* close the epoll fd */
    close(monitor->epoll_fd);

    /* delete socket hash */
    int_hash_iterate(monitor->socket_hash, socket_free_callback);
    int_hash_delete(monitor->socket_hash);

    /* free monitor memory */
    free(monitor);
}

SocketInfo* sm_add_socket(int socket_fd, callback_t callback, void* user_data,
        int events) {

    struct epoll_event eevent;

    SocketInfo* si;

    /* check if the fd is already there */
    si = int_hash_find(monitor->socket_hash, socket_fd);
    if(si == NULL) {
        si = SocketInfo_alloc();
        int_hash_insert(monitor->socket_hash, socket_fd, si);
    }

    /* set parameters */
    si->callback = callback;
    si->user_data = user_data;
    si->events = events;
    si->socket_fd = socket_fd;

    /* add to epoll */
    monitor->socket_count++;
    memset(&eevent, 0, sizeof(eevent));
    eevent.events = events;
    eevent.data.ptr = si;
    epoll_ctl(monitor->epoll_fd, EPOLL_CTL_ADD, socket_fd, &eevent);

    return si;
}

void sm_add_events(SocketInfo* si, int events) {
    struct epoll_event eevent;

    /* set events in epoll */
    si->events |= events;
    memset(&eevent, 0, sizeof(eevent));
    eevent.events = si->events;
    eevent.data.ptr = si;
    epoll_ctl(monitor->epoll_fd, EPOLL_CTL_MOD, si->socket_fd, &eevent);
}

void sm_del_events(SocketInfo* si, int events) {
    struct epoll_event eevent;

    /* set events in epoll */
    si->events &= ~events;
    memset(&eevent, 0, sizeof(eevent));
    eevent.events = si->events;
    eevent.data.ptr = si;
    epoll_ctl(monitor->epoll_fd, EPOLL_CTL_MOD, si->socket_fd, &eevent);
}

void sm_del_socket(SocketInfo* si) {
    /* erase the socket from the epoll */
    epoll_ctl(monitor->epoll_fd, EPOLL_CTL_DEL, si->socket_fd, NULL);

    /* clear parameters */
    si->events = 0;
    si->callback = NULL;
    si->user_data = NULL;

    /* decremente the socket count */
    monitor->socket_count--;

    /* we need to keep the fd in the hash
     * to prevent troubles if this function was called from sm_poll */
}

void sm_poll(time_type timeout) {
    struct epoll_event events[MAX_EVENTS];
    int ret, i;
    SocketInfo* si;

    log(INFO, "sockets = %d", monitor->socket_count);

    /* poll for events and call the callbacks */
    ret = epoll_wait(monitor->epoll_fd, events, MAX_EVENTS, timeout);
    if(ret > 0) {
        for(i = 0; i < ret; ++i) {
            si = events[i].data.ptr;
            /* if the callback is null the socket was removed already */
            if(si->callback != NULL) {
                log(INFO, "Event on socket %d", si->socket_fd);
                si->callback(events[i].events, si->user_data);
            }
        }
    } else if(ret < 0) {
        log(ERROR, "%s", strerror(errno));
    }
}

void sm_init() {
    monitor = sm_new();
}

void sm_quit() {
    sm_delete(monitor);
}
