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

#ifndef SERVERCORE_HH
#define SERVERCORE_HH

#include "GameRoom.hh"

#include <map>
#include <set>
#include <memory>
#include <boost/ptr_container/ptr_map.hpp>
#include <boost/ptr_container/ptr_vector.hpp>
#include <stdint.h>

#include "ComponentBase.hh"

#include "ServerModule.hh"

#include "DatabaseManager.hh"

#include "Threads/SafeObject.hh"


typedef uint32_t GameId;

typedef boost::function<void (const XMPP::Jid& game_room)> OnGameStart;

/*! \brief Manage all games in the server plus control the game component. */
class ServerCore : public ComponentBase {
    public:
        /*! \brief Constructor.
         *
         * \param config is the configuration for this component.
         */
        ServerCore(const XML::Tag& config,
                   DatabaseManager& database_manager,
                   const XMPP::ErrorHandler& handle_error);

        /*! \brief Destructor.
         *
         * Closes server connection if available.
         */
        ~ServerCore();

        /*! \brief Create a game
         *
         * \param game is the game to be inserted.
         * \param on_game_start is a notifier for the game jid.
         * \return Returns the jid of the game room
         */
        XMPP::Jid createGame(Game* game,
                const OnGameEnd& on_game_end = OnGameEnd());

    private:
        void onConnect();

        /*! \brief Handle an incoming iq */
        void handleIq(const XMPP::Stanza& stanza);

        /*! \brief Handle an incoming search request */
        void handleSearch(const XMPP::Stanza& stanza);

        /*! \brief Handle an incoming presence */
        void handlePresence(const XMPP::Stanza& stanza);

        /*! \brief Create a game. The real one.
         *
         * This one is not thread safe.
         */
        void _createGame(Game* game,
                GameId game_id,
                const OnGameEnd& on_game_end);

        /*! \brief Close a game room.
         *
         * This is a tunnel.
         * */
        void closeGame(GameId game_id);

        /*! \brief close a game room.
         *
         * This is the real one.
         * */
        void _closeGame(GameId game_id);

        /*! \brief Hide a game room.
         *
         * This is a tunnel.
         * */
        void endGame(GameId game_id, const std::vector<GamePlayer>& players);

        /*! \brief Hide a game room.
         *
         * This is the real one.
         * */
        void _endGame(GameId game_id, const std::vector<GamePlayer>& players);

        /*! \brief handle an error */
        void handleError(const std::string& error);

        /*! \brief Receive a close notification */
        void onClose();

        /*! \brief Receive an error notification */
        void onError(const std::string& msg);

        /*! \brief Notify all server modules the user status  */
        void notifyUserStatus(const XMPP::Jid& user_name, const UserStatus& status);

        std::string node_name;

        DatabaseManager& database_manager;

        Threads::SafeObject<std::map<XMPP::Jid, UserStatus> > users_status;

        boost::ptr_vector<ServerModule> modules;

        boost::ptr_map<GameId, GameRoom> game_rooms;

        XMPP::ErrorHandler handle_error;

        GameId game_ids;

};

#endif
