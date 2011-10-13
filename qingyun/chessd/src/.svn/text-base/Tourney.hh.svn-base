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

#ifndef TOURNEY_HH
#define TOURNEY_HH

#include "Game.hh"
#include <vector>

struct TourneyPlayer{
	XMPP::Jid jid;
	Rating rating;
	float score;
	TourneyPlayer(XMPP::Jid _jid, Rating _rating, float _score=0) : jid(_jid), rating(_rating), score(_score) { }
};

typedef std::vector<TourneyPlayer> TourneyPlayerList;

class Tourney {
	public:
		virtual ~Tourney() { }

		virtual const TourneyPlayerList& players() const=0;

		virtual void addPlayer(const TourneyPlayer& p)=0;

		virtual std::vector<Game*>* match()=0;

		virtual void addResult(const std::vector<GamePlayerResult>& prl)=0;
};

#endif
