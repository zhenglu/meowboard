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

#ifndef PIECE_HH
#define PIECE_HH

class Piece {
	public:
		Piece(int type,int color);

		Piece();

		virtual ~Piece() { }

		int type() const;

		int color() const;

		virtual int pieceReal() const = 0;

	private:
		int _type;

		int _color;
};

class ChessPiece : public Piece {
	public:
		enum PieceType {
			NOTYPE=0,KING=1,QUEEN=2,ROOK=3,BISHOP=4,KNIGHT=5,PAWN=6
		};
		enum PieceColor {
			NOCOLOR=-1,WHITE=0,BLACK=1
		};

		ChessPiece(int __type, int __color);

		ChessPiece(char name);

		ChessPiece(const ChessPiece& cp);

		ChessPiece();

		static PieceType chartotype(char a);

		static PieceColor chartocolor(char a);

		virtual int pieceReal() const;

		bool operator==(const ChessPiece& cp) const;

	private:
};

#endif
