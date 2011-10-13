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

#ifndef CHESSBASEDGAME_HH
#define CHESSBASEDGAME_HH

#include "BoardGame.hh" 
#include <vector>

class ChessBasedGame : public BoardGame {
	public:
		enum Color {
			WHITE=0,BLACK=1,UNDEFINED=-1
		};

		ChessBasedGame(int n, int m);

		~ChessBasedGame();

		virtual int numberOfTurns() const;

		const std::vector<State*>& getHistory() const;

		virtual int winner() const=0;

		virtual int turn() const=0;

		virtual bool verifyCheckMate() const=0;

		virtual int verifyDraw() const=0;

		virtual const ChessState& getChessState() const=0;

		virtual bool verifyAndMakeMove(const std::string &move)=0;

	protected:
		virtual bool verifyMove(const ChessMove& j) const=0;

		bool verifyPieceMove(const ChessMove& mv) const;

		void setState(const std::string& FEN) const;

		std::string getPosForFEN() const;

		/*! \brief Verify if a given position is being Atacked by 
		 * another piece of the opposite player */
		bool beingAttacked(const Position &onde,int jogador) const;

		/*! \brief Finds where the king of the given player is*/
		Position findKing(int jogador) const;

		/*! \brief Verify if the given player is in Check*/
		bool verifyCheck(int player) const;

		//All the positions returned by this functions are correct (tested by verifyMove)
		std::vector <Position> *getPositions(const Position& p) const;

		/*! \brief Verify if a a Pawn made EnPassant*/
		bool verifyEnPassant(const ChessMove& mv) const;

	private:
		/*! \brief Verify if between the move from and to is clean in a diagonal line*/
		bool VerifyDiagonal(const ChessMove& mv) const;

		/*! \brief Verify if between the move from and to is clean in a horizontal line*/
		bool VerifyHorizontal(const ChessMove& mv) const;

		/*! \brief Verify if between the move from and to is clean in a diagonal line*/
		bool VerifyVertical(const ChessMove& mv) const;

		//These functions do not verify if the move resulted in a check position for the player who is making the move
		/*! \brief Verify if a Horse move is correct*/
		bool verifyHorseMove(const ChessMove& mv) const;

		/*! \brief Verify if a Pawn move is correct*/
		bool verifyPawnMove(const ChessMove& mv) const;

		/*! \brief Verify if a Rook move is correct*/
		bool verifyRookMove(const ChessMove& mv) const;

		/*! \brief Verify if a Bishop move is correct*/
		bool verifyBishopMove(const ChessMove& mv) const;

		/*! \brief Verify if a Queen move is correct*/
		bool verifyQueenMove(const ChessMove& mv) const;

		/*! \brief Verify if a King move is correct*/
		bool verifyKingMove(const ChessMove& mv) const;

		//all possible position but some of them may be incorrect (not tested by verifyMove)
		std::vector <Position> *getpossiblePositions(const Position& p) const;

		std::vector <Position> *getVerticalandHorizontal(const Position& p) const;

		std::vector <Position> *getDiagonals(const Position& p) const;
};

#endif
