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


#ifndef JABBER_BIND_H
#define JABBER_BIND_H

#include <iksemel.h>

struct JabberBind;

typedef struct JabberBind JabberBind;

JabberBind* jb_new(iks* config);

void jb_delete(JabberBind* jc);

void jb_run(JabberBind* jb);

#endif
