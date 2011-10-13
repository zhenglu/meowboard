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

#include "ChessMatchAnnouncement.hh"
#include "GameChess.hh"
#include "MatchChess.hh"


ChessMatchAnnouncement::ChessMatchAnnouncement(const std::vector<XML::Tag>& players, const XML::AttributeMap& __attributes) :
	_players(MatchChess::getPlayersFromXML(players)),
	_match_players(players),
	_attributes(__attributes) {

	if(_attributes.find("color") == _attributes.end())
		this->_players[0].color=UNDEFINED;
}

Game* ChessMatchAnnouncement::createGame(const XMPP::Jid& player) const {
	std::vector<XML::Tag> __match_players(_match_players);

	XML::Tag other_player("player");
	other_player.attributes()=__match_players[0].attributes();
	other_player.attributes().erase("color");
	other_player.attributes()["jid"]=player.full();


	__match_players.push_back(other_player);

	std::vector<GamePlayer> __players=MatchChess::getPlayersFromXML(__match_players);
	if(this->category() == "Untimed")
		return new GameChessUntimed(__players,this->_attributes);
	return new GameChess(__players,this->_attributes);
}

XML::Tag* ChessMatchAnnouncement::notification() const {
	XML::TagGenerator t;
    t.openTag("announcement");

    foreach(it,this->_attributes)
        t.addAttribute(it->first,it->second);

    foreach(it,this->_match_players)
        t.addChild(new XML::Tag(*it));

    t.closeTag();
    return t.getTag();
}
