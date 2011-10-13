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

#include "MatchAnnouncementFactory.hh"

#include "ChessMatchAnnouncement.hh"

#include "MatchFactory.hh"

MatchAnnouncement* MatchAnnouncementFactory::create(const XML::Tag& offer, const TeamDatabase& teams) {
	XML::Tag offer2(offer);
	MatchFactory::validateXML(offer2,1);

	return new ChessMatchAnnouncement(MatchFactory::getPlayersTag(offer2),offer2.attributes());

}
