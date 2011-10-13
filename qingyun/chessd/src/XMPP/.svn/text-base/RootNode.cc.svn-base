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

#include "RootNode.hh"
#include "Exception.hh"
#include <boost/bind.hpp>

using namespace XML;
using namespace std;

namespace XMPP {

	RootNode::RootNode(const StanzaHandler& send_stanza,
			const XMPP::Jid& jid,
			const std::string& name,
			const std::string& category,
			const std::string& type) :
		Node(send_stanza, jid, name, category, type)
    {
    }

	RootNode::~RootNode() { }

	void RootNode::handleStanza(Stanza* _stanza) throw() {
        std::auto_ptr<Stanza> stanza(_stanza);
        try {
            if(stanza->to().node().empty()) {
                Node::handleStanza(stanza.release());
            } else {
                HandlerMap::const_iterator it;
                it = this->node_handlers.find(stanza->to().node());
                if(it != this->node_handlers.end()) {
                    it->second(new Stanza(*stanza));
                } else {
                    throw item_not_found("Item not found");
                }
            }
        } catch (const xmpp_exception& error) {
            this->sendStanza(error.getErrorStanza(stanza.release()));
        }
	}

	void RootNode::setNodeHandler(const string& node, const StanzaHandler& handler) {
		this->node_handlers.insert(make_pair(node,handler));
	}

	void RootNode::removeNodeHandler(const string& node) {
		this->node_handlers.erase(node);
	}
}
