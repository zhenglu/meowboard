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

#ifndef XMPPHANDLERS_HH
#define XMPPHANDLERS_HH

#include "XML/Xml.hh"

#include "Stanza.hh"

#include <boost/function.hpp>

namespace XMPP {
	typedef boost::function<void (XML::Tag*)>               TagHandler;
	typedef boost::function<void (Stanza*)>                 StanzaHandler;
	typedef boost::function<void ()>                        TimeoutHandler;
	typedef boost::function<void (const std::string&)>      ErrorHandler;
    typedef boost::function<void (const Stanza& stanza)>    ConstStanzaHandler;
}


#endif
