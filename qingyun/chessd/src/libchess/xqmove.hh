
#ifndef XIANGQI_MOVE_HH
#define XIANGQI_MOVE_HH

#include <string>
#include "xqpiece.hh"
#include "ChessMove.hh"

class XiangqiMove : public Move {
	protected:
		Position _from;

		Position _to;

		int player;

	public:
		XiangqiMove(int player, const std::string& mv);
		
		XiangqiMove(const Position& start, const Position& end,const int j);

		XiangqiMove(const Move &mv);

		const Position& to() const ;

		const Position& from() const ;

		int color() const ;
};

#endif
