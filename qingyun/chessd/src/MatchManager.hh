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

#ifndef MATCHMANAGER_HH
#define MATCHMANAGER_HH

#include <map>
#include <set>
#include <memory>
#include <boost/ptr_container/ptr_map.hpp>

#include "XMPP/Roster.hh"

#include "Match.hh"
#include "MatchDatabase.hh"
#include "TeamDatabase.hh"

#include "ServerModule.hh"

#include "ServerCore.hh"
#include "DatabaseManager.hh"

/*! \brief This class manages all the matchs int the server. */
class MatchManager : public ServerModule {
    public:
        /*! \brief Constructor
         *
         * \param core_interface is the interface to the core.
         * \param config is the configuration for this component.
         */
        MatchManager(ServerCore& game_manager,
                     DatabaseManager& database_manager,
                     const XMPP::StanzaHandler& sendStanza);

        /*! \brief Destructor
         *
         * Closes server connection if available
         */
        ~MatchManager();

        std::vector<std::string> namespaces() const;

    private:

        /*! \brief Receive a notification of a status change */
        void handleUserStatus(const XMPP::Jid& user_name,
                              const UserStatus& status);

        /*! process an incoming iq */
        void handleIq(const XMPP::Stanza& stanza);

        /*! \brief Handle an incoming match offer. */
        void handleOffer(const XMPP::Stanza& query);

        /*! \brief Handle an incoming match acceptance. */
        void handleAccept(const XMPP::Stanza& query);

        /*! \brief Handle an incoming match declinance. */
        void handleDecline(const XMPP::Stanza& query);

        /*! \brief Handle a request to list adjourned games */
        void handleList(const XMPP::Stanza& query);

        /*! \brief Process a match offer
         *
         * This function was part of handleOffer,
         * it was split due to the need to get an adjourned
         * game asynchronously */
        void processOffer(const XMPP::Stanza& stanza, Match* _match);

        /* ! \brief Resume a offer for an adjourned game
         *
         * This function is caleed after the adjourned
         * was read from the database
         * */
        void resumeOffer(int adj_id, const std::string& history);

        /*! \brief Notify the users of a match offer. */
        void notifyOffer(int id, const XMPP::Jid& requester);

        /*! \brief Notify the users of a match result. */
        void notifyResult(const Match& match, int id, bool accepted);

        /*! \brief Close a match. */
        void closeMatch(int id, bool accepted);

        /*! \brief Receive a game start notification.
         *
         * This is a tunnel to the real one.
         * */
        void notifyGameStart(int match_id, Match* match, const XMPP::Jid& jid);

        /*! \brief Receive a game start notification.
         *
         * This is the real one. This is required by the protocol.
         * The game room must be in the match result.
         * */
        void _notifyGameStart(int match_id, Match* match, const XMPP::Jid& jid);

        /*! \brief Send the iq result of a match offer */
        void sendOfferResult(const XMPP::Stanza& stanza, int match_id);

        /*! \brief List a player's adjourned games */
        void listAdjournedGames(const XMPP::Stanza& query, DatabaseInterface& database);

        /*! \brief Load an adjourned game from the database */
        void loadAdjourned(int game_id, DatabaseInterface& database);

        /*! \brief Team database */
        TeamDatabase teams;

        /*! \brief Pending offers */
        MatchDatabase match_db;

        uint64_t match_ids;

        ServerCore& game_manager;

        DatabaseManager& database;

        std::vector<std::pair<int, XMPP::Stanza*> > delayed_offer;
};

#endif
