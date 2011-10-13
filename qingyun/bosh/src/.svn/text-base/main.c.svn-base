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
#include <errno.h>
#include <unistd.h>

#include <iksemel.h>


#include "jabber_bind.h"
#include "socket_monitor.h"
#include "log.h"

int main(int argc, char** argv) {
    iks* config = 0;
	JabberBind* bind = 0;
    int ret;
    char* config_file;

    config_file = (argc<2) ? "config.xml" : argv[1];

    /* check if it is ok ti read the file */
    if(access(config_file, R_OK) != 0) {
        fprintf(stderr, "Could not read %s: %s.\n", config_file, strerror(errno));
        return 1;
    }

    ret = iks_load(config_file, &config);

    if(ret != IKS_OK) {
        fprintf(stderr, "Could not parse %s.\n", config_file);
        return 1;
    }

    /* init the socket monitor */
    sm_init();

    bind = jb_new(config);

    iks_delete(config);

	if(bind == NULL) {
        fprintf(stderr, "Failed to start service.\n");
		return 1;
	}

	jb_run(bind);

	jb_delete(bind);

    sm_quit();

    log_quit();

    return 0;
}
