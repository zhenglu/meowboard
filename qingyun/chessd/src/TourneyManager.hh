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

#ifndef TOURNEYMANAGER_HH
#define TOURNEYMANAGER_HH

#include <map>
#include <set>
#include <memory>
#include <boost/ptr_container/ptr_map.hpp>

#include <stdint.h>

#include "Tourney.hh"

#include "ServerModule.hh"

#include "ServerCore.hh"
#include "DatabaseManager.hh"

/*! \brief This class manages all the matchs int the server. */
class TourneyManager : public ServerModule {
    public:
        /*! \brief Constructor
         *
         * \param core_interface is the interface to the core.
         * \param config is the configuration for this component.
         */
        TourneyManager(
                ServerCore& game_manager,
                DatabaseManager& database_manager,
                const XMPP::StanzaHandler& send_stanza);

        /*! \brief Destructor
         *
         * Closes server connection if available
         */
        ~TourneyManager();

        std::vector<std::string> namespaces() const;

    private:

        /*! \brief Handle an incoming tourney iq. */
        void handleIq(const XMPP::Stanza& query);

        /*! \brief Handle an incoming request to create a tourney. */
        void handleCreate(const XMPP::Stanza& query);

        /*! \brief Handle an incoming request to list all tourneys. */
        void handleList(const XMPP::Stanza& query);

        /*! \brief Handle an incoming request to join a tourney. */
        void handleJoin(const XMPP::Stanza& query);

        /*! \brief Start the tourney */
        void startTourney(uint64_t tourney_id);

        /*! \brief Start a tourney's next round */
        void startNextRound(uint64_t tourney_id);

        /*! \brief Notify a game tourney to the players.
         *
         * This is just a tunnel to the real one
         * */
        void notifyGame(uint64_t tourney_id,
                const std::vector<GamePlayer>& players,
                const XMPP::Jid& game_room);

        /*! \brief Notify a game tourney to the players. */
        void _notifyGame(uint64_t tourney_id,
                const std::vector<GamePlayer>& players,
                const XMPP::Jid& game_room);

        /*! \brief Take a result of a tourney game
         *
         * This is a tunnel to the real one
         * */
        void reportResult(uint64_t tourney_id,
                int game_id,
                const std::vector<GamePlayerResult>& results);

        /*! \brief Take a result of a tourney game */
        void _reportResult(uint64_t tourney_id,
                int game_id,
                const std::vector<GamePlayerResult>& results);

        /*! \brief Receive a close notification. */
        void onClose();

        /*! \brief Receive a error notification. */
        void onError(const std::string& error);

        struct TourneyStatus {
            std::string name;
            std::string description;
            Util::Time start_time;
            bool running;
            XMPP::Jid owner;
            std::auto_ptr<Tourney> tourney;
        };

        /*! \brief Team database */
        boost::ptr_map<uint64_t, TourneyStatus> tourneys;

        uint64_t tourney_ids;

        ServerCore& game_manager;

        DatabaseManager& database;

        XMPP::ErrorHandler handleError;
};

#endif
