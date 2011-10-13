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

#ifndef XMPPSTANZA_HH
#define XMPPSTANZA_HH

#include <string>
#include "Jid.hh"
#include "XML/Xml.hh"
#include "StanzaBase.hh"
#include "ChildrenList.hh"

#include <boost/ptr_container/ptr_vector.hpp>
#include <stdexcept>

namespace XMPP {

	typedef boost::ptr_vector<XML::Tag> TagList;

	class Stanza : public StanzaBase {
		public:
			explicit Stanza(const std::string& type);

			explicit Stanza(XML::Tag* tag);

			explicit Stanza(Moved<XML::Tag> tag);

			explicit Stanza(Moved<StanzaBase> stanza_base);

			explicit Stanza(Moved<Stanza> stanza);

			Stanza(const Stanza& stanza);

			~Stanza();

			ChildrenList& children() { return this->_children; }
			const ChildrenList& children() const { return this->_children; }

			XML::Tag& findTag(const std::string& name) {
                foreach(tag, this->children().tags()) {
                    if(tag->name() == name)
                        return *tag;
                }
                throw XML::child_not_found("Child not found");
            }

			const XML::Tag& findTag(const std::string& name) const {
                foreach(tag, this->children().tags()) {
                    if(tag->name() == name)
                        return *tag;
                }
                throw XML::child_not_found("Child not found");
            }

			void clearChildren();

			std::string xml() const;

			/* this functions are helpers to create stanzas */
			static Stanza* createErrorStanza(Stanza* original,
					const std::string& error,
					const std::string& condition,
					const std::string& text = "",
                    const std::string& xmlns = "urn:ietf:params:xml:ns:xmpp-stanzas");

			static XML::Tag* createTag(Stanza* stanza);


			static Stanza* createIQResult(Stanza* stanza);

			Stanza* createIQResult() const;

            XML::Tag& query() { return this->findTag("query"); }
            const XML::Tag& query() const { return this->findTag("query"); }

            XML::Tag& firstTag() {
                if(this->children().tags().begin() == this->children().tags().end()) {
                    throw XML::child_not_found("Child not found");
                }
                return *this->children().tags().begin();
            }

            const XML::Tag& firstTag() const {
                if(this->children().tags().begin() == this->children().tags().end()) {
                    throw XML::child_not_found("Child not found");
                }
                return *this->children().tags().begin();
            }

		private:
		    ChildrenList _children;

	};

}
#endif
