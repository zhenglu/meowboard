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

#include "Chess.hh"
#include "Piece.hh"
#include "ChessMove.hh"
#include <vector>
#include <string>
#include <stdlib.h>

Chess::Chess() : ChessBasedGame(8,8) {
    this->current_state=new ChessState();

    this->putPieces();

    this->history->putInHistory(this->current_state);

    this->current_state=new ChessState();

    this->_turn=WHITE;
}

Chess::Chess(const std::string& FEN) : ChessBasedGame(8,8) {
    this->current_state=new ChessState(FEN);

    this->_turn=current_state->turn();

    this->history->putInHistory(new ChessState(FEN));

    this->setState(FEN.substr(0,FEN.find(' ')));
}

Chess::~Chess() {
    delete this->current_state;
}

    int Chess::winner() const {
        if( verifyCheckMate(0) )
            return BLACK;
        else if( verifyCheckMate(1) )
            return WHITE;
        return UNDEFINED;
    }

int Chess::turn() const {
    return _turn;
}

    bool Chess::verifyAndMakeMove(const std::string& move) {
        if( move.size()!=4 and move.size()!=5)
            return false;
        if(move[0]<'a' or move[0]>'h')
            return false;
        if(move[1]<'1' or move[1]>'8')
            return false;
        if(move[2]<'a' or move[2]>'h')
            return false;
        if(move[3]<'1' or move[3]>'8')
            return false;
        if(move.size()==5)
            if(toupper(move[4])!='Q' and toupper(move[4])!='N' and toupper(move[4])!='B' and toupper(move[4])!='R')
                return false;

        ChessMove cm(this->_turn,move);
        if(verifyMove(cm) == true) {
            updateMove(cm);
            return true;
        }
        return false;
    }

void Chess::makeMove(const std::string& mv) {
    updateMove(ChessMove(this->_turn,mv));
}

bool Chess::verifyCheckMate() const {
    return verifyCheckMate(this->_turn);
}

    int Chess::verifyDraw() const {
        if(verifyThreefoldRepetition()==true)
            return 1;

        if(verifyImpossibilityOfCheckmate()==true)
            return 2;

        if(this->getChessState().halfmoves >= 50)
            return 3;

        if(verifyStaleMate(this->_turn)==true)
            return 4;

        return 0;
    }

const ChessState& Chess::getChessState() const {
    return *(static_cast<ChessState*>(this->current_state));
}

int Chess::countPieces(const std::string& fen) {
    int count=0;
    for(unsigned int i=0;i<fen.size();i++)
        if(fen[i]>='0' and fen[i]<='9')
            count+=fen[i]-'0';
    return count;
}

bool Chess::hasThePawnPromoted() const {
    return this->pawn_promoted;
}

void Chess::updateHistory() {
    this->history->putInHistory(this->current_state);
    this->current_state = new ChessState();
    *(static_cast<ChessState*>(this->current_state)) = *static_cast<const ChessState*>(this->history->lastState());
}

