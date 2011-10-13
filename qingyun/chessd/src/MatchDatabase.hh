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

#ifndef MATCHDATABASE_HH
#define MATCHDATABASE_HH

#include "Match.hh"
#include "Util/IDSet.hh"
#include "XMPP/Jid.hh"

#include <map>
#include <set>

#include <boost/ptr_container/ptr_map.hpp>

/*! \brief A non-persistence database of the current matchs. */
class MatchDatabase {
	public:
		MatchDatabase();
		~MatchDatabase();

		/*! \brief Insert a match to the database */
		int insertMatch(Match* match);

		/*! \brief Inform that a player accepted a match */
		void acceptMatch(int match_id, const XMPP::Jid& player);

		/*! \brief Ask if there is a match with the given id */
        bool hasMatch(int match_id) const;

		/*! \brief Ask whether this player blongs to the match.  */
		bool hasPlayer(int match_id, const XMPP::Jid& player) const;

		/*! \brief Get all the matchs which the player is in.  */
		const std::set<int>& getPlayerMatchs(const XMPP::Jid& player);

		/*! \brief Removes the match and return the Match instance */
		Match* closeMatch(int match_id);

        /*! \brief Get a match by its id. */
		const Match& getMatch(int match_id) const;

        /*! \brief Get the ids of all active matchs */
        std::vector<int> getActiveMatchs() const;

        /*! \brief Ask whether everyone in the has accepted */
        bool isDone(int match_id) const;

        /*! \brief Replaces a match */
        void replaceMatch(int match_id, Match* match);
		
	private:

		struct MatchInfo {
			std::auto_ptr<Match> match;
			std::map<XMPP::Jid, bool> accepted_players;
			int pending_count;
			MatchInfo(Match* match);
		};

        MatchInfo& findMatchInfo(int match_id);
        const MatchInfo& findMatchInfo(int match_id) const;

		Util::IDSet match_ids;
		boost::ptr_map<int, MatchInfo> matchs;
		std::map<XMPP::Jid, std::set<int> > player_matchs;
};

#endif
