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

#include "History.hh"

History::History() {
	this->allgame.clear();
}

History::History(State* est) {
	this->allgame.clear();
	this->putInHistory(est);
}

History::~History() {
	for(int i=0;i<(int)allgame.size();i++)
		delete allgame[i];
	allgame.clear();
}

void History::putInHistory(State* est) {
	allgame.push_back(est);
}

const std::vector<State*>& History::getHistory() const {
	return this->allgame;
}

const State* History::lastState() const {
	return allgame[allgame.size()-1];
}

int History::size() const {
	return allgame.size();
}

ChessHistory::ChessHistory() : History() {
}

const ChessState& ChessHistory::operator[](int p) const {
	return *static_cast<ChessState*>(this->getHistory()[p]);
}