void Chess::updateTurn() {
    this->_turn=(this->_turn==WHITE?BLACK:WHITE);
}

    bool Chess::verifyCheckMate(int player) const {
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

    bool Chess::verifyMove(const ChessMove &mv) const {
        if(this->verifyPieceMove(mv))
            return willBeInCheck(mv)==false;

        return false;
    }

bool Chess::willBeInCheck(const ChessMove& mv) const {
    //TODO a better way to take out mutable from Board
    char a=this->gameboard->getPieceReal(mv.from());
    char b=this->gameboard->getPieceReal(mv.to());
    bool specialmv=this->verifyCastle(mv) or this->verifyEnPassant(mv);

    this->makeMove(mv);
    bool ans=this->verifyCheck(mv.color());

    if(specialmv)
        this->setState(current_state->boardFEN());
    else {
        this->gameboard->createPiece(mv.from(),new ChessPiece(a));
        this->gameboard->createPiece(mv.to(),new ChessPiece(b));
    }
    return ans;
}

bool Chess::madeCheckMate(const ChessMove& mv) const {
    //TODO a better way to take out mutable from Board
    char a=this->gameboard->getPieceReal(mv.from());
    char b=this->gameboard->getPieceReal(mv.to());
    bool specialmv=this->verifyCastle(mv) or this->verifyEnPassant(mv);

    this->makeMove(mv);
    bool ans = this->verifyCheckMate(mv.color()^1);

    if(specialmv)
        this->setState(current_state->boardFEN());
    else {
        this->gameboard->createPiece(mv.from(),new ChessPiece(a));
        this->gameboard->createPiece(mv.to(),new ChessPiece(b));
    }
    return ans;
}

void Chess::makeMove(const ChessMove &mv) const {
    //FIXME workarrounds
    //verify if the move was castle
    if(this->verifyCastle(mv) ) {
        int distx;
        distx=mv.to().x - mv.from().x ;
        Position aux;
        if(distx < 0)
            aux = Position(0,mv.from().y);
        else
            aux = Position(7,mv.from().y);
        ChessMove cast_swift = ChessMove(aux,Position( (mv.from().x + mv.to().x)/2,mv.from().y),mv.color());
		this->gameboard->movePiece(cast_swift.from(),cast_swift.to());
	}
	//enpassant
	if(this->verifyEnPassant(mv))
		this->gameboard->createPiece(Position(mv.to().x,mv.from().y), new ChessPiece());
	this->gameboard->movePiece(mv.from(),mv.to());

    //Pawn promotion
    //Is the Pawn at the end of the tab? 
    //Default is to transform the pawn to queen
    //or get the piece at the end of move
    if(this->gameboard->getType(mv.to()) == ChessPiece::PAWN) {
        int final = ( (mv.color() == 0) ? 7 : 0);
        if(mv.to().y == final) {
            if(mv.move().size()==5)
                this->gameboard->createPiece(mv.to(),new ChessPiece(ChessPiece::chartotype(mv.move()[4]),(ChessPiece::PieceColor)(mv.color())));
            else
                this->gameboard->createPiece(mv.to(),new ChessPiece(ChessPiece::QUEEN,(ChessPiece::PieceColor)(mv.color())));
        }
    }
}

void Chess::updateMove(const ChessMove &mv) {
    this->pawn_promoted=false;
    if(this->gameboard->getType(mv.from()) == ChessPiece::PAWN)
        if(mv.to().y == ( (mv.color() == 0) ? 7 : 0))
            this->pawn_promoted=true;

    //transform move to PGN before making the move
    this->PGN_of_last_move=this->ChessMoveToPGN(mv);

    this->makeMove(mv);
    this->updateState(mv);
    this->updateHistory();
    this->updateTurn();
}

//if this function is called, then the move is valid
void Chess::updateState(const ChessMove& mv) {
    ChessState* current_state = static_cast<ChessState*>(this->current_state);

    current_state->enpassant=Position(-1,-1);

    if(mv.color() == BLACK)
        current_state->fullmoves++;

    bool captured=this->countPieces(current_state->board_fen)!=this->countPieces(this->getPosForFEN());
    current_state->board_fen = this->getPosForFEN();
    current_state->_turn = (this->current_state->_turn == WHITE ? BLACK : WHITE );
    current_state->halfmoves++;

    //FIXME this code is horrible.
    if((this->gameboard->getType(mv.to()) == ChessPiece::PAWN) or captured)
        current_state->halfmoves=0;

    //if the pawn moved 2 squares...
    if(this->gameboard->getType(mv.to()) == ChessPiece::PAWN) {
        if( abs(mv.to().y - mv.from().y) == 2) {
            current_state->enpassant = Position(mv.to().x, (int)((mv.color() == 0 )? (mv.to().y-1) : (mv.to().y+1) ) );
        }
    }

    //Has the king moved?
    if(this->gameboard->getType(mv.to()) == ChessPiece::KING) {
        char king, queen;
        king = ((mv.color() == 0)? 'K' : 'k');
        queen = ((mv.color() == 0) ? 'Q' : 'q');
        if(current_state->castle.find(king) < current_state->castle.size())
            current_state->castle.erase(current_state->castle.find(king),1);
        if(current_state->castle.find(queen) < current_state->castle.size())
            current_state->castle.erase(current_state->castle.find(queen),1);
    }
    //Are the Rooks at their initial places?
    {
        std::pair<Position,char> p[4];
        char rooks[4];
        rooks[0]=rooks[1]='R';
        rooks[2]=rooks[3]='r';
        p[0]=std::make_pair(Position(0,0),'Q');
        p[1]=std::make_pair(Position(7,0),'K');
        p[2]=std::make_pair(Position(0,7),'q');
        p[3]=std::make_pair(Position(7,7),'k');
        for(int i=0;i<4;i++)
            if ( this->gameboard->getPieceReal(p[i].first) != rooks[i] )
                if(current_state->castle.find(p[i].second) < current_state->castle.size())
                    current_state->castle.erase(current_state->castle.find(p[i].second),1);

        if(current_state->castle.size() == 0)
            current_state->castle="-";
    }
}

bool Chess::verifyCastle(const ChessMove& mv) const {
    /*if the king has moved two positions to the right or left, then it's a castle*/
    if( (this->gameboard->getType(mv.from()) == ChessPiece::KING))
        if( abs(mv.to().x - mv.from().x)==2 )
            return true;
    return false;
}

bool Chess::verifyStaleMate(int player) const {
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

bool Chess::verifyThreefoldRepetition() const {
    int count=1;
    ChessHistory &history=*static_cast<ChessHistory*>(this->history);
    //only needs to verify "halfmoves" for equal states
    for(int i=2;i<=history[history.size()-1].halfmoves;i+=2)
        if(history[history.size()-1-i]==history[history.size()-1])
            count++;
    if(count>=3)
        return true;
    return false;
}

bool Chess::canCheckmate(int player) const {
    int count = 0;
    for(int i=0;i<this->nlines;i++)
        for(int j=0;j<this->ncolums;j++) {
            if((*this->gameboard)[i][j]->type() != ChessPiece::NOTYPE and
                    (*this->gameboard)[i][j]->color() == player) {
                count ++;
            }
        }

    /* The player can't checkmate if he has only the king */
    if(count == 1) {
        return false;
    }

    return true;
}

bool Chess::verifyImpossibilityOfCheckmate() const {
    /*this vector stores all pieces of each player*/
    std::vector<std::pair<ChessPiece,Position> > aux[2];
    for(int i=0;i<this->nlines;i++)
        for(int j=0;j<this->ncolums;j++) {
            if((*this->gameboard)[i][j]->type() != ChessPiece::NOTYPE) {
                aux[(*this->gameboard)[i][j]->color()].push_back(std::make_pair(*static_cast<ChessPiece*>((*this->gameboard)[i][j]),Position(j,i)));
                if(aux[(*this->gameboard)[i][j]->color()].size() > 2)
                    return false;
            }
        }

    /* if the two players have only one piece, then it's sure that it is only one king foreach
     * and is a impossibility of checkmate */
    if((aux[0].size()==1) and (aux[1].size()==1))
        return true;
    /* if white has one piece and black has 2, a king and a bishop or a knight*/
    else if((aux[0].size()==1) and (aux[1].size()==2)) {
        for(int i=0;i<(int)aux[1].size();i++)
            if(aux[1][i].first.type()==ChessPiece::BISHOP or aux[1][i].first.type()==ChessPiece::KNIGHT)
                return true;
    }

    /* if black has one piece and white has 2, a king and a bishop or a knight*/
    else if((aux[0].size()==2) and (aux[1].size()==1)) {
        for(int i=0;i<(int)aux[0].size();i++)
            if(aux[0][i].first.type()==ChessPiece::BISHOP or aux[0][i].first.type()==ChessPiece::KNIGHT)
                return true;
    }

    /*if both players have 2 pieces a king and a bishop of different color*/
    else if((aux[0].size()==2) and (aux[1].size()==2)) {
        for(int i=0;i<(int)aux[0].size();i++)
            if(aux[0][i].first.type()==ChessPiece::BISHOP)
                for(int j=0;j<(int)aux[1].size();j++)
                    if(aux[1][j].first.type()==ChessPiece::BISHOP)
                        if((aux[0][i].second.x+aux[0][i].second.y)%2 == (aux[1][j].second.x+aux[1][j].second.y)%2)
                            return true;
    }
    return false;
}

std::vector<Position> Chess::howmanyCanMove(const Position& where, const ChessPiece& p) const {
    std::vector<Position> ans;
    for(int i=0;i<this->nlines;i++)
        for(int j=0;j<this->ncolums;j++) {
            Position aux(j,i);
            if(p==ChessPiece((char)this->gameboard->getPieceReal(aux))) {
                if(this->verifyMove(ChessMove(aux,where,p.color()))==true)
                    ans.push_back(aux);
            }
        }
    return ans;
}

bool Chess::differentColumns(Position where, const std::vector<Position>& pos) {
    /*iterate through all elements of "pos" searching for a position
      that has the same column as "where*/
    for(unsigned int i=0;i<pos.size();i++)
        if(where != pos[i])
            if(where.x == pos[i].x)
                return false;
    return true;
}

bool Chess::differentRows(Position where, const std::vector<Position>& pos) {
    /*iterate through all elements of "pos" searching for a position
      that has the same row as "where*/
    for(unsigned int i=0;i<pos.size();i++)
        if(where != pos[i])
            if(where.y == pos[i].y)
                return false;
    return true;
}

std::string Chess::ChessMoveToPGN(const ChessMove& mv) const {
    std::string from,to,promotion,check;

    if(this->verifyCastle(mv)==true) {
        //this difference in short castles is always > 0
        if(mv.to().x - mv.from().x > 0)
            to="O-O";
        else
            to="O-O-O";
    } else {

        //set to that is always present
        to=mv.to().toStringNotation();

        //only pawns that don't require the name of the piece
        if(this->gameboard->getType(mv.from()) != ChessPiece::PAWN)
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
        if( (this->gameboard->getType(mv.to()) != ChessPiece::NOTYPE) or this->verifyEnPassant(mv)) {
            if(this->gameboard->getType(mv.from()) == ChessPiece::PAWN)
                from=char(mv.from().x+'a');
            from+="x";
        }
    }

    /*if the player made a check*/
    if( willBeInCheck( ChessMove(mv.from(),mv.to(),mv.color()^1) ) ) {
        check="+";
        /*if it was checkmate*/
        if( madeCheckMate(mv) == true)
            check="#";
    }

    if(this->hasThePawnPromoted() == true)
        promotion=(mv.move().size()==5)?(toupper(mv.move()[4])):('Q');

    return from+to+promotion+check;
}

void Chess::putPieces() {
    Position p(0,0);
    ChessPiece::PieceColor player=ChessPiece::WHITE;
    //maybe just set state is fine
    this->gameboard->createPiece(Position(1,0),new ChessPiece(ChessPiece::KNIGHT,player));
    this->gameboard->createPiece(Position(6,0),new ChessPiece(ChessPiece::KNIGHT,player));
    this->gameboard->createPiece(Position(0,1),new ChessPiece(ChessPiece::PAWN,player));
    this->gameboard->createPiece(Position(1,1),new ChessPiece(ChessPiece::PAWN,player));
    this->gameboard->createPiece(Position(2,1),new ChessPiece(ChessPiece::PAWN,player));
    this->gameboard->createPiece(Position(3,1),new ChessPiece(ChessPiece::PAWN,player));
    this->gameboard->createPiece(Position(4,1),new ChessPiece(ChessPiece::PAWN,player));
    this->gameboard->createPiece(Position(5,1),new ChessPiece(ChessPiece::PAWN,player));
    this->gameboard->createPiece(Position(6,1),new ChessPiece(ChessPiece::PAWN,player));
    this->gameboard->createPiece(Position(7,1),new ChessPiece(ChessPiece::PAWN,player));
    this->gameboard->createPiece(Position(0,0),new ChessPiece(ChessPiece::ROOK,player));
    this->gameboard->createPiece(Position(7,0),new ChessPiece(ChessPiece::ROOK,player));
    this->gameboard->createPiece(Position(2,0),new ChessPiece(ChessPiece::BISHOP,player));
    this->gameboard->createPiece(Position(5,0),new ChessPiece(ChessPiece::BISHOP,player));
    this->gameboard->createPiece(Position(3,0),new ChessPiece(ChessPiece::QUEEN,player));
    this->gameboard->createPiece(Position(4,0),new ChessPiece(ChessPiece::KING,player));
    player=ChessPiece::BLACK;
    this->gameboard->createPiece(Position(1,7),new ChessPiece(ChessPiece::KNIGHT,player));
    this->gameboard->createPiece(Position(6,7),new ChessPiece(ChessPiece::KNIGHT,player));
    this->gameboard->createPiece(Position(0,6),new ChessPiece(ChessPiece::PAWN,player));
    this->gameboard->createPiece(Position(1,6),new ChessPiece(ChessPiece::PAWN,player));
    this->gameboard->createPiece(Position(2,6),new ChessPiece(ChessPiece::PAWN,player));
    this->gameboard->createPiece(Position(3,6),new ChessPiece(ChessPiece::PAWN,player));
    this->gameboard->createPiece(Position(4,6),new ChessPiece(ChessPiece::PAWN,player));
    this->gameboard->createPiece(Position(5,6),new ChessPiece(ChessPiece::PAWN,player));
    this->gameboard->createPiece(Position(6,6),new ChessPiece(ChessPiece::PAWN,player));
    this->gameboard->createPiece(Position(7,6),new ChessPiece(ChessPiece::PAWN,player));
    this->gameboard->createPiece(Position(0,7),new ChessPiece(ChessPiece::ROOK,player));
    this->gameboard->createPiece(Position(7,7),new ChessPiece(ChessPiece::ROOK,player));
    this->gameboard->createPiece(Position(2,7),new ChessPiece(ChessPiece::BISHOP,player));
    this->gameboard->createPiece(Position(5,7),new ChessPiece(ChessPiece::BISHOP,player));
    this->gameboard->createPiece(Position(3,7),new ChessPiece(ChessPiece::QUEEN,player));
    this->gameboard->createPiece(Position(4,7),new ChessPiece(ChessPiece::KING,player));
}
