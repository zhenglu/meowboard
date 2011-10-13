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

#include "ServerModule.hh"

#include "Util/utils.hh"
#include "XMPP/Exception.hh"

using namespace std;
using namespace XML;
using namespace XMPP;
using namespace Util;

ServerModule::ServerModule(const StanzaHandler& send_stanza) :
    send_stanza(send_stanza),
    iq_id(0) {
        this->dispatcher.start();
    }

ServerModule::~ServerModule() {
    this->dispatcher.stop();
}

void ServerModule::notifyUserStatus(const Jid& user_name,
                                    const UserStatus& status) {
    this->dispatcher.queue(boost::bind(&ServerModule::_notifyUserStatus,
                this, user_name, status));
}

void ServerModule::_notifyUserStatus(const Jid& user_name,
                                    const UserStatus& status) {
    this->users_status[user_name] = status;
    this->handleUserStatus(user_name, status);
}

void ServerModule::sendStanza(Stanza* stanza) {
    this->send_stanza(stanza);
}

void ServerModule::sendIq(Stanza* iq, const StanzaHandler& on_result) {
    uint64_t id = this->iq_id++;
    iq->id() = to_string(id);
    if(not on_result.empty()) {
        this->iq_result_handlers.insert(make_pair(id, on_result));
    }
    this->sendStanza(iq);
}

void ServerModule::handleStanza(const Stanza& stanza) {
    this->dispatcher.queue(boost::bind(&ServerModule::_handleStanza,
                this, new Stanza(stanza)));
}

void ServerModule::_handleStanza(Stanza* _stanza) {
    auto_ptr<Stanza> stanza(_stanza);
    try {
        if(stanza->type() == "iq") {
            if(not this->isUserAvailable(stanza->from())) {
                throw not_acceptable("User is not allowed to send messages while not available");
            }
            if(stanza->type() == "result") {
                if(not stanza->id().empty()) {
                    uint64_t id = parse_string<uint64_t>(stanza->id());
                    if(this->iq_result_handlers.count(id) > 0) {
                        this->iq_result_handlers.find(id)->second(stanza.release());
                        this->iq_result_handlers.erase(id);
                    }
                }
            } else if(stanza->subtype() == "get" or stanza->subtype() == "set") {
                this->handleIq(*stanza);
            }
        }
    } catch (const xmpp_exception& error) {
        this->sendStanza(error.getErrorStanza(stanza.release()));
    }
}

bool ServerModule::isUserAvailable(const Jid& user) const {
    map<Jid, UserStatus>::const_iterator it = this->users_status.find(user);
    if(it == this->users_status.end()) {
        return false;
    }
    return it->second.available;
}

bool ServerModule::isUserPlaying(const Jid& user) const {
    map<Jid, UserStatus>::const_iterator it = this->users_status.find(user);
    if(it == this->users_status.end()) {
        return false;
    }
    return it->second.games_playing > 0;
}

bool ServerModule::isMultigameUser(const Jid& user) const {
    map<Jid, UserStatus>::const_iterator it = this->users_status.find(user);
    if(it == this->users_status.end()) {
        return false;
    }
    return it->second.multigame;
}

bool ServerModule::canPlay(const Jid& user) const {
    map<Jid, UserStatus>::const_iterator it = this->users_status.find(user);
    if(it == this->users_status.end()) {
        return false;
    }
    return it->second.canPlay();
}

void ServerModule::onStart() {
}

void ServerModule::onStop() {
}

void ServerModule::handleUserStatus(const Jid&, const UserStatus&) {
}

void ServerModule::start() {
    this->dispatcher.queue(boost::bind(&ServerModule::onStart, this));
}

void ServerModule::stop() {
    this->dispatcher.exec(boost::bind(&ServerModule::onStop, this));
}
