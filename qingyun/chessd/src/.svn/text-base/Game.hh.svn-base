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

#ifndef GAME_HH
#define GAME_HH

#include <map>
#include <string>
#include <vector>
#include "Team.hh"
#include "Rating.hh"
#include "XML/Xml.hh"
#include "Util/Timer.hh"

enum END_CODE {
    END_NO_REASON=0,

    END_WHITE_MATE=1,
    END_BLACK_MATE=2,

    END_STALEMATE=3,

	//time of white is over
    END_WHITE_TIME_OVER=4,
	//time of black is over
    END_BLACK_TIME_OVER=5,

    END_WHITE_RESIGNED=6,
    END_BLACK_RESIGNED=7,

    END_DRAW_AGREEMENT=8,
    END_DRAW_REPETITION=9,
    END_DRAW_50_MOVES=10,
    END_DRAW_IMPOSSIBLE_MATE=11,
    END_DRAW_TIME_OVER=12,

    END_CANCELED_AGREEMENT=13,
    END_CANCELED_TIMED_OUT=14,

    END_ADJOURNED=14,

    END_ADJOURNED_AGREEMENT=15,
    END_ADJOURNED_SHUTDOWN=16,

	END_WHITE_WO=17,
	END_BLACK_WO=18,
	END_BOTH_WO=19
};

static const char game_end_reason_table[][32] = {
    "no-reason",
    "white-mated",
    "black-mated",
    "stalemate",
    "white-timeover",
    "black-timeover",
    "white-resigned",
    "black-resigned",
    "draw-agreement",
    "draw-repetition",
    "draw-fifty-moves",
    "draw-impossible-mate",
    "draw-timeover",
    "canceled-agreement",
    "canceled-timed-out",
    "adjourned-agreement",
    "adjourned-shutdown",
    "white-wo",
    "black-wo",
    "both-wo"
};

class GameResult {
	public:
		virtual ~GameResult() { }
		
		/*! \brief The game category */
		virtual const std::string& category() const = 0;

		/*! \brief Return a code that says why the game has ended */
		virtual const END_CODE end_reason() const = 0;

		/*! \brief Returns a list containing all players in the game */
		virtual const std::vector<GamePlayerResult>& players() const = 0;

		/*! \brief Is the game rated? */
		virtual bool isRated() const = 0;

		/*! \brief Returns the game history */
		virtual XML::Tag* history() const = 0;

		/*! \brief Update the player ratings
		 *
		 * The given ratings must contain all players in the game.
		 * The map is modified to match new ratings.
		 * */
		virtual void updateRating(std::map<XMPP::Jid, Rating>& ratings) const = 0;
};

/*! \brief A class to represent a game that was adjourned */
class AdjournedGame {
	public:
		virtual ~AdjournedGame() { }

        /*! \brief Returns the game's history */
		virtual XML::Tag* history() const = 0;

        /*! \brief Returns the game's category */
        virtual const std::string& category() const = 0;

        /*! \brief Returns a list of players */
		virtual const std::vector<GamePlayer>& players() const = 0;
};

class Game {
	public:
		virtual ~Game() { }

		/*! \brief Current state of the game.
         *
         * \param current_time is the time counted from the begining of the game.
         */
		virtual XML::Tag* state(const Util::Time& current_time) const = 0;


        /*! \brief The list of players in the game */
        virtual const std::vector<GamePlayer>& players() const = 0;

		/*! \brief Current history of the game*/
		virtual XML::Tag* history() const = 0;

		/*! \brief The game category. */
		virtual const std::string& category() const = 0;

		/*! \brief A title for the game. */
		virtual const std::string& title() const = 0;

		/*! \brief A title for the game. */
		virtual bool isRated() const = 0;

		/*! \brief The player has resigned. */
		virtual void resign(const XMPP::Jid& player) = 0;

		/*! \brief The players have lost by W.O. */
		virtual void wo(const std::vector<XMPP::Jid>& wos) = 0;

		/*! \brief The player has called a flag. */
		virtual void call_flag(const Util::Time& current_time) = 0;

		/*! \brief Set the game as a draw. */
		virtual void draw() = 0;

		/*! \brief Adjourn the game. */
		virtual AdjournedGame* adjourn(const Util::Time& current_time) = 0;

		/*! \brief Has the game ended?  */
		virtual bool done(const Util::Time& current_time) = 0;

        /*! \brief Returns the game result. */
		virtual GameResult* result() const = 0;
		
		/*! \brief Make a move in the game.
		 *
         * \param player is the player who made the move.
		 * \param movement is the description of the movement.
         * \param time_stamp is the time of the move since the begining of the game.
         * \return A description of the move, it can be just like in the history.
         * \throw May throw invalid_move exception if the move is invalid.
         */
		virtual XML::Tag* move(const XMPP::Jid& player, const std::string& movement, const Util::Time& time_stamp) = 0;

};



#endif
