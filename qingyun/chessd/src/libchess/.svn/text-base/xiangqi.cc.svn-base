

#include "xiangqi.hh"
#include "xqpiece.hh"
#include "xqmove.hh"
#include <vector>
#include <string>
#include <stdlib.h>

Xiangqi::Xiangqi() : XiangqiBasedGame(10,9) {
	this->current_state=new XiangqiState();

	this->putPieces();

	this->history->putInHistory(this->current_state);

	this->current_state=new XiangqiState();

	this->_turn=WHITE;
}

Xiangqi::Xiangqi(const std::string& FEN) : XiangqiBasedGame(10,9) {
	this->current_state=new XiangqiState(FEN);

	this->_turn=current_state->turn();

	this->history->putInHistory(new XiangqiState(FEN));

	this->setState(FEN.substr(0,FEN.find(' ')));
}

Xiangqi::~Xiangqi() {
	delete this->current_state;
}

int Xiangqi::winner() const {
	if( verifyCheckMate(XiangqiPiece::WHITE) )
		return BLACK;
	else if( verifyCheckMate(XiangqiPiece::BLACK) )
		return WHITE;
	return UNDEFINED;
}

int Xiangqi::turn() const {
	return _turn;
}

bool Xiangqi::verifyAndMakeMove(const std::string& move) {
	if( move.size()!=4 and move.size()!=5)
		return false;
	if(move[0]<'a' or move[0]>'i')
		return false;
	if(move[1]<'0' or move[1]>'9')
		return false;
	if(move[2]<'a' or move[2]>'i')
		return false;
	if(move[3]<'0' or move[3]>'9')
		return false;
    /*FIXME: this is succeeded from Chess*/
	if(move.size()==5)
		if(toupper(move[4])!='Q' and toupper(move[4])!='N' and toupper(move[4])!='B' and toupper(move[4])!='R')
			return false;

	XiangqiMove cm(this->_turn,move);
	if(verifyMove(cm) == true) {
		updateMove(cm);
		return true;
	}
	return false;
}

void Xiangqi::makeMove(const std::string& mv) {
	updateMove(XiangqiMove(this->_turn,mv));
}

bool Xiangqi::verifyCheckMate() const {
	return verifyCheckMate(this->_turn);
}

int Xiangqi::verifyDraw() const {
	if(verifyThreefoldRepetition()==true)
		return 1;

	if(verifyImpossibilityOfCheckmate()==true)
		return 2;
/*   FIXME: there's no halfmoves in Xiangqi
	if(this->getChessState().halfmoves >= 50)
		return 3;
*/
	if(verifyStaleMate(this->_turn)==true)
		return 4;

	return 0;
}

const XiangqiState& Xiangqi::getChessState() const {
	return *(static_cast<XiangqiState*>(this->current_state));
}
/* count numbers in the board_fen */
int Xiangqi::countPieces(const std::string& fen) {
	int count=0;
	for(unsigned int i=0;i<fen.size();i++)
		if(fen[i]>='0' and fen[i]<='9')
			count+=fen[i]-'0';
	return count;
}

/*FIXME: should I need it?*/
bool Xiangqi::hasThePawnPromoted() const {
	return false;
}

void Xiangqi::updateHistory() {
	this->history->putInHistory(this->current_state);
	this->current_state = new XiangqiState();
	*(static_cast<XiangqiState*>(this->current_state)) = *static_cast<const XiangqiState*>(this->history->lastState());
}

void Xiangqi::updateTurn() {
	this->_turn=(this->_turn==WHITE?BLACK:WHITE);
}

bool Xiangqi::verifyCheckMate(int player) const {
	if(verifyCheck(player) == false)
		return false;

	for(int i=0; i<this->nlines;i++) 
		for(int j=0;j<this->ncolums;j++) 
			if( this->gameboard->color(Position(j,i)) == player ) {
				Position pos(j,i);
				std::vector <Position> *p=getPositions(pos);
				if(p->size() > 0) {
					delete p;
					return false;
				}
				delete p;
			}
	return true;
}

