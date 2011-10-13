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

#include "MatchChess.hh"
#include "GameChess.hh"
#include "Util/utils.hh"
#include "GameException.hh"

/*
 * MatchChess stuff
*/

MatchChess::MatchChess(const std::vector<XML::Tag>& players, const XML::AttributeMap& __attributes) : 
	_match_players(players) ,
	_attributes(__attributes) ,
	_players(getPlayersFromXML(players))
{
}

MatchChess::~MatchChess(){
}

const std::vector<GamePlayer>& MatchChess::players() const {
	return this->_players;
}

const std::string& MatchChess::category() const {
	return this->_attributes.find("category")->second;
}

Game* MatchChess::createGame() const {
	if(this->category()!="untimed")
		return new GameChess(this->getPlayersFromXML(this->_match_players),this->_attributes);
	return new GameChessUntimed(this->getPlayersFromXML(this->_match_players),this->_attributes);
}

XML::Tag* MatchChess::notification() const {
	XML::TagGenerator t;
	t.openTag("match");
//	t.addAttribute("category",this->category());
	foreach(it,this->_attributes)
		t.addAttribute(it->first,it->second);
	foreach(it,this->_match_players)
		t.addChild(new XML::Tag(*it));
	t.closeTag();
	return t.getTag();
}

std::vector<GamePlayer> MatchChess::getPlayersFromXML(const std::vector<XML::Tag>& xml_players) {
	std::vector<GamePlayer> players;
	srand(time(NULL));
	int color=rand()%2;
	foreach(c_it,xml_players) {
		Util::Time time,inc;
		XMPP::Jid aux(c_it->getAttribute("jid"));
		if(c_it->hasAttribute("time"))
			time=Util::Time::Seconds(c_it->getAttribute("time"));
		if(c_it->hasAttribute("inc"))
			inc=Util::Time::Seconds(c_it->getAttribute("inc"));
		if(c_it->hasAttribute("color"))
			color=(c_it->getAttribute("color")=="white"?0:1);
		players.push_back(GamePlayer(aux,time,inc,(PLAYER_COLOR)color));
		color=(color+1)%2;
	}
	return players;
}

//MatchChessAdjourn stuff

std::vector<XML::Tag> MatchChessAdjourn::getPlayersTag(const XML::Tag& history_adjourn) {
	std::vector<XML::Tag> players;
	foreach(it,history_adjourn.tags())
		if(it->name()=="player")
			players.push_back(XML::Tag(*it));
	return players;
}

Game* MatchChessAdjourn::createGame() const {
	if(this->category()!="untimed")
		return new GameChess(this->history);
	return new GameChessUntimed(this->history);
}

XML::Tag* MatchChessAdjourn::notification() const {
	XML::TagGenerator t;
	t.openTag("match");
	t.addAttribute("category",this->category());
	t.addAttribute("type","adjourned");
	foreach(it,this->_match_players)
		t.addChild(new XML::Tag(*it));
	t.closeTag();
	return t.getTag();
}

