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

#ifndef CHESSMATCHANNOUNCEMENT_HH
#define CHESSMATCHANNOUNCEMENT_HH

#include "MatchAnnouncement.hh"

struct ChessMatchAnnouncement : public MatchAnnouncement {
	public:

		ChessMatchAnnouncement(const std::vector<XML::Tag>& players, const XML::AttributeMap& __attributes);

		/*! \brief Destructor */
		virtual ~ChessMatchAnnouncement() { }

		/*! \brief The players involved in the match */
		virtual const std::vector<GamePlayer>& players() const { return _players; } ;

        /*! \brief Return the game category. */
		virtual const std::string& category() const { return _attributes.find("category")->second; };

        /*! \brief Create a game discribed by the match. */
		virtual Game* createGame(const XMPP::Jid& player) const;

		/*! \brief The offer notification */
		virtual XML::Tag* notification() const ;

	private:
		std::vector<GamePlayer> _players;

		std::vector<XML::Tag> _match_players;

		XML::AttributeMap _attributes;
};

#endif