bool Xiangqi::verifyMove(const XiangqiMove &mv) const {
	if(this->verifyPieceMove(mv))
		return willBeInCheck(mv)==false;

	return false;
}
/* a smart way to judge if the move will trigger the check*/
bool Xiangqi::willBeInCheck(const XiangqiMove& mv) const {
	//TODO a better way to take out mutable from Board
	char a=this->gameboard->getPieceReal(mv.from());
	char b=this->gameboard->getPieceReal(mv.to());

	this->makeMove(mv);
	bool ans=this->verifyCheck(mv.color());

	this->gameboard->createPiece(mv.from(),new XiangqiPiece(a));
	this->gameboard->createPiece(mv.to(),new XiangqiPiece(b));
	return ans;
}

bool Xiangqi::madeCheckMate(const XiangqiMove& mv) const {
	//TODO a better way to take out mutable from Board
	char a=this->gameboard->getPieceReal(mv.from());
	char b=this->gameboard->getPieceReal(mv.to());

	this->makeMove(mv);
	bool ans = this->verifyCheckMate(mv.color()^1);

	this->gameboard->createPiece(mv.from(),new XiangqiPiece(a));
	this->gameboard->createPiece(mv.to(),new XiangqiPiece(b));
	return ans;
}

void Xiangqi::makeMove(const XiangqiMove &mv) const {
	//FIXME workarrounds
	//verify if the move was castle
	this->gameboard->movePiece(mv.from(),mv.to());


}

void Xiangqi::updateMove(const XiangqiMove &mv) {


	//transform move to PGN before making the move
	this->PGN_of_last_move=this->XiangqiMoveToPGN(mv);

	this->makeMove(mv);
	this->updateState(mv);
	this->updateHistory();
	this->updateTurn();
}

//if this function is called, then the move is valid
void Xiangqi::updateState(const XiangqiMove& mv) {
	XiangqiState* current_state = static_cast<XiangqiState*>(this->current_state);


	if(mv.color() == BLACK)
		current_state->fullmoves++;

	bool captured=this->countPieces(current_state->board_fen)!=this->countPieces(this->getPosForFEN());
	current_state->board_fen = this->getPosForFEN();
	current_state->_turn = (this->current_state->_turn == WHITE ? BLACK : WHITE );

}
/*FIXME: is it useless?*/
bool Xiangqi::verifyCastle(const XiangqiMove& mv) const {
	/*if the king has moved two positions to the right or left, then it's a castle*/
	if( (this->gameboard->getType(mv.from()) == XiangqiPiece::KING))
		if( abs(mv.to().x - mv.from().x)==2 )
			return true;
	return false;
}

bool Xiangqi::verifyStaleMate(int player) const {
	//if the player is in check, it's not stalemate
	if(verifyCheck(player) == true)
		return false;

	/*Search for the pieces of "player" and check if there's a valid position for each one*/
	for(int i=0; i<this->nlines;i++)
		for(int j=0;j<this->ncolums;j++)
			if( this->gameboard->color(Position(j,i)) == player ) {
				Position pos(j,i);
				std::vector <Position> *p=getPositions(pos);
				if(p->size()>0) {
					delete p;
					return false;
				}
				delete p;
			}
	return true;
}

/*FIXME: is it useless?*/
bool Xiangqi::verifyThreefoldRepetition() const {

	return false;
}

