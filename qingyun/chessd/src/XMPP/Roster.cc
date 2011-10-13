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

#include "Roster.hh"

#include "Exception.hh"

using namespace std;

namespace XMPP {
    Roster::Roster(const StanzaHandler& stanza_sender, const ChangeStatusHandler& status_handler) :
        stanza_sender(stanza_sender),
        status_handler(status_handler) { }

    Roster::~Roster() { }

    void Roster::handlePresence(const Stanza& stanza) {
        if(stanza.subtype().empty()) {
            this->setUserStatus(stanza.from(), true);
        } else if(stanza.subtype() == "unavailable") {
            this->setUserStatus(stanza.from(), false);
        } else {
            throw bad_request("Invalid presence");
        }
    }

    void Roster::setUserStatus(const Jid& user, bool available) {
        this->status_handler(user, available);
        user_map::iterator it = this->users.find(user);
        if(it == this->users.end()) {
            this->users.insert(make_pair(user, UserInfo(available)));
        } else {
            it->second.available = available;
            if(not it->second.handler.empty())
                it->second.handler(available);
        }
    }

    void Roster::insertUserMonitor(const Jid& user, const UserStatusHandler& handler) {
        user_map::iterator it = this->users.find(user);
        if(it == this->users.end()) {
            this->users.insert(make_pair(user,UserInfo(handler)));
            handler(false);
        } else {
            it->second.handler = handler;
            it->second.handler(it->second.available);
        }
    }

    void Roster::removeUserMonitor(const Jid& user) {
        this->users.find(user)->second.handler.clear();
    }

    bool Roster::isUserAvailable(const Jid& user) const {
        user_map::const_iterator it = this->users.find(user);
        if(it == this->users.end()) {
            return false;
        } else {
            return it->second.available;
        }
    }
};
