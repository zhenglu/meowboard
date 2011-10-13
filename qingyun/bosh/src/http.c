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
#include <ctype.h>

#include "http.h"

#define HTTP_HEADER "HTTP/1.1 %d %s\r\n" \
                        "Content-type: %s; charset=UTF-8\r\n" \
                        "Content-Length: %d\r\n" \
                        "\r\n"

void http_delete(HttpHeader* header) {
    int i;

    if(header->type != NULL) {
        free(header->type);
    }
    if(header->path != NULL) {
        free(header->path);
    }

    for(i = 0; i < header->n_fields; ++i) {
        free(header->fields[i].name);
        free(header->fields[i].value);
    }

    free(header);
}

/* split the string using delim as a separator */
int strsplit(const char* str, const char* end_str, char** parts,
        int n, char delim) {
    char** end_parts = parts + n;
    const char* tmp = str;
    int ret = 0;

    while(str != end_str && parts != end_parts) {
        tmp = str;
        for(;tmp!=end_str && *tmp!=delim;++tmp);
        *parts = strndup(str, tmp-str);
        ++parts; ++ret;
        for(;tmp!=end_str && *tmp==delim;++tmp);
        str = tmp;
    }

    return ret;
}

/* find any character containing in the string delim */
const char* strstrany(const char* str, const char* delim) {
    int i, N = strlen(delim);

    for(;;++str) {
        for(i = 0; i < N; ++i) {
            if(*str==delim[i]) {
                return str;
            }
        }
    }
    return NULL;
}

/* parse http header */
HttpHeader* http_parse(const char* str) {
    const char* end_line,* next_line;
    const char* colon;
    char* parts[3];
    int fail = 0;
    int i, n;
    HttpHeader* header;

    /* the http header ends with two line seps, if it is not there,
     * then is not complete */
    if(strstr(str, HTTP_LINE_SEP HTTP_LINE_SEP) == NULL &&
            strstr(str, "\n\n") == NULL) {
        return NULL;
    }
    
    /* alloc header struct */
    header = calloc(sizeof(HttpHeader), 1);

    /* read type */
    end_line = strstrany(str, HTTP_LINE_SEP);
    n = strsplit(str, end_line, parts, 3, ' ');
    if(n == 3) {
        header->type = parts[0];
        header->path = parts[1];
        parts[0] = parts[1] = NULL;
        str = end_line;
        while(isspace(*str)) ++str;
    } else {
        fail = 1;
    }

    /* free unused data */
    for(i = 0; i < n; ++i) {
        if(parts[i] != NULL) {
            free(parts[i]);
        }
    }


    /* read http fields */
    for(i = 0; i < MAX_HTTP_FIELDS && fail == 0; ++i) {
        end_line = strstr(str, "\n");
        if(end_line == NULL) {
            fail = 1;
            break;
        }
        next_line = end_line + 1;
        if(end_line > str && *(end_line-1) == '\r') {
            end_line--;
        }
        if(end_line == str)
            break;
        colon = strstr(str, ":");
        if(colon == NULL) {
            fail = 1;
            break;
        }
        header->fields[i].name = strndup(str, colon - str);
        header->fields[i].value = strndup(colon + 1, end_line - (colon + 1));
        header->n_fields ++;

        str = next_line;
    }


    if(fail) {
        http_delete(header);
        return NULL;
    } else {
        return header;
    }
}

char* make_http_head(int code, size_t data_size, const char* content_type) {
	char* msg;
	const char* code_msg;

	if(code == 200) {
		code_msg = "OK";
	} else {
		code_msg = "ERROR";
	}

	asprintf(&msg, HTTP_HEADER, code, code_msg, content_type, (int)data_size);

	return msg;
}

const char* http_get_field(HttpHeader* header, const char* field) {
    int i;

    for(i = 0;i < header->n_fields; ++i) {
        if(strcmp(field, header->fields[i].name) == 0)
            return header->fields[i].value;
    }

    return NULL;
}

