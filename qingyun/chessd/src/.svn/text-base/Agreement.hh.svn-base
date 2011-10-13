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

#ifndef AGREEMENT_HH
#define AGREEMENT_HH

#include "XMPP/Jid.hh"
#include <map>
#include <set>

/*! \brief A helper class to manage any kind of agreement betwen users */

class Agreements {
    public:
        Agreements();

        /*! \brief Insert a user and set its state to not agreed */
        void insert(const XMPP::Jid& jid);

        /*! \brief Set a user state to agreed on the given agreement */
        bool agreed(int agreement, const XMPP::Jid& jid);

        /*! \brief Returns how many users has not agreed */
        int left_count(int agreement) const;

        /*! \brief Returns how many users has agreed */
        int agreed_count(int agreement) const;

        /*! \brief Reset the class to initial state */
        void clear(int agreement);

        /*! \brief Returns then number of users in he agreement */
        int size() const { return this->users.size(); }
    private:
        typedef std::set<XMPP::Jid> UserSet;

        typedef std::set<const XMPP::Jid*> Agreement;

        typedef std::map<int, Agreement> AgreementMap;

        UserSet users;

        AgreementMap agreements;
};

#endif
