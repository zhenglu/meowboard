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


#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <time.h>
#include <errno.h>
#include <string.h>
#include <inttypes.h>
#include <ctype.h>
#include <unistd.h>

#include "log.h"

const char VERBOSE_LEVEL_NAME[][64] = {
    "DEBUG",
    "INFO",
    "WARNING",
    "ERROR"
};

struct {
    char* filename;
    FILE* file;
    uint64_t rotate_size;
    char* compression_command;
    int level;
} log_conf = {NULL, NULL, 0, NULL, ERROR};

void log_quit() {
    if(log_conf.filename != NULL) {
        free(log_conf.filename);
    } 
    if(log_conf.file != NULL && log_conf.file != stdout) {
        fclose(log_conf.file);
    }
    if(log_conf.compression_command != NULL) {
        free(log_conf.compression_command);
    }
}
void log_set_file(const char* filename) {
    log_conf.filename = NULL;

    if(filename == NULL) {
        /* if no log file given, log to stdout */
        fprintf(stderr, "No log file defined, log is set to standard output\n");
        log_conf.file = stdout;
    } else {
        /* open log file, if failed, log to stdout */
        log_conf.file = fopen(filename, "aw");
        if(log_conf.file == NULL) {
            fprintf(stderr, "Could not open %s: %s\n", filename,
                    strerror(errno));
            log_conf.file = stdout;
        } else {
            /* store the filename */
            log_conf.filename = strdup(filename);
        }
    }
}

void log_set_rotate(const char* rotate_size, const char* rotate_command) {
    char unit;
    int ret;

    /* set compression command */
    if(rotate_command == NULL || strlen(rotate_command) == 0) {
        log_conf.compression_command = NULL;
    } else {
        log_conf.compression_command = strdup(rotate_command);
    }

    /* parse rotate size */
    ret = sscanf(rotate_size, "%" PRIu64 " %c", &log_conf.rotate_size, &unit);
    if(ret != 2) {
        fprintf(stderr, "Unable to parse rotate size, log rotation"
                " is disabled\n");
        log_conf.rotate_size = 0;
    } else {
        if(tolower(unit) == 'b') {
            log_conf.rotate_size *= 1ull;
        } else if(tolower(unit) == 'k') {
            log_conf.rotate_size *= 1ull<<10;
        } else if(tolower(unit) == 'm') {
            log_conf.rotate_size *= 1ull<<20;
        } else if(tolower(unit) == 'g') {
            log_conf.rotate_size *= 1ull<<30;
        } else {
            fprintf(stderr, "Unable to parse rotate size, log"
                    " rotation is disabled\n");
            log_conf.rotate_size = 0;
        }
    }
}

void log_set_verbose(const char* level) {
    if(level == NULL) {
        log_conf.level = ERROR;
    } else if(strcmp(level, "ERROR") == 0) {
        log_conf.level = ERROR;
    } else if(strcmp(level, "WARNING") == 0) {
        log_conf.level = WARNING;
    } else if(strcmp(level, "INFO") == 0) {
        log_conf.level = INFO;
    } else if(strcmp(level, "DEBUG") == 0) {
        log_conf.level = DEBUG;
    } else {
        log_conf.level = ERROR;
    }

    log(INFO, "Set log level to %d", log_conf.level);
}

void log_init(iks* config) {
    /* set log file */
    log_set_file(iks_find_attrib(config, "filename"));

    /* set rotation parameters */
    log_set_rotate(iks_find_attrib(config, "rotate_size"),
                   iks_find_attrib(config, "compression_command"));

    /* set log verbose level */
    log_set_verbose(iks_find_attrib(config, "verbose"));
}

void log_rotate() {
    time_t t;
    char* new_name = NULL;
    char time_str[512];

    /* close current log file */
    fclose(log_conf.file);
    log_conf.file = NULL;

    /* put the date in the log filename */
    t = time(NULL);
    strftime(time_str, sizeof(time_str), "%Y-%m-%d-%H-%M-%S", localtime(&t));
    asprintf(&new_name, "%s-%s", log_conf.filename, time_str);
    rename(log_conf.filename, new_name);

    /* compress the log file */
#if 0
    char* argument = NULL;
    if(log_conf.compression_command != NULL) {
        if(fork() == 0) {
            /* prepare command line */
            asprintf(&argument, "%s '%s'", log_conf.compression_command,
                    new_name);
            execlp("sh", "sh", "-c", argument, NULL);
            perror("Error compressing log file");
            exit(1);
        }
    }
#endif

    /* open a new log file */
    log_conf.file = fopen(log_conf.filename, "aw");
    if(log_conf.file == NULL) {
        fprintf(stderr, "Could not open %s: %s\n", log_conf.filename,
                strerror(errno));
        log_conf.file = stdout;
    }

    free(new_name);
}

void _log(const char* function_name, int level, const char* format, ...) {
    time_t t;
    va_list args;
    char* new_format = NULL;

    char time_str[512];

    /* check verbosity level */
    if(level < log_conf.level) {
        return;
    }

    /* check output file */
    if(log_conf.file == NULL) {
        fprintf(stderr, "Log output not set.\n");
        log_conf.file = stderr;
    }

    /* create timestamp */
    t = time(NULL);
    strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", localtime(&t));

    /* modify the format received */
    asprintf(&new_format, "%s %s %s: %s\n", time_str, function_name,
            VERBOSE_LEVEL_NAME[level], format);

    /* print the message to the log */
    va_start(args, format);
    vfprintf(log_conf.file, new_format, args);
    fflush(log_conf.file);
    free(new_format);

    /* check if the file size reached the rotate size */
    if(log_conf.file != stdout && log_conf.rotate_size > 0) {
        if(ftell(log_conf.file) >= log_conf.rotate_size) {
            log_rotate();
        }
    }
}

