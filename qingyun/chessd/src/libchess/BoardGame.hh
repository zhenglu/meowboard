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

#ifndef BOARDGAME_HH
#define BOARDGAME_HH

#include "ChessMove.hh"
#include "Board.hh"
#include "Piece.hh"
#include "State.hh"
#include "History.hh"

class BoardGame {
	public:
		BoardGame(int n,int m) : nlines(n), ncolums(m) {}

		virtual ~BoardGame() {}

		virtual int winner() const=0;

		virtual int turn() const =0;

		virtual bool verifyAndMakeMove(const std::string &move)=0;

	protected:
		virtual void updateTurn()=0;

		mutable Board* gameboard;//FIXME mutable is horrible

		int nlines,ncolums;

		State* current_state;

		History* history;

	private:
};
#endif
