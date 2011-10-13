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

#include "Disco.hh"
#include "Util/utils.hh"

using namespace std;
using namespace XML;


#define XMLNS_DISCO_INFO   "http://jabber.org/protocol/disco#info"
#define XMLNS_DISCO_ITEMS  "http://jabber.org/protocol/disco#items"

namespace XMPP {

	DiscoItem::DiscoItem(const std::string& name,
			const Jid& jid) :
		_name(name),
		_jid(jid) { }

	Disco::Disco(const StanzaHandler& sender,
			const std::string& name,
			const std::string& category,
			const std::string& type) :
		stanza_sender(sender),
		_name(name),
		_category(category),
		_type(type) { }

	Disco::~Disco() {
	}

	void Disco::handleIqInfo(const Stanza& stanza) {
        Stanza* result = stanza.createIQResult();
        TagGenerator generator;

        generator.openTag("query");
        generator.addAttribute("xmlns", XMLNS_DISCO_INFO);
        generator.openTag("identity");
        generator.addAttribute("category", this->category());
        generator.addAttribute("type", this->type());
        generator.addAttribute("name", this->name());
        generator.closeTag();

		foreach(feature, this->features()) {
            generator.openTag("feature");
            generator.addAttribute("var", *feature);
            generator.closeTag();
		}
        if(this->_ext_info.get() != 0) {
            generator.addChild(this->_ext_info->clone());
        }
        result->children().push_back(generator.getTag());
		this->stanza_sender(result);
	}

	void Disco::handleIqItems(const Stanza& stanza) {
        Stanza* result = stanza.createIQResult();
        TagGenerator generator;

        generator.openTag("query");
        generator.addAttribute("xmlns", XMLNS_DISCO_ITEMS);
		foreach(item, this->items()) {
            generator.openTag("item");
            generator.addAttribute("name", item->name());
            generator.addAttribute("jid", item->jid().full());
            generator.closeTag();
		}
        result->children().push_back(generator.getTag());
		this->stanza_sender(result);
	}
}
