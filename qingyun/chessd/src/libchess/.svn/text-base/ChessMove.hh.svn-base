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

#ifndef CHESSMOVE_HH
#define CHESSMOVE_HH

#include <string>
#include "Piece.hh"

class Position {
	private:
		union {
			int pos[2];
			struct {
				int x,y;
			};
		};

	public:
		std::string toStringNotation() const ;

		Position(const std::string& p);

		Position(int __x, int __y);

		Position() ; 

		bool operator ==(const Position& p) const;

		bool operator !=(const Position& p) const;

		Position operator +(const Position& p) const;
};

class Move {
	private:
		std::string _move;

	public:
		const std::string& move() const;

		Move();

		Move(const std::string& __move);
};

class ChessMove : public Move {
	protected:
		Position _from;

		Position _to;

		int player;

	public:
		ChessMove(int player, const std::string& mv);
		
		ChessMove(const Position& a, const Position& b,const int j);

		ChessMove(const Move &mv);

		const Position& to() const ;

		const Position& from() const ;

		int color() const ;
};

class BugHouseMove : public ChessMove {
	private:
		ChessPiece _piece;

	public:
		BugHouseMove(int player, const std::string& mv);

		bool hasPiece() const;

		const ChessPiece& piece() const;
};

#endif
