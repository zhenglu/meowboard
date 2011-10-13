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

#include "PartialJid.hh"

using namespace std;

namespace XMPP {

    PartialJid::PartialJid(const string& jid) {
        size_t arroba = jid.find('@');
        if(arroba != string::npos) {
            this->node() = jid.substr(0, arroba);
            arroba ++;
        } else {
            arroba = 0;
        }
        size_t barra = jid.find('/', arroba);
        if(barra == string::npos) {
            barra = jid.size();
        }
        this->domain() = jid.substr(arroba, barra - arroba);
    }

    PartialJid::PartialJid(const PartialJid& jid) : _node(jid.node()),
        _domain(jid.domain()) { }

    PartialJid::PartialJid(const std::string& node,
                           const std::string& domain) :
        _node(node),
        _domain(domain) { }

    PartialJid::PartialJid() { }

    PartialJid::~PartialJid() { }

    string PartialJid::full() const {
        string resp;
        if(not this->node().empty())
            resp = this->node() + "@";
        resp += this->domain();
        return resp;
    }

}
