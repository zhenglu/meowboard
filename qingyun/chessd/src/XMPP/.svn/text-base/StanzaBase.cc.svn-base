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

#include "StanzaBase.hh"
#include <algorithm>

using namespace std;

using namespace XMPP;

StanzaBase::StanzaBase(const StanzaBase& stanza_base) :
	_from(stanza_base.from()),
	_to(stanza_base.to()),
	_type(stanza_base.type()),
	_subtype(stanza_base.subtype()),
	_lang(stanza_base.lang()),
	_id(stanza_base.id()) { }

StanzaBase::StanzaBase(Moved<StanzaBase> stanza_base) {
	swap(_from,stanza_base->from());
	swap(_to,stanza_base->to());
	swap(_type,stanza_base->type());
	swap(_subtype,stanza_base->subtype());
	swap(_lang,stanza_base->lang());
	swap(_id,stanza_base->id());
}
