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

#ifndef SERVER_MODULE_HH
#define SERVER_MODULE_HH

#include <vector>
#include <string>

#include <stdint.h>

#include "XMPP/Stanza.hh"
#include "XMPP/Node.hh"
#include "XMPP/handlers.hh"

#include "Threads/Dispatcher.hh"

#include "UserStatus.hh"

class ServerModule {
    public:

        ServerModule(const XMPP::StanzaHandler& send_stanza);

        virtual ~ServerModule();

        /*! \brief Return the iq namespaces handled by this module */
        virtual std::vector<std::string> namespaces() const = 0;

        /*! \brief Notify the status of a user */
        void notifyUserStatus(const XMPP::Jid& user_name,
                              const UserStatus& status);

        /*! \brief process an incoming stanza */
        void handleStanza(const XMPP::Stanza& stanza);

        void start();

        void stop();

    protected:

        virtual void onStart();

        virtual void onStop();

        virtual void handleIq(const XMPP::Stanza& iq) = 0;

        /*! \brief Notify the status of a user */
        virtual void handleUserStatus(const XMPP::Jid& user_name,
                                      const UserStatus& status);

        void sendStanza(XMPP::Stanza* stanza);

        void sendIq(XMPP::Stanza* stanza,
                    const XMPP::StanzaHandler& on_result =
                        XMPP::StanzaHandler());

        bool isUserAvailable(const XMPP::Jid& user) const;

        bool isUserPlaying(const XMPP::Jid& user) const;

        bool isMultigameUser(const XMPP::Jid& user) const;

        bool canPlay(const XMPP::Jid& user) const;

        Threads::Dispatcher dispatcher;

    private:

        /*! \brief Notify the status of a user */
        void _notifyUserStatus(const XMPP::Jid& user_name,
                               const UserStatus& status);

        void _handleStanza(XMPP::Stanza* stanza);

        XMPP::StanzaHandler send_stanza;

        std::map<uint64_t, XMPP::StanzaHandler> iq_result_handlers;

        uint64_t iq_id;

        std::map<XMPP::Jid, UserStatus> users_status;
};


#endif
