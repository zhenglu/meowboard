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

#ifndef MATCHANNOUNCEMENT_HH
#define MATCHANNOUNCEMENT_HH

#include <vector>
#include <string>

#include "Team.hh"
#include "Game.hh"

struct MatchAnnouncement {
	public:

		/*! \brief Destructor */
		virtual ~MatchAnnouncement() { }

		/*! \brief The players involved in the match */
		virtual const std::vector<GamePlayer>& players() const = 0;

        /*! \brief Return the game category. */
		virtual const std::string& category() const = 0;

        /*! \brief Create a game discribed by the match. */
		virtual Game* createGame(const XMPP::Jid& player) const = 0;

		/*! \brief The offer notification */
		virtual XML::Tag* notification() const = 0;

};

#endif
