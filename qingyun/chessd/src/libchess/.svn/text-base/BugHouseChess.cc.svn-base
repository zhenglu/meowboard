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

#include "BugHouseChess.hh"

BugHouseChess::BugHouseChess() : Chess()
{
	this->eaten_piece=ChessPiece('.');
	this->_pieces=std::vector<std::vector<ChessPiece> >(2);
}

int BugHouseChess::verifyDraw() const {
	//TODO
	return 0;
}

bool BugHouseChess::verifyAndMakeMove(const std::string& move) {
	if(move.size()>=4 and move[1]!='@') {
		return Chess::verifyAndMakeMove(move);
	}
	else if(move[1]=='@') {
		BugHouseMove mv(this->turn(),move);
		if(verifyMove(mv)==true) {
			updateMove(mv);
			return true;
		}
	}
	return false;
}

bool BugHouseChess::verifyCheckMate() const {
	return verifyCheckMate(0) or verifyCheckMate(1);
}

int BugHouseChess::winner() const {
	if(this->verifyCheckMate(0)==true)
		return 1;
	if(this->verifyCheckMate(1)==true)
		return 0;
	return -1;
}

const ChessPiece& BugHouseChess::eatenPiece() const {
	return this->eaten_piece;
}

void BugHouseChess::putInPieces(const ChessPiece& p) {
	if(p.type() != ChessPiece::NOTYPE) {
		ChessPiece aux(p.type(),(p.color()+1)%2);
		this->_pieces[aux.color()].push_back(aux);
	}
}
const std::vector<ChessPiece>& BugHouseChess::pieces(int player) const {
	return this->_pieces[player];
}

bool BugHouseChess::willBeInCheck(const BugHouseMove& mv) const {
	if(mv.hasPiece()) {
		this->gameboard->createPiece(mv.to(),new ChessPiece(mv.piece()));
		if(this->verifyCheck(mv.color()) == false) {
			this->gameboard->createPiece(mv.to(),new ChessPiece('.'));
			return false;
		}
		this->gameboard->createPiece(mv.to(),new ChessPiece('.'));
	} else {
		throw("this should not happen");
//		return Chess::willBeInCheck(mv);
	}
	return true;
}

//FIXME optmize this
//just test the positions around the king
bool BugHouseChess::verifyCheckMate(int player) const {
	if(Chess::verifyCheckMate(player)==true) {
		for(int i=0;i<8;i++)
			for(int j=0;j<8;j++) {
				if((*gameboard)[i][j]->type()==ChessPiece::NOTYPE) {
					this->gameboard->createPiece(Position(i,j),new ChessPiece(ChessPiece::QUEEN,player));
					if(this->verifyCheck(player) == false) {
						this->gameboard->createPiece(Position(i,j),new ChessPiece('.'));
						return false;
					}
					this->gameboard->createPiece(Position(i,j),new ChessPiece('.'));
				}
			}
		return true;
	}
	return false;
}

bool BugHouseChess::verifyMove(const BugHouseMove& mv) const {
	if(mv.hasPiece()) {
		if(std::find(_pieces[mv.color()].begin(),_pieces[mv.color()].end(),mv.piece()) == _pieces[mv.color()].end())
			return false;
		if(this->gameboard->getType(mv.to()) != ChessPiece::NOTYPE)
			return false;
		return willBeInCheck(mv)==false;
	}
	throw "this should not happen";
	return false;
	//return Chess::verifyMove(mv);
}

void BugHouseChess::makeMove(const BugHouseMove& mv) const {
	if(mv.hasPiece()) {
		ChessPiece* aux = new ChessPiece();
		*aux=mv.piece();
		this->gameboard->createPiece(mv.to(),aux);
	} else {
		throw "this should not happen";
		//Chess::makeMove(mv);
	}
}

void BugHouseChess::updateMove(const BugHouseMove& mv) {
	if(mv.hasPiece()) {
		this->eaten_piece=*(static_cast<ChessPiece*>((*this->gameboard)[mv.to().y][mv.to().x]));
		this->makeMove(mv);
		this->updateState(mv);
		Chess::updateHistory();
		Chess::updateTurn();
	} else {
		throw "this should not happen";
		//Chess::updateMove(mv);
	}
}

/*this function will only be called if you put a piece, so no need to test if the move has piece*/
void BugHouseChess::updateState(const BugHouseMove& mv) {
	ChessState* current_state = static_cast<ChessState*>(this->current_state);

	current_state->enpassant=Position(-1,-1);

	if(mv.color() == BLACK)
		current_state->fullmoves++;

	current_state->board_fen = this->getPosForFEN();

	current_state->halfmoves++;

	current_state->_turn = (this->current_state->_turn == WHITE ? BLACK : WHITE );
}
