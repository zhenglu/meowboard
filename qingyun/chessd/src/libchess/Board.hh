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

#ifndef BOARD_HH
#define BOARD_HH

#include "Piece.hh"
#include "ChessMove.hh"

#include <vector>

class Board
{
	private:
	protected:
		int nlines,ncolums;
		std::vector<std::vector<Piece*> > board;
	public:
		Board(int n,int m); 

		virtual ~Board();

		const std::vector<Piece*>& operator[](int value) const;

		void createPiece(const Position& pos,Piece *pe);
		
		//TODO change the name of the three functions below
		/*Functions to get info about the Board*/
		int getType(const Position& p) const;

		int getPieceReal(const Position& p) const;

		int color(const Position& p) const;

		/*! \brief Mave a move, it just move pieces*/
		virtual void movePiece(const Position& from, const Position& to)=0;
};

class ChessBoard : public Board {
	public:
		ChessBoard(int n, int m);

		virtual ~ChessBoard();

		//Make a ChessMove
		virtual void movePiece(const Position& from, const Position& to);
	private:
};

#endif
