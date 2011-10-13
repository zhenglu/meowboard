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

#ifndef HISTORY_HH
#define HISTORY_HH

#include "State.hh"

class History {
	private:
		std::vector<State*> allgame;

	public:
		History();

		History(State* est);

		virtual ~History();

		void putInHistory(State* est);

		const std::vector<State*>& getHistory() const ;

		const State* lastState() const ;

		int size() const;
};

class ChessHistory : public History {
	public:
		ChessHistory();

		const ChessState& operator[](int p) const ;

	private:
};

#endif
