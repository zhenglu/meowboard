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


#ifndef LOG_H
#define LOG_H

#include <iksemel.h>

#define DEBUG 0
#define INFO 1
#define WARNING 2
#define ERROR 3

#define log(level, ...) _log(__func__, level, __VA_ARGS__)

void log_init(iks* config);

void log_quit();

void _log(const char* function_name, int level, const char* format, ...)
    __attribute__ ((format (printf, 3, 4)));


#endif
