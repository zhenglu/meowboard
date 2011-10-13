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


#ifndef HTTP_H
#define HTTP_H

#define MAX_HTTP_FIELDS (64)

#define HTTP_LINE_SEP "\r\n"

#define HTTP_XML_CONTENT "text/xml"
#define HTTP_HTML_CONTENT "text/html"

typedef struct HttpField {
    char* name;
    char* value;
} HttpField;

typedef struct HttpHeader {
    char* type;
    char* path;
    HttpField fields[MAX_HTTP_FIELDS];
    int n_fields;
} HttpHeader;

void http_delete(HttpHeader* header);

HttpHeader* http_parse(const char* str);

char* make_http_head(int http_code, size_t data_size, const char* content_type);

const char* http_get_field(HttpHeader* header, const char* field);

#endif
