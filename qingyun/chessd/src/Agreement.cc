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

#include "Agreement.hh"

#include "Util/utils.hh"

using namespace std;

Agreements::Agreements() { }

void Agreements::insert(const XMPP::Jid& jid) {
    this->users.insert(jid);
}

bool Agreements::agreed(int id, const XMPP::Jid& jid) {
    /* find player */
    UserSet::iterator user = this->users.find(jid);
    if(user == this->users.end()) {
        return false;
    }

    /* find agreement */
    Agreement& agreement = this->agreements[id];
    
    /* insert player to the agreement */
    return agreement.insert(&*user).second;
}

int Agreements::agreed_count(int id) const {
    AgreementMap::const_iterator it = this->agreements.find(id);
    if(it == this->agreements.end()) {
        return 0;
    } else {
        return it->second.size();
    }
}

int Agreements::left_count(int id) const {
    return this->size() - this->agreed_count(id);
}

void Agreements::clear(int id) {
    this->agreements.erase(id);
}
