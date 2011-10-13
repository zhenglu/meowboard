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

#ifndef CHESSTOURNEY_HH
#define CHESSTOURNEY_HH

#include "Pairing/Tourney.hh"
#include "Tourney.hh"
#include "Util/utils.hh"
#include "Util/Timer.hh"
#include "Game.hh"
#include <vector>


class ChessTourney : public Tourney {
	public:
		ChessTourney (const std::string& __category, const Util::Time& _initial_time, const Util::Time& _inc, int rounds);

		virtual ~ChessTourney() { }

		virtual const TourneyPlayerList& players() const;

		virtual void addPlayer(const TourneyPlayer& p);

		virtual std::vector<Game*>* match();

		virtual void addResult(const std::vector<GamePlayerResult>& prl);

		int rounds() const { return this->_rounds;}

		int missing_rounds() const { return this->_missing_rounds;}

	protected:
		std::vector<Game*>* makeGames(const std::list<Pairing::Game>& games) ;

		std::string _category;

		Util::Time initial_time;
		
		Util::Time inc;
		
		TourneyPlayerList _players;

		XML::AttributeMap game_attributes;

	private:
		std::map<XMPP::Jid,int> player_map;

		std::set<XMPP::Jid> result_set;

		int _rounds;

		int _missing_rounds;

		Pairing::Tourney tourney;

		//Variable for control, because Pairing::ChessTourney requires a start that almost do 
		//the same thing as MakeAssignments
		bool tourney_started;

		bool missing_results;
};

#endif
