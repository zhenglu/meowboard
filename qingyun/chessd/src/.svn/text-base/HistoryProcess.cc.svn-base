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

#include "HistoryProcess.hh"
#include "XML/TagGenerator.hh"
#include "XML/Exception.hh"
#include "Util/utils.hh"
#include <sstream>

XML::Tag* HistoryProcess::generate(XML::Tag* history_tag) {
	std::string category=history_tag->getAttribute("category");
	if(category=="standard" or category=="blitz" or category=="lightning" or category=="untimed")
		return ChessHistoryProcess::generate(history_tag);
	return 0;
}

XML::Tag* ChessHistoryProcess::generate(XML::Tag* history_tag) {
	Xiangqi chess;
	XML::TagGenerator new_history;
	std::string category=history_tag->getAttribute("category");
	int new_time;
	int initial_time=-1;

	/*
	 * position 0 is white
	 * position 1 is black
	*/
	std::vector<XML::Tag*> players(2);

	foreach(it,history_tag->tags()) {
		if(it->name()=="player") {
			int p=((it->getAttribute("color")==std::string("white"))?0:1);
			players[p]=new XML::Tag(*it);
			players[p]->attributes().erase("score");
			//FIXME workarround to get initial time
			if(category!="untimed") {
				if(players[p]->hasAttribute("time")==false) {
					std::stringstream s(history_tag->findTag("moves").getAttribute("movetext"));
					std::string tmp;
					if(s >> tmp >> tmp) {
						players[p]->attributes()["time"]=tmp;
					}
				}
				initial_time=Util::parse_string<int>(players[p]->attributes()["time"]);
			}
//			players[p]->attributes().erase("time");
		}
	}

	new_history.openTag("history");
	new_history.addAttribute("category",category);
	{
		new_history.addChild(players[0]);
		new_history.addChild(players[1]);

		std::stringstream s(history_tag->findTag("moves").getAttribute("movetext"));
		std::string move;
		new_time=-1;
		new_history.addChild(generateStateTag(chess.getChessState(),initial_time,"",""));
		for(int i=0;s >> move;i++) {
			chess.makeMove(move);

			if(category!="untimed")
				s >> new_time;

			new_history.addChild(generateStateTag(chess.getChessState(),new_time,move,chess.PGNOfLastMove()));
		}
		new_history.closeTag();
	}
	delete history_tag;
	return new_history.getTag();
}

XML::Tag* ChessHistoryProcess::generateStateTag(const XiangqiState& state, int time, const std::string& mv, const std::string& short_mv) {
	XML::TagGenerator new_state;
	new_state.openTag("state");
	{
		new_state.addAttribute("board",state.boardFEN());
		new_state.addAttribute("turn",state.turn()==Xiangqi::WHITE?"white":"black");
/*			new_state.addAttribute("castle",state.castle);
		if(state.enpassant.x!=-1)
			new_state.addAttribute("enpassant",state.enpassant.toStringNotation());
		else
			new_state.addAttribute("enpassant","-");
		new_state.addAttribute("halfmoves",Util::to_string(state.halfmoves));
		new_state.addAttribute("fullmoves",Util::to_string(state.fullmoves));*/
		new_state.addAttribute("move",mv);
		new_state.addAttribute("short",short_mv);
		if(time>=0)
			new_state.addAttribute("time",Util::to_string(time));
		new_state.closeTag();
	}
	return new_state.getTag();
}
