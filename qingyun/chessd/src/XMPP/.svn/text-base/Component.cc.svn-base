/*
 *   Copyright (c) 2007-2008 C3SL.
 *
 *   This file is part of Chessd.
 *
 *   Chessd is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   Chessd is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 */

#include "Component.hh"
#include "Jid.hh"
#include "sha1.h"

#include <iksemel.h>
#include <memory>

using namespace XML;

enum STATUS {
    NOT_CONNECTED,
    WAITING_HANDSHAKE,
    CONNECTED
};

namespace XMPP {

    Component::Component(const std::string& component_name) :
        stream("jabber:component:accept"),
		component_name(component_name) { }

	Component::~Component() { }

	Stanza* Component::recvStanza(int max_time) {
        Tag* tag = this->stream.recvTag(max_time);
        if(tag != 0) {
            return new Stanza(tag);
        } else {
            return 0;
        }
	}

	void Component::sendStanza(Stanza* stanza) {
		if(stanza->from().empty())
			stanza->from() = Jid(this->component_name);
		this->stream.sendTag(Stanza::createTag(stanza));
	}

    void Component::connect(const std::string& host, int port, const std::string& password ) {
        std::auto_ptr<Tag> tag;

        this->stream.connect(host, port);

        tag = std::auto_ptr<Tag>(this->stream.recvTag(-1));

        if (tag->name() != "stream:stream") {
            throw "Authentication error: Invalid protocol";
        } else if(not tag->hasAttribute("id")) {
            throw "Authentication error: Invalid protocol";
        } else {
            std::string data = tag->getAttribute("id") + password;
            char hash[128];
            sha1_digest(data.c_str(), data.size(), hash);
            Tag* tag_tmp = new Tag("handshake");
            tag_tmp->children().push_back(new CData(hash));
            this->stream.sendTag(tag_tmp);
        }

        tag = std::auto_ptr<Tag>(this->stream.recvTag(-1));

        if(tag->name() != "handshake") {
            throw "Authentication error: Permission Denied";
        }
    }

    void Component::close() {
        this->stream.close();
    }
    
}
