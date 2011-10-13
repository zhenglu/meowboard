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

#ifndef BUGHOUSECHESS_HH
#define BUGHOUSECHESS_HH

#include "Chess.hh"
#include "ChessMove.hh"

class BugHouseChess : public Chess{
	public:
		BugHouseChess();

		virtual int verifyDraw() const;

		virtual bool verifyAndMakeMove(const std::string& move);

		virtual bool verifyCheckMate() const;

		int winner() const;

		const ChessPiece& eatenPiece() const;

		void putInPieces(const ChessPiece& p);

		const std::vector<ChessPiece>& pieces(int player) const;
	private:
		/*! \brief Verify if the Game is Draw*/
//		bool verifyDraw(int player) const;

		/*! \brief Verify if the player who made the move will be in check if the move is done*/
		bool willBeInCheck(const BugHouseMove& mv) const;

		/*! \brief Verify if the player given lost*/
		bool verifyCheckMate(int player) const;

		/*! \brief Verify is a given move is valid. This is a high level function*/
		bool verifyMove(const BugHouseMove& mv) const;

		/*! \brief make a given move and does not verify anything, just makes the move*/
		void makeMove(const BugHouseMove& mv) const; //FIXME this const is not good

		/*! \brief Considering the given move is valid, this function
		 * updates the state, history, board and turn */
		void updateMove(const BugHouseMove& mv);

		/*! \brief update The State, considering one move was made*/
		//FIXME sera que tah bom esse comeu??
		void updateState(const BugHouseMove& mv);

		ChessPiece eaten_piece;

		std::vector<std::vector<ChessPiece> > _pieces;
};

#endif
