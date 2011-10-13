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

#ifndef TOURNEYFACTORY_HH
#define TOURNEYFACTORY_HH

#include "ChessTourney.hh"
#include "XML/Xml.hh"

struct TourneyFactory {
	public:
		static Tourney* create(const XML::Tag& offer) {
			//test category
			return new ChessTourney(offer.getAttribute("category"),
                                    Util::Time::Seconds(offer.getAttribute("time")),
                                    Util::Time::Seconds(offer.getAttribute("inc")),
                                    Util::parse_string<int>(offer.getAttribute("rounds")));
			/*
			 * throw ;4
			*/
			return 0;
		}
	private:
};


#endif
