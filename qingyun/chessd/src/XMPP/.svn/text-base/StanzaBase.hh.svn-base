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

#ifndef XMPPSTANZABASE_HH
#define XMPPSTANZABASE_HH

#include <string>
#include "Jid.hh"
#include "XML/Xml.hh"
#include "Move.hh"

namespace XMPP {

	class StanzaBase {
		public:
			StanzaBase(const std::string& type) : _type(type) { }

			StanzaBase(const StanzaBase& stanza_base);

			StanzaBase(Moved<StanzaBase> stanza_base);

			~StanzaBase() { }

			std::string& type() { return this->_type; }
			const std::string& type() const { return this->_type; }

			std::string& subtype() { return this->_subtype; }
			const std::string& subtype() const { return this->_subtype; }

			std::string& id() { return this->_id; }
			const std::string& id() const { return this->_id; }

			Jid& from() { return this->_from; }
			const Jid& from() const { return this->_from; }

			Jid& to() { return this->_to; }
			const Jid& to() const { return this->_to; }

			std::string& lang() { return this->_lang; }
			const std::string& lang() const { return this->_lang; }

		private:
			Jid _from, _to;
			std::string _type, _subtype, _lang, _id;
	};

}
#endif
