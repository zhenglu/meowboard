
#include "xqmove.hh"

using namespace std;

XiangqiMove::XiangqiMove(int player, const std::string &mv) : Move(mv) {
	int x,y;
	if(mv.size() >=4) {
		x=mv[0]-'a';
		y=mv[1]-'0';
		this->_from=Position(x,y);
		x=mv[2]-'a';
		y=mv[3]-'0';
		this->_to=Position(x,y);
		this->player=player;
	} else {
		this->_from=this->_to=Position(-1,-1);
	}
}

XiangqiMove::XiangqiMove(const Position& a, const Position& b,const int j) : Move(a.toStringNotation()+b.toStringNotation()) {
	this->_from=a;
	this->_to=b;
	this->player=j;
}

XiangqiMove::XiangqiMove(const Move &mv) : Move(mv.move()) {
	int x,y;
	std::string aux=mv.move();
	if(aux.size() >=4) {
		x=aux[0]-'a';
		y=aux[1]-'0';
		this->_from=Position(x,y);
		x=aux[2]-'a';
		y=aux[3]-'0';
		this->_to=Position(x,y);
		this->player=player;
	} else {
		this->_from=this->_to=Position(-1,-1);
	}
}

const Position& XiangqiMove::to() const {
	return this->_to;
}

const Position& XiangqiMove::from() const {
	return this->_from;
}

int XiangqiMove::color() const {
	return this->player;
}
