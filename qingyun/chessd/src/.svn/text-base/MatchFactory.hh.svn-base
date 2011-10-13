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

#ifndef MATCHFACTORY_HH
#define MATCHFACTORY_HH

#include "Match.hh"
#include "TeamDatabase.hh"

class MatchAnnouncementFactory;

class MatchFactory {
	public:
		static Match* create(const XML::Tag& match_offer,
				const TeamDatabase& teams) ;

	private:
		static std::vector<XML::Tag> getPlayersTag(const XML::Tag& match_offer) ;

//		static StandardPlayerList getPlayersfromXML(const XML::Tag& _match_offer) ;

		static void validateXML(XML::Tag& _match_offer, int num_players) ;

		static bool isTimeValid(const XML::Tag& _player,const std::string& category) ;

		friend class MatchAnnouncementFactory;
};

#endif
