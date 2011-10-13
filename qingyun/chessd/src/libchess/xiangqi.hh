

#ifndef XIANGQI_HH
#define XIANGQI_HH

#include "xqbasedgame.hh"

class Xiangqi : public XiangqiBasedGame {
	public:
		/*! \brief Constructor function that iniciates an Internation Xiangqi game */
		Xiangqi();

		/*! \brief Constructor function that iniciates an Internation Xiangqi game, given the FEN*/
		Xiangqi(const std::string& FEN);

		~Xiangqi();

		/*! \return the winner player*/
		virtual int winner() const ;

		/*! \brief return whose turn it is*/
		virtual int turn() const;

		/*! \brief A high-level function that make a move if the given move is correct
		 * \return true if the move is valid
		 * \return false if the move isn't valid
		 * */
		virtual bool verifyAndMakeMove(const std::string &mv);

		/*! \brief This functions consider the move is valid,
		 * does not check anything, just make the move specified
		 * by "mv". Don't use this method unless you know
		 * for sure the move is correct
		*/
		void makeMove(const std::string& mv);

		/*! \brief verify if a player has won*/
		virtual bool verifyCheckMate() const;

		/*! \brief verify if the game ended on a draw*/
		virtual int verifyDraw() const ;

		/*! \return return the current state*/
		virtual const XiangqiState& getChessState() const;

		static int countPieces(const std::string& fen);

		bool hasThePawnPromoted() const;

		const std::string& PGNOfLastMove() { return this->PGN_of_last_move;}
	protected:
		/*! \brief puts the new State on history*/
		void updateHistory();

		virtual void updateTurn();

		/*! \brief Verify if the player given lost*/
		bool verifyCheckMate(int player) const;

	private:
		/*! \brief Verify is a given move is valid. This is a high level function*/
		virtual bool verifyMove(const XiangqiMove& mv) const;

		bool willBeInCheck(const XiangqiMove& mv) const;

		bool madeCheckMate(const XiangqiMove& mv) const;

		/*! \brief make a given move and does not verify anything, just makes the move*/
		void makeMove(const XiangqiMove &mv) const; //FIXME this const is not good

		/*! \brief Considering the given move is valid, this function
		 * updates the state, history, board and turn */
		void updateMove(const XiangqiMove &mv);

		/*! \brief update The State, considering one move was made*/
		void updateState(const XiangqiMove& mv);

		bool verifyCastle(const XiangqiMove& mv) const;

		/*! \brief Verify if the Game is Draw by stalemate*/
		bool verifyStaleMate(int player) const;

		/*! \brief Verify if the Game is Draw by three fold repetirion rule*/
		bool verifyThreefoldRepetition() const;

		/*! \brief Verify if the Game is Draw by impossibility of checkmate*/
		bool verifyImpossibilityOfCheckmate() const;

		std::vector<Position> howmanyCanMove(const Position& where, const XiangqiPiece& p) const;

		/*! \brief check if all others position different of "where" have different columns than "where"*/
		static bool differentColumns(Position where, const std::vector<Position>& pos);

		/*! \brief check if all others position different of "where" have different rows than "where"*/
		static bool differentRows(Position where, const std::vector<Position>& pos);

		/*! \transform a move to PGN notation*/
		std::string XiangqiMoveToPGN(const XiangqiMove& mv) const;

		virtual void putPieces();

		int _turn;


		std::string PGN_of_last_move;
};

#endif
