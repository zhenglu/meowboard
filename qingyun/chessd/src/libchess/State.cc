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

#include "State.hh"
#include "../Util/utils.hh"
#include <string>

State::State() {
}

State::State(const std::string& posfen) {
	this->board_fen=posfen;
}

const std::string& State::boardFEN() const {
	return this->board_fen;
}

int State::turn() const {
	return this->_turn;
}

/*The initial State of Chess*/
ChessState::ChessState() {
	this->castle=std::string("KQkq");
	this->enpassant=Position(-1,-1);
	this->halfmoves=0;
	this->fullmoves=1;
	this->_turn=ChessPiece::WHITE;
	this->board_fen=std::string("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR");
}

ChessState::ChessState(const std::string& FEN) : State(FEN.substr(0,FEN.find(' '))) {
	/*Interprets a FEN*/
	unsigned int begin=FEN.find(' ') + 1;

	if(FEN[begin]=='w')
		this->_turn = ChessPiece::WHITE;
	else if(FEN[begin]=='b')
		this->_turn = ChessPiece::BLACK;

	begin=FEN.find(' ',begin) + 1;

	this->castle=FEN.substr(begin,FEN.find(' ',begin));

	begin=FEN.find(' ',begin) + 1;

	if(FEN[begin]=='-')
		this->enpassant=Position(-1,-1);
	else
		this->enpassant=Position(FEN.substr(begin,FEN.find(' ',begin)));

	begin=FEN.find(' ',begin) + 1;

	this->halfmoves=Util::parse_string<int>(FEN.substr(begin,FEN.find(' ',begin)));

	begin=FEN.find(' ',begin) + 1;

	this->fullmoves=Util::parse_string<int>(FEN.substr(begin,FEN.size()));

}

std::string ChessState::FEN() const {
	std::string ans;
	ans=this->board_fen;
	ans+=" ";
	ans+=(char)(this->_turn == ChessPiece::WHITE ? 'w' : 'b');
	ans+=" ";
	ans+=this->castle;
	ans+=" ";
	if( this->enpassant == Position(-1,-1) )
		ans+='-';
	else
		ans+=this->enpassant.toStringNotation();
	ans+=" ";
	ans+=Util::to_string(this->halfmoves);
	ans+=" ";
	ans+=Util::to_string(this->fullmoves);
	return ans;
}

bool ChessState::operator==(const ChessState& s) const {
	return (this->board_fen == s.board_fen) and (this->castle == s.castle) and (this->enpassant == s.enpassant) and (this->_turn == s._turn);
}

BugHouseState::BugHouseState() {
	states=std::vector<ChessState>(2);
}

ChessState& BugHouseState::operator[](int v) {
	return states[v];
}