bool Xiangqi::verifyImpossibilityOfCheckmate() const {
	/*this vector stores all pieces of each player*/
	std::vector<std::pair<XiangqiPiece,Position> > aux[2];
	for(int i=0;i<this->nlines;i++)
		for(int j=0;j<this->ncolums;j++) {
			if((*this->gameboard)[i][j]->type() != XiangqiPiece::NOTYPE) {
				aux[(*this->gameboard)[i][j]->color()].push_back(std::make_pair(*static_cast<XiangqiPiece*>((*this->gameboard)[i][j]),Position(j,i)));
				if(aux[(*this->gameboard)[i][j]->color()].size() > 2)
					return false;
			}
		}

	/* if the two players each have only one piece, then it's sure that it is only one king foreach
	 * and is a impossibility of checkmate */
	if((aux[0].size()==1) and (aux[1].size()==1))
		return true;
	/* if white has one piece and black has 2, a king and an elephant*/
	else if((aux[0].size()==1) and (aux[1].size()==2)) {
		for(int i=0;i<(int)aux[1].size();i++)
			if(aux[1][i].first.type()==XiangqiPiece::ELEPHANT)
				return true;
	}

	/* if black has one piece and white has 2, a king and an elephant*/
	else if((aux[0].size()==2) and (aux[1].size()==1)) {
		for(int i=0;i<(int)aux[0].size();i++)
			if(aux[0][i].first.type()==XiangqiPiece::ELEPHANT)
				return true;
	}

	/*if both players have 2 pieces a king and an elephant of different color*/
	else if((aux[0].size()==2) and (aux[1].size()==2)) {
		for(int i=0;i<(int)aux[0].size();i++)
			if(aux[0][i].first.type()==XiangqiPiece::ELEPHANT)
				for(int j=0;j<(int)aux[1].size();j++)
					if(aux[1][j].first.type()==XiangqiPiece::ELEPHANT)
							return true;
	}
	return false;
}

std::vector<Position> Xiangqi::howmanyCanMove(const Position& where, const XiangqiPiece& p) const {
	std::vector<Position> ans;
	for(int i=0;i<this->nlines;i++)
		for(int j=0;j<this->ncolums;j++) {
			Position aux(j,i);
			if(p==XiangqiPiece((char)this->gameboard->getPieceReal(aux))) {
				if(this->verifyMove(XiangqiMove(aux,where,p.color()))==true)
					ans.push_back(aux);
			}
		}
	return ans;
}

bool Xiangqi::differentColumns(Position where, const std::vector<Position>& pos) {
	/*iterate through all elements of "pos" searching for a position
	that has the same column as "where*/
	for(unsigned int i=0;i<pos.size();i++)
		if(where != pos[i])
			if(where.x == pos[i].x)
				return false;
	return true;
}

bool Xiangqi::differentRows(Position where, const std::vector<Position>& pos) {
	/*iterate through all elements of "pos" searching for a position
	that has the same row as "where*/
	for(unsigned int i=0;i<pos.size();i++)
		if(where != pos[i])
			if(where.y == pos[i].y)
				return false;
	return true;
}

std::string Xiangqi::XiangqiMoveToPGN(const XiangqiMove& mv) const {
	std::string from,to,check;


		//set to that is always present
		to=mv.to().toStringNotation();

		//only pawns that don't require the name of the piece
		if(this->gameboard->getType(mv.from()) != XiangqiPiece::PAWN)
			//set the name of the piece at the beginning
			from=toupper(this->gameboard->getPieceReal(mv.from()));

		std::vector<Position> pieces_positions;
		//if more than one piece of the same type can go to that position
		if((pieces_positions=howmanyCanMove(mv.to(),this->gameboard->getType(mv.from()))).size() > 1) {
			/*if the piece going to position mv.to() is in a different
			  column from the others*/
			if(differentColumns(mv.from(),pieces_positions)==true)
				from+=char(mv.from().x+'a');
			/*if the piece going to position mv.to() is in a different
			  row from the others*/
			else if(differentRows(mv.from(),pieces_positions)==true)
				from+=char(mv.from().y+'0');
			else /*need a full position*/
				from+=mv.from().toStringNotation();
		}

		/*if a piece will be captured*/
		if( (this->gameboard->getType(mv.to()) != XiangqiPiece::NOTYPE) ) {
			if(this->gameboard->getType(mv.from()) == XiangqiPiece::PAWN)
				from=char(mv.from().x+'a');
			from+="x";
		}

	/*if the player made a check*/
	if( willBeInCheck( XiangqiMove(mv.from(),mv.to(),mv.color()^1) ) ) {
		check="+";
		/*if it was checkmate*/
		if( madeCheckMate(mv) == true)
			check="#";
	}

	return from+to+check;
}

