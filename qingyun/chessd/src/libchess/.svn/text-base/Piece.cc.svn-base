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

#include <ctype.h>
#include <cstdlib>
#include "Piece.hh"

char TabelaChar[]=".KQRBNP";
/*
char TabelaName[][7]={
	"",
	"Rei",
	"Rainha",
	"Torre",
	"Bispo",
	"Cavalo",
	"Peao"
};
*/

Piece::Piece(int __type,int __color) : _type(__type), _color(__color) {
}

ChessPiece::ChessPiece(int __type, int __color) : Piece(__type,__color) {
}

ChessPiece::ChessPiece(char name) : Piece(chartotype(name),chartocolor(name)) {
}

ChessPiece::ChessPiece(const ChessPiece& cp) : Piece(cp.type(),cp.color()) {
}

ChessPiece::ChessPiece() : Piece(NOTYPE,NOCOLOR) {
}

int Piece::type() const {
	return this->_type;
}

int Piece::color() const {
	return this->_color;
}

ChessPiece::PieceType ChessPiece::chartotype(char a) {
	a=toupper(a);
	switch(a) {
		case '.':
			return NOTYPE;
		case 'K':
			return KING;
		case 'Q':
			return QUEEN;
		case 'R':
			return ROOK;
		case 'B':
			return BISHOP;
		case 'N':
			return KNIGHT;
		case 'P':
			return PAWN;
	}
	return NOTYPE;
}

ChessPiece::PieceColor ChessPiece::chartocolor(char name) {
	return (name=='.')?NOCOLOR:((name>='a') and (name<='z'))?BLACK:WHITE;
}

int ChessPiece::pieceReal() const {
	return (int)(this->color() == WHITE ? (TabelaChar[this->type()]) : (tolower(TabelaChar[this->type()])));
}

bool ChessPiece::operator==(const ChessPiece& cp) const {
	return this->pieceReal()==cp.pieceReal();
}
