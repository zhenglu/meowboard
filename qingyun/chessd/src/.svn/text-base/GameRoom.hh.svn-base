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

#ifndef GAMEROOM_HH
#define GAMEROOM_HH

#include "XMPP/Muc.hh"
#include "XMPP/Node.hh"
#include "XMPP/handlers.hh"
#include "Game.hh"
#include "Agreement.hh"
#include "DatabaseManager.hh"
#include "Threads/Dispatcher.hh"

enum GameStatus {
	GAME_RUNNING,
	GAME_ENDED
};

/*! \brief The callback used to notify a game result
 *
 * \param game_id is the game id in the database, it is -1 if the game was not
 *          inserted into the database
 * \param result are the results of each player
 */
typedef boost::function<void (int game_id, const std::vector<GamePlayerResult>& result)> OnGameEnd;

struct GameRoomHandlers {
    typedef boost::function<void ()> callback;
    XMPP::StanzaHandler send_stanza;
    callback close_game;
    callback hide_game;
    OnGameEnd report_result;
    GameRoomHandlers(const XMPP::StanzaHandler& send_stanza,
            const callback& close_game,
            const callback& hide_game,
            const OnGameEnd& report_result) :
        send_stanza(send_stanza),
        close_game(close_game),
        hide_game(hide_game),
        report_result(report_result) { }
};

/*! \brief A game room. */
class GameRoom : public XMPP::Muc {
    public:
        /*! \brief Constructor.
         *
         * \param game is the game being played.
         * \param room_name is the room's jid.
         * \param database_manager is an referene to the database.
         * \param handlers are the functions needed by the room
         */
        GameRoom(Game* game,
                const XMPP::Jid& room_name,
                DatabaseManager& database_manager,
                const GameRoomHandlers& handlers);

        /*! \brief Destructor. */
        ~GameRoom();

        virtual void handleStanza(XMPP::Stanza* stanza) throw();

        const Game& game() const { return *this->_game; }

        void stop();

        bool isActive() const { return this->game_active; }

    private:
        enum GameRequest {
            REQUEST_DRAW,
            REQUEST_CANCEL,
            REQUEST_ADJOURN,
            REQUEST_DRAW_DECLINE,
            REQUEST_CANCEL_DECLINE,
            REQUEST_ADJOURN_DECLINE
        };

        enum GameEndType {
            END_TYPE_NORMAL = 0,
            END_TYPE_CANCELED = 1,
            END_TYPE_ADJOURNED = 2
        };

        void onStop();

        void _handleStanza(XMPP::Stanza* stanza);

        /*! \brief Handle an incoming game iq */
        void handleGame(XMPP::Stanza* stanza);

        /*! \brief Handle a move stana. */
        void handleMove(const XMPP::Stanza& stanza);

        /*! \brief Handle a resign stana. */
        void handleResign(const XMPP::Stanza& stanza);

        /*! \brief Handle a draw stana. */
        void handleDrawAccept(const XMPP::Stanza& stanza);

        /*! \brief Handle a draw-decline stana. */
        void handleDrawDecline(const XMPP::Stanza& stanza);

        /*! \brief Handle a cancel stanza. */
        void handleCancelAccept(const XMPP::Stanza& stanza);

        /*! \brief Handle a cancel-decline stanza. */
        void handleCancelDecline(const XMPP::Stanza& stanza);

        /*! \brief Handle a adjourn stanza. */
        void handleAdjournAccept(const XMPP::Stanza& stanza);

        /*! \brief Handle a adjourn-decline stanza. */
        void handleAdjournDecline(const XMPP::Stanza& stanza);

        /*! \brief Handle a state request. */
        void handleState(const XMPP::Stanza& stanza);

        /*! \brief Notify a request to the users. */
        void notifyRequest(GameRequest request, const XMPP::Jid& requester);

        /*! \brief Send the user the current game state. */
        void notifyState(const XMPP::Jid& jid);

        /*! \brief Send the user the game result. */
        void notifyResult(const XMPP::Jid& user);

        /*! \brief Notify a move to the users. */
        void notifyMove(XML::Tag* long_tag);

        /*! \brief End the game. */
        void endGame(GameEndType type, END_CODE end_code = END_NO_REASON);

        /*! \brief Receive a notification of a user in the muc. */
        void notifyUserStatus(const XMPP::Jid& jid, const std::string& nick, bool available);

        /*! \brief Handle a game iq. */
        void handleGameIq(const XMPP::Stanza& from);

        /*! \brief check timeouts */
        void checkTime();

        void setResult(const GameResult& result);

        void setExtendedInfo();

        XMPP::Stanza* createStateStanza();

        XMPP::Stanza* createResultStanza(const std::string& lang);

        XMPP::Stanza* createMoveStanza(XML::Tag* move_tag);

        void broadcastResultStanza();

        Util::Time currentTime();

        std::auto_ptr<Game> _game;

        END_CODE result_reason;

        std::vector<GamePlayerResult> players_result;

        XMPP::Jid room_jid;

        DatabaseManager& database_manager;

        Threads::Dispatcher dispatcher;

        GameRoomHandlers handlers;

        Agreements agreement;

        std::set<XMPP::Jid> all_players;

        volatile bool game_active;

        Util::Time start_time;

        int move_count;

        GameEndType end_type;

        std::map<XMPP::Jid, Util::Time> player_timeout;
};

#endif