void Xiangqi::putPieces() {
	XiangqiPiece::XiangqiPieceColor player=XiangqiPiece::WHITE;
	//maybe just set state is fine
	this->gameboard->createPiece(Position(0,0),new XiangqiPiece(XiangqiPiece::ROOK,player));
	this->gameboard->createPiece(Position(8,0),new XiangqiPiece(XiangqiPiece::ROOK,player));
	this->gameboard->createPiece(Position(1,0),new XiangqiPiece(XiangqiPiece::HORSE,player));
	this->gameboard->createPiece(Position(7,0),new XiangqiPiece(XiangqiPiece::HORSE,player));
	this->gameboard->createPiece(Position(2,0),new XiangqiPiece(XiangqiPiece::ELEPHANT,player));
	this->gameboard->createPiece(Position(6,0),new XiangqiPiece(XiangqiPiece::ELEPHANT,player));
	this->gameboard->createPiece(Position(3,0),new XiangqiPiece(XiangqiPiece::ADVISOR,player));
	this->gameboard->createPiece(Position(5,0),new XiangqiPiece(XiangqiPiece::ADVISOR,player));
	this->gameboard->createPiece(Position(4,0),new XiangqiPiece(XiangqiPiece::KING,player));
	this->gameboard->createPiece(Position(1,2),new XiangqiPiece(XiangqiPiece::CANNON,player));
	this->gameboard->createPiece(Position(7,2),new XiangqiPiece(XiangqiPiece::CANNON,player));
	this->gameboard->createPiece(Position(0,3),new XiangqiPiece(XiangqiPiece::PAWN,player));
	this->gameboard->createPiece(Position(2,3),new XiangqiPiece(XiangqiPiece::PAWN,player));
	this->gameboard->createPiece(Position(4,3),new XiangqiPiece(XiangqiPiece::PAWN,player));
	this->gameboard->createPiece(Position(6,3),new XiangqiPiece(XiangqiPiece::PAWN,player));
	this->gameboard->createPiece(Position(8,3),new XiangqiPiece(XiangqiPiece::PAWN,player));
	player=XiangqiPiece::BLACK;
	this->gameboard->createPiece(Position(0,9),new XiangqiPiece(XiangqiPiece::ROOK,player));
	this->gameboard->createPiece(Position(8,9),new XiangqiPiece(XiangqiPiece::ROOK,player));
	this->gameboard->createPiece(Position(1,9),new XiangqiPiece(XiangqiPiece::HORSE,player));
	this->gameboard->createPiece(Position(7,9),new XiangqiPiece(XiangqiPiece::HORSE,player));
	this->gameboard->createPiece(Position(2,9),new XiangqiPiece(XiangqiPiece::ELEPHANT,player));
	this->gameboard->createPiece(Position(6,9),new XiangqiPiece(XiangqiPiece::ELEPHANT,player));
	this->gameboard->createPiece(Position(3,9),new XiangqiPiece(XiangqiPiece::ADVISOR,player));
	this->gameboard->createPiece(Position(5,9),new XiangqiPiece(XiangqiPiece::ADVISOR,player));
	this->gameboard->createPiece(Position(4,9),new XiangqiPiece(XiangqiPiece::KING,player));
	this->gameboard->createPiece(Position(1,7),new XiangqiPiece(XiangqiPiece::CANNON,player));
	this->gameboard->createPiece(Position(7,7),new XiangqiPiece(XiangqiPiece::CANNON,player));
	this->gameboard->createPiece(Position(0,6),new XiangqiPiece(XiangqiPiece::PAWN,player));
	this->gameboard->createPiece(Position(2,6),new XiangqiPiece(XiangqiPiece::PAWN,player));
	this->gameboard->createPiece(Position(4,6),new XiangqiPiece(XiangqiPiece::PAWN,player));
	this->gameboard->createPiece(Position(6,6),new XiangqiPiece(XiangqiPiece::PAWN,player));
	this->gameboard->createPiece(Position(8,6),new XiangqiPiece(XiangqiPiece::PAWN,player));
}
