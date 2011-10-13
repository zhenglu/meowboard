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

#include "Stanza.hh"

#include "Util/utils.hh"

using namespace std;
using namespace XML;

namespace XMPP {

	Stanza::Stanza(const std::string& type) : StanzaBase(type) { }

	Stanza::Stanza(Moved<StanzaBase> stanza_base) : StanzaBase(stanza_base) { }

	Stanza::Stanza(const Stanza& stanza) :
		StanzaBase(stanza) {
		foreach(it, stanza.children())
			this->children().push_back(it->clone());
	}

	Stanza::Stanza(Moved<Stanza> stanza) :
		StanzaBase(move<StanzaBase>(*stanza)) {
		swap(this->children(), stanza->children());
	}

	Stanza::~Stanza() { }

	static bool is_cdata(const XML::Item& item) {
		return typeid(item) == typeid(XML::CData);
	}
	Stanza::Stanza(Tag* tag) : StanzaBase(tag->name()) {
		this->subtype().swap(tag->attributes()["type"]);
		this->to() = Jid(tag->attributes()["to"]);
		this->from() = Jid(tag->attributes()["from"]);
		this->lang().swap(tag->attributes()["xml:lang"]);
		this->id().swap(tag->attributes()["id"]);
		tag->children().erase_if(is_cdata);
		this->children().transfer(this->children().end(), tag->children());
		delete tag;
	}

	Stanza::Stanza(Moved<Tag> tag) : StanzaBase(tag->name()) {
		this->subtype().swap(tag->attributes()["type"]);
		this->to() = Jid(tag->attributes()["to"]);
		this->from() = Jid(tag->attributes()["from"]);
		this->lang().swap(tag->attributes()["xml:lang"]);
		this->id().swap(tag->attributes()["id"]);
		tag->children().erase_if(is_cdata);
		this->children().transfer(this->children().end(), tag->children());
	}

	Tag* Stanza::createTag(Stanza* stanza) {
		Tag* tag = new Tag(stanza->type());
		if(not stanza->subtype().empty())
			tag->attributes()["type"].swap(stanza->subtype());
		if(not stanza->lang().empty())
			tag->attributes()["xml:lang"].swap(stanza->lang());
		if(not stanza->from().empty())
			tag->attributes()["from"] = stanza->from().full();
		if(not stanza->to().empty())
			tag->attributes()["to"] = stanza->to().full();
		if(not stanza->id().empty())
			tag->attributes()["id"].swap(stanza->id());
		tag->children().transfer(tag->children().end(), stanza->children());
		delete stanza;
		return tag;
	}

	string Stanza::xml() const {
		string tmp;
		tmp += "<" + this->type();
		if(not this->from().empty())
			tmp += " from='" + this->from().full() + "'";
		if(not this->to().empty())
			tmp += " to='" + this->to().full() + "'";
		if(not this->subtype().empty())
			tmp += " type='" + this->subtype() + "'";
		if(not this->lang().empty())
			tmp += " xml:lang='" + this->lang() + "'";
		if(not this->id().empty())
			tmp += " id='" + this->id() + "'";
		tmp += ">";
        foreach(it, this->children())
			it->xml(tmp);
		tmp += "</" + this->type() + ">";
		return tmp;
	}

	Stanza* Stanza::createErrorStanza(Stanza* stanza,
			const std::string& error,
			const std::string& condition,
			const std::string& text,
            const std::string& xmlns
            ) {
        // "urn:ietf:params:xml:ns:xmpp-stanzas"
		stanza->to().swap(stanza->from());
		stanza->subtype() = "error";
		Tag* deferr = new Tag(condition);
		deferr->setAttribute("xmlns", xmlns);
		Tag* errort = new Tag("error");
		errort->setAttribute("type", error);
		errort->children().push_back(deferr);
		if(not text.empty()) {
			Tag* ttext = new Tag("text");
			ttext->setAttribute("xmlns", "xmlns");
			errort->children().push_back(ttext);
			errort->children().push_back(new CData(text));
		}
		stanza->children().push_back(errort);
		return stanza;
	}

	void Stanza::clearChildren() {
		this->children().clear();
	}

	Stanza* Stanza::createIQResult(Stanza* stanza) {
		swap(stanza->from(), stanza->to());
		stanza->subtype() = "result";
		stanza->clearChildren();
		return stanza;
	}

	Stanza* Stanza::createIQResult() const {
        Stanza* stanza = new Stanza("iq");
        stanza->to() = this->from();
        stanza->from() = this->to();
        stanza->id() = this->id();
		stanza->subtype() = "result";
		return stanza;
	}
}
