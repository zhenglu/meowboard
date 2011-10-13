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

#ifndef XMPPROSTER_HH
#define XMPPROSTER_HH

/*! \brief A XMPP roster implementation.
 */

#include "Jid.hh"
#include "Stanza.hh"
#include "handlers.hh"

#include <map>



namespace XMPP {

    typedef boost::function<void (bool available)> UserStatusHandler;
    typedef boost::function<void (const XMPP::Jid& jid, bool available)> ChangeStatusHandler;

    class Roster {
        public:

            /*! \brief Constructor 
             *
             * \param stanza_sender is the function used to send stanzas
             * \param handler is called on every status change
             * */
            Roster(const StanzaHandler& stanza_sender,
                    const ChangeStatusHandler& handler = ChangeStatusHandler());

            /*! \brief Destructor */
            ~Roster();

            /*! \brief Handle a presence stanza */
            void handlePresence(const Stanza& stanza);

            /*! \brief Insert a user status monitor
             *
             * Insert a handler to be called on user status change. Current
             * user status is immediately notified.
             * \param user is the user's jid.
             * \param handler is the handler function.
             */
            void insertUserMonitor(const Jid& user, const UserStatusHandler& handler);

            /*! \brief Remove a user monitor.
             *
             * \param user is the user's jid.
             */
            void removeUserMonitor(const Jid& user);

            /*! \brief Get the user availability
             *
             * \param user is the user's jid.
             * \return Returns the user availability.
             */
            bool isUserAvailable(const Jid& user) const;
        private:
            struct UserInfo {
                bool available;
                UserStatusHandler handler;
                UserInfo() : available(false) { }
                UserInfo(bool available) : available(available) { }
                UserInfo(UserStatusHandler handler) :
                    available(false),
                    handler(handler) { }
            };

            void setUserStatus(const Jid& user, bool available);

            typedef std::map<Jid, UserInfo> user_map;

            user_map users;

            StanzaHandler stanza_sender;

            ChangeStatusHandler status_handler;
    };
}

#endif
