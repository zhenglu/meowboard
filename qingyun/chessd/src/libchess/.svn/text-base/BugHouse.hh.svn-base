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
#include <vector>
#include <map>

/*
 * assumptions:
 * team A will be players 0 and 3
 * team B will be players 1 and 2
 * players 0 and 1 will be playing against each other
 * players 2 and 3 will be playing against each other
 * 0 and 2 will be white
 * 1 and 3 will be black
*/

class BugHouse {
	public:
		BugHouse();

		~BugHouse();
		
		/*! \brief return the number of turns of a game*/
		int numberOfTurns(int game) const;

		/*! \brief returns the team who have won*/
		int winner() const;

		/*! \brief returns the turn of game*/
		int turn(int game) const;

		/*! \brief verify if a checkmate has ocurred
		 * \return false if a checkmate hasn't ocurred
		 * \return true if a checkmate ocurred
		*/
		bool verifyCheckMate() const;

		/*! \brief verify if a draw has ocurred
		 * \return 0 if a checkmate hasn't ocurred
		 * \return >1 if a checkmate has ocurred
		*/
		int verifyDraw() const;

		/*! \brief get the current state of the game*/
		const BugHouseState& getBugHouseState() const;

		/*! \brief high-level function that executes a move
		 * \return false if the move was invalid
		 * \return true if the move was valid and the move was made
		*/
		bool verifyAndMakeMove(int player, const std::string& move);

	private:

		/*! \brief updates the state of the game, this function is
		 * only called after making a move */
		void updateState();

		/*a list of chess games*/
		std::vector<BugHouseChess> games;

		/*map for player->team*/
		std::map<int,int> playerteam;

		/*state of the game*/
		BugHouseState current_state;
};
