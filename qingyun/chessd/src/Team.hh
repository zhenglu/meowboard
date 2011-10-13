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

#ifndef TEAM_HH
#define TEAM_HH

#include "XMPP/Jid.hh"
#include "Util/Timer.hh"

enum PLAYER_COLOR {
	WHITE=0,
	BLACK=1,
	UNDEFINED=-1
};

enum GAME_RESULT {
	WIN = 0,
    LOSE = 1,
    DRAW = 2,
    NORESULT = 3
};

const static char PLAYER_ROLE_NAME[][16] = {
    "white",
    "black"
};

const static char PLAYER_RESULT_NAME[][16] = {
    "won",
    "lost",
    "draw",
    "undefined"
};

/*! \brief All the info necessary for a player*/
struct GamePlayer {
	/* \brief creates a Player for Standard Chess
	 *
	 * \param jid: Jabber id from the player
	 * time:  The time for the Match
	 * inc: lag stuff
	 * color: the collor from the player in acord to StandardPlayerColor, it must be "black" or "white"
	 */
	GamePlayer(const XMPP::Jid &jid,
               const Util::Time &time,
               const Util::Time &inc,
               const PLAYER_COLOR color) :
		jid(jid),
		time(time),
		inc(inc),
		color(color) { }

    GamePlayer() : color(UNDEFINED) { }

	XMPP::Jid jid;
	Util::Time time, inc;
	PLAYER_COLOR color;
	/* \brief a Player is recognized by it's jid, so the less comparable signal is just comparing jids,
	 * considering that a player has a unique jid.
	 */
	bool operator <(const GamePlayer& p) const {
		return jid<p.jid;
	}
	bool operator ==(const GamePlayer& p) const {
		return jid==p.jid;
	}
};

struct GamePlayerResult {
    GamePlayerResult() : result(NORESULT) { }

	explicit GamePlayerResult(const GamePlayer& _player,
                              const GAME_RESULT _result = NORESULT) :
        player(_player),
        result(_result) { }

	GamePlayer player;
	GAME_RESULT result;
};
#endif
