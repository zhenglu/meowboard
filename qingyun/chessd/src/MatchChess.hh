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

#ifndef MATCHCHESS_HH
#define MATCHCHESS_HH

#include "MatchFactory.hh"
#include "Match.hh"
#include "XML/Xml.hh"

class ChessMatchAnnouncemen;

struct MatchChess : public Match {
	public:
		MatchChess(const std::vector<XML::Tag>& players, const XML::AttributeMap& __attributes);

		virtual ~MatchChess();
		
		virtual const std::vector<GamePlayer>& players() const;

		virtual const std::string& category() const;

		virtual Game* createGame() const ;

		/*! \brief The offer notification */
		virtual XML::Tag* notification() const;

	protected:
		/*this is protected just for MatchChessAdjourn*/
		std::vector<XML::Tag> _match_players;

	private:
		XML::AttributeMap _attributes;

		std::vector<GamePlayer> _players;

		/*! \brief transform a Tag descriptions of a player to a player structure
		 *!	\description if the color of the players were not chosen, then this
		 *function will choose the color randomsly
		*/
		static std::vector<GamePlayer> getPlayersFromXML(const std::vector<XML::Tag>& players);

		friend class ChessMatchAnnouncement;

};

struct MatchChessAdjourn : public MatchChess {
	public:
		MatchChessAdjourn(XML::Tag* _history) : MatchChess(this->getPlayersTag(*_history),_history->attributes()), history(_history) { }

		virtual Game* createGame() const;

		virtual XML::Tag* notification() const;
	private:
		static std::vector<XML::Tag> getPlayersTag(const XML::Tag& history_adjourn);

		XML::Tag* history;
};

#endif
