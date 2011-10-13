
#ifndef XIANGQI_BASEDGAME_HH
#define XIANGQI_BASEDGAME_HH

#include "xqstate.hh"
#include "xqmove.hh"
#include "xqhistory.hh"
#include "xqpiece.hh"
#include "BoardGame.hh" 
#include <vector>

class XiangqiBasedGame : public BoardGame {
	public:
		enum Color {
			WHITE=0,BLACK=1,UNDEFINED=-1
		};

		XiangqiBasedGame(int n, int m);

		~XiangqiBasedGame();

		virtual int numberOfTurns() const;

		const std::vector<State*>& getHistory() const;

		virtual int winner() const=0;

		virtual int turn() const=0;

		virtual bool verifyCheckMate() const=0;

		virtual int verifyDraw() const=0;

		virtual const XiangqiState& getChessState() const=0;

		virtual bool verifyAndMakeMove(const std::string &move)=0;

	protected:
		virtual bool verifyMove(const XiangqiMove& j) const=0;

		bool verifyPieceMove(const XiangqiMove& mv) const;

		/*! \brief create pieces according to board_fen 
		 * where board_fen means the 1st part of the FEN string. */
		void setState(const std::string& board_FEN) const;

		std::string getPosForFEN() const;

		/*! \brief Verify if a given position is being Atacked by 
		 * another piece of the opposite player */
		bool beingAttacked(const Position &onde,int opposite_player) const;

		/*! \brief Finds where the king of the given player is*/
		Position findKing(int jogador) const;

		/*! \brief Verify if the given player is in Check*/
		bool verifyCheck(int player) const;

		//All the positions returned by this functions are correct (tested by verifyMove)
		std::vector <Position> *getPositions(const Position& p) const;


	private:
        bool VerifyJiuGongGeMove(const XiangqiMove& mv) const ;

		/*! \brief Verify if between the move from and to is clean in a diagonal line*/
		bool VerifyDiagonal(const XiangqiMove& mv) const;

		/*! \brief Verify if between the move from and to is clean in a horizontal line*/
        /* in this function, it will not check if from.y == to.y*/
		bool VerifyHorizontal(const XiangqiMove& mv) const;

		/*! \brief Verify if between the move from and to is clean in a vertical line*/
        /* in this function, it will not check if from.x == to.x*/
		bool VerifyVertical(const XiangqiMove& mv) const;

		//These functions do not verify if the move resulted in a check position for the player who is making the move
		/*! \brief Verify if a Horse move is correct*/
		bool verifyHorseMove(const XiangqiMove& mv) const;

		//These functions do not verify if the move resulted in a check position for the player who is making the move

		/*! \brief Verify if a Pawn move is correct*/
		bool verifyPawnMove(const XiangqiMove& mv) const;
		//These functions do not verify if the move resulted in a check position for the player who is making the move

		/*! \brief Verify if a Rook move is correct*/
		bool verifyRookMove(const XiangqiMove& mv) const;
		//These functions do not verify if the move resulted in a check position for the player who is making the move

		/*! \brief Verify if a Advisor move is correct*/
		bool verifyAdvisorMove(const XiangqiMove& mv) const;
		//These functions do not verify if the move resulted in a check position for the player who is making the move

		/*! \brief Verify if a Elephant move is correct*/
		bool verifyElephantMove(const XiangqiMove& mv) const;
		//These functions do not verify if the move resulted in a check position for the player who is making the move

		/*! \brief Verify if a King move is correct*/
		bool verifyKingMove(const XiangqiMove& mv) const;
		//These functions do not verify if the move resulted in a check position for the player who is making the move

		/*! \brief Verify if a Cannon move is correct*/
		bool verifyCannonMove(const XiangqiMove& mv) const;

		//all possible position but some of them may be incorrect (not tested by verifyMove)
		std::vector <Position> *getpossiblePositions(const Position& p) const;

		std::vector <Position> *getVerticalandHorizontal(const Position& p) const;

		std::vector <Position> *getCannonNextStep(const Position& p) const;

};

#endif
