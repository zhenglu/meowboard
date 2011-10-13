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

#include "BugHouse.hh"

BugHouse::BugHouse() {
	this->games=std::vector<BugHouseChess>(2);
	this->updateState();
	this->playerteam[0]=0;
	this->playerteam[1]=1;
	this->playerteam[2]=1;
	this->playerteam[3]=0;
}

int BugHouse::numberOfTurns(int game) const {
	return this->games[game].numberOfTurns();
}

int BugHouse::winner() const {
	for(int i=0;i<(int)games.size();i++)
		if(this->games[i].verifyCheckMate())
			return this->playerteam.find(games[i].winner()+i*2)->second;
	return -1;
}

int BugHouse::turn(int game) const {
	return this->games[game].turn();
}

bool BugHouse::verifyCheckMate() const {
	for(int i=0;i<(int)games.size();i++)
		if(this->games[i].verifyCheckMate())
			return true;
	return false;
}

int BugHouse::verifyDraw() const {
	//TODO
//	for(int i=0;i<games.size();i++)
//		if(this->games[i].verifyDraw())
//			return true;
	return 0;
}

const BugHouseState& BugHouse::getBugHouseState() const {
	return this->current_state;
}

bool BugHouse::verifyAndMakeMove(int player, const std::string& move) {
	int g=player/2;
	if(games[g].turn() == player%2) {
		if(games[g].verifyAndMakeMove(move)) {
			if(games[g].eatenPiece().type() != ChessPiece::NOTYPE)
				games[g].putInPieces(ChessPiece(games[g].eatenPiece().type(),(games[g].eatenPiece().color()+1)%2));
			updateState();
			return true;
		}
	}
	return false;
}

void BugHouse::updateState() {
	current_state[0]=games[0].getChessState();
	current_state[1]=games[1].getChessState();
}
