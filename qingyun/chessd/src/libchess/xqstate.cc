
#include "xqpiece.hh"
#include "xqstate.hh"
#include "../Util/utils.hh"
#include <string>


/*The initial State of Chess*/
XiangqiState::XiangqiState() {
/* should_adapt_to_xq */
	this->fullmoves=1;
	this->_turn=XiangqiPiece::WHITE;
	this->board_fen=std::string("rnbakabnr/9/1c5c1/p1p1p1p1p/9/9/P1P1P1P1P/1C5C1/9/RNBAKABNR");
}

XiangqiState::XiangqiState(const std::string& FEN) : State(FEN.substr(0,FEN.find(' '))) {
	/*Interprets a FEN*/
	unsigned int begin=FEN.find(' ') + 1;

	if(FEN[begin]=='w')
		this->_turn = ChessPiece::WHITE;
	else if(FEN[begin]=='b')
		this->_turn = ChessPiece::BLACK;

	begin=FEN.find(' ',begin) + 1;

	std::string castle=FEN.substr(begin,FEN.find(' ',begin));

	begin=FEN.find(' ',begin) + 1;

	if(FEN[begin]=='-')
	{
	// en passant
        ;
    }
	else
    {
        //that's impossible
        ;
    }   
	begin=FEN.find(' ',begin) + 1;

	int halfmoves=Util::parse_string<int>(FEN.substr(begin,FEN.find(' ',begin)));

	begin=FEN.find(' ',begin) + 1;

	this->fullmoves=Util::parse_string<int>(FEN.substr(begin,FEN.size()));

}

std::string XiangqiState::FEN() const {
	std::string ans;
	ans=this->board_fen;
	ans+=" ";
	ans+=(char)(this->_turn == ChessPiece::WHITE ? 'w' : 'b');
	ans+=" ";
	ans+="-"; //castle;
	ans+=" ";
	ans+='-'; //enpassant;
	ans+=" ";
	ans+="-"; //halfmoves;
	ans+=" ";
	ans+=Util::to_string(this->fullmoves);
	return ans;
}

bool XiangqiState::operator==(const XiangqiState& s) const {
	return (this->board_fen == s.board_fen) and (this->_turn == s._turn);
}

