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

#include "ChessBasedGame.hh"
#include "../Util/utils.hh"
#include <cstdlib>

ChessBasedGame::ChessBasedGame(int n, int m) : BoardGame(n,m) {
    this->gameboard=new ChessBoard(n,m);
    this->history = new ChessHistory();
}

ChessBasedGame::~ChessBasedGame() {
    delete this->gameboard;
    delete this->history;
}

int ChessBasedGame::numberOfTurns() const {
    return this->history->getHistory().size()-1;
}

const std::vector<State*>& ChessBasedGame::getHistory() const {
    return this->history->getHistory();
}

bool ChessBasedGame::verifyPieceMove(const ChessMove& mv) const {
    Position to = mv.to();
    Position from = mv.from();
    if((to.x >=0) and (to.x < this->ncolums) and (to.y >= 0) and (to.y < this->nlines)) {
        if((mv.color()!=(this->gameboard->color(from))) or (mv.color()==(this->gameboard->color(to))))
            return false;

        //not very much portable
        switch (this->gameboard->getType(from)) {
            case ChessPiece::KNIGHT:
                return this->verifyHorseMove(mv);
            case ChessPiece::PAWN:
                return this->verifyPawnMove(mv);
            case ChessPiece::ROOK:
                return this->verifyRookMove(mv);
            case ChessPiece::BISHOP:
                return this->verifyBishopMove(mv);
            case ChessPiece::QUEEN:
                return this->verifyQueenMove(mv);
            case ChessPiece::KING:
                return this->verifyKingMove(mv);
            default:
                return false;
        };
    }
    return false;
}

void ChessBasedGame::setState(const std::string& board_FEN) const {
    int line=7;
    int column=0;
    for(int i=0;i<(int)board_FEN.size();i++) {
        if((board_FEN[i] >= '0') and (board_FEN[i] <= '9')) {
            for( int k = 0; k < board_FEN[i] - '0';k++) {
                this->gameboard->createPiece(Position(column,line), new ChessPiece() );
                column++;
            }
        }
        else if(board_FEN[i]=='/') {
            column=0;
            line--;
        }
        else {
            this->gameboard->createPiece(Position(column,line),new ChessPiece(board_FEN[i]));
            column++;
        }
    }
}

std::string ChessBasedGame::getPosForFEN() const {
    std::string fen;
    int count;
    for(int i=nlines-1;i>=0;i--) {
        count=0;
        for(int j=0;j<ncolums;j++) {
            if((*this->gameboard)[i][j]->pieceReal()=='.')
                count++;
            else {
                if(count>0)
                    fen+=Util::to_string(count);
                count=0;
                fen+=(char)((*this->gameboard)[i][j]->pieceReal());
            }
        }
        if(count>0)
            fen+=Util::to_string(count);

        fen+= '/' ;
    }
    fen.erase(fen.end()-1);
    return fen;
}

bool ChessBasedGame::beingAttacked(const Position &p, int player) const {
    for(int i=0;i<this->nlines;i++)
        for(int j=0;j<this->ncolums;j++)
            if((*this->gameboard)[i][j]->color() == player)
                if(this->verifyPieceMove(ChessMove(Position(j,i),p,player)))
                    return true;
    return false;
}

Position ChessBasedGame::findKing(int player) const {
    for(int i=0; i<this->nlines;i++)
        for(int j=0;j<this->ncolums;j++) {
            if( ((*this->gameboard)[i][j]->type() == ChessPiece::KING) and ((*this->gameboard)[i][j]->color() == player) )
                return Position(j,i);
        }
    return Position(-1,-1);
}

bool ChessBasedGame::verifyCheck(int player) const {
    return beingAttacked(findKing(player),player^1);
}

std::vector <Position> *ChessBasedGame::getPositions(const Position& p) const {
    int jogador = this->gameboard->color(p);
    std::vector <Position> *pos = this->getpossiblePositions(p);
    std::vector <Position> *ans = new std::vector <Position>;
    for(int i=0;i<(int)pos->size();i++) {
        if(verifyMove( ChessMove(p,(*pos)[i],jogador) ))
            ans->push_back( (*pos)[i] );
    }
    delete pos;
    return ans;
}

//this function does not verify anything
bool ChessBasedGame::verifyEnPassant(const ChessMove& j) const {
    if(this->gameboard->getType(j.from()) == ChessPiece::PAWN) {
        int distx=abs(j.to().x - j.from().x );
        int disty=abs(j.to().y - j.from().y );
        if( (distx == 1) and (disty == 1) and (this->gameboard->color(j.to()) == -1) ) {
            return true;
        }
    }
    return false;
}

bool ChessBasedGame::VerifyDiagonal(const ChessMove& mv) const {
    Position to = mv.to();
    Position from = mv.from();
    int i,j;
    int k=(to.x-from.x)>0?1:-1;
    int l=(to.y-from.y)>0?1:-1;
    for(i=from.x+k,j=from.y+l;Position(i,j)!=to;i+=k,j+=l)
        if(this->gameboard->color(Position(i,j)) != ChessPiece::NOCOLOR)
            return false;
    return true;
}


bool ChessBasedGame::VerifyHorizontal(const ChessMove& mv) const {
    Position to = mv.to();
    Position from = mv.from();
    int k=(to.x-from.x)>0?1:-1;
    for(int i=from.x+k; i!=to.x ; i+=k)
        //access board and verify
        if(this->gameboard->color(Position(i,from.y)) != ChessPiece::NOCOLOR)
            return false;
    return true;
}

bool ChessBasedGame::VerifyVertical(const ChessMove& mv) const {
    Position to = mv.to();
    Position from = mv.from();
    int l=(to.y-from.y)>0?1:-1;

    for(int j=from.y+l; j!= to.y ; j+=l)
        if(this->gameboard->color(Position(from.x,j)) != ChessPiece::NOCOLOR)
            return false;
    return true;
}

bool ChessBasedGame::verifyHorseMove(const ChessMove& mv) const {
    int distx,disty;
    distx=abs(mv.from().x-mv.to().x);
    disty=abs(mv.from().y-mv.to().y);
    if(this->gameboard->color(mv.to()) == mv.color())
        return false;
    if( distx+disty == 3 )
        if( (distx==2 and disty==1) or (distx==1 and disty==2) )
            return true;
    return false;
}

bool ChessBasedGame::verifyPawnMove(const ChessMove& mv) const {
    int distx,disty;
    const Position& from = mv.from();
    const Position& to = mv.to();
    distx=abs(from.x-to.x);
    disty=(to.y-from.y);

    if( (mv.color() == ChessPiece::WHITE ) && (disty < 0))
        return false;
    else if( (mv.color() == ChessPiece::BLACK) && (disty > 0))
        return false;
    if(this->gameboard->color(to) == mv.color())
        return false;

    disty=abs(disty);

    //Pawn moving just one square
    if( (disty == 1) and (distx == 0) ) {
        if(this->gameboard->color(to) != ChessPiece::NOCOLOR)
            return false;
        return true;
    }
    //Pawn moving two squares ahead
    else if( (disty == 2) and (distx == 0) ) {
        int meio=(from.y+to.y)/2;
        if(this->gameboard->color(Position(from.x,meio)) != ChessPiece::NOCOLOR)
            return false;
        if(this->gameboard->color(to) != ChessPiece::NOCOLOR)
            return false;

        //FIXME not very much portable
        if(this->gameboard->color(from) == ChessPiece::WHITE) {
            if(from.y != 1)
                return false;
        }
        else if(from.y != this->nlines - 2)
            return false;
        return true;
    }
    //enpassant or capturing in diagonal
    else if( (disty == 1) and (distx == 1) ) {
        if(this->gameboard->color(to) != ChessPiece::NOCOLOR)
            return true;
        else if( this->getChessState().enpassant == to)
            return true;
        return false;
    }
    else
        return false;

    return false;
}

bool ChessBasedGame::verifyRookMove(const ChessMove& mv) const {
    int distx,disty;
    distx=abs(mv.from().x-mv.to().x);
    disty=abs(mv.from().y-mv.to().y);
    if(this->gameboard->color(mv.to()) != mv.color())
    {
        if(this->gameboard->color(mv.to()) != mv.color())
        {
            if( (distx == 0) and (disty > 0) ) 
                return VerifyVertical(mv);
            else if( (distx > 0) and (disty == 0) ) {
                return VerifyHorizontal(mv);
            }
        }
    }
    return false;
}

bool ChessBasedGame::verifyBishopMove(const ChessMove& mv) const {
    int distx,disty;
    const Position& to = mv.to();
    const Position& from = mv.from();
    distx=abs(from.x-to.x);
    disty=abs(from.y-to.y);
    if(this->gameboard->color(mv.to()) != mv.color())
        if( (distx == disty) and (distx!=0) ) 
            return VerifyDiagonal(mv);

    return false;
}

bool ChessBasedGame::verifyQueenMove(const ChessMove& mv) const {
    int distx,disty;
    distx=abs(mv.from().x-mv.to().x);
    disty=abs(mv.from().y-mv.to().y);

    if(this->gameboard->color(mv.to()) != mv.color()) {
        if(distx > 0 or disty > 0) {
            if( distx == disty ) {
                return VerifyDiagonal(mv);
            }
            else if( (distx > 0) and (disty == 0)) {
                return VerifyHorizontal(mv);
            }
            else if( (disty > 0) and (distx == 0)) {
                return VerifyVertical(mv);
            }
        }
    }
    return false;
}

bool ChessBasedGame::verifyKingMove(const ChessMove& mv) const {
    if(this->gameboard->color(mv.to()) != mv.color()) {
        int distx=(mv.from().x-mv.to().x);
        int disty =(mv.from().y-mv.to().y);
        if( (abs(distx) <=1) and ( abs(disty) <= 1))
            return true;
        if( ( abs(distx) == 2) and ( abs(disty) == 0) ) {
            char queen='q',king='k';
            if(mv.color() == WHITE) {
                queen='Q';	king='K';
            }
            //castle
            std::string castle=(this->getChessState().castle);
            if((distx > 0 and (castle.find(queen,0) < castle.size())) or ((distx<0)and(castle.find(king,0) < castle.size()))) {
                //verify if there's someone between the rook and the king
                if(verifyRookMove(ChessMove(distx>0?Position(0,mv.to().y):Position(7,mv.to().y),Position(mv.from().x-distx/abs(distx),mv.to().y),mv.color()))==false)
                    return false;
                if(verifyCheck(mv.color())) //verify if the king is in check
                    return false;
                else if(beingAttacked(Position((mv.from().x+mv.to().x)/2,mv.to().y),mv.color()^1)) //if 1 after the king position is being atacked
                    return false;
                else if(beingAttacked(mv.to(),mv.color()^1)) //if 2 after the king position is being atacked
                    return false;
                return true;
            }
        }
    }
    return false;
}

std::vector <Position> *ChessBasedGame::getVerticalandHorizontal(const Position& p) const {
    std::vector <Position> *ans=new std::vector <Position>;
    int py[]={1,-1,0,0};
    int px[]={0,0,1,-1};
    for(int k=0;k<4;k++) {
        for(int i=(p.y+py[k]),j=(p.x+px[k]);(i<this->nlines) and (i>=0) and (j < this->ncolums) and (j>=0);i+=py[k],j+=px[k]) {
            Position aux(j,i);
            ans->push_back(aux);
            if(this->gameboard->color(aux) != UNDEFINED)
                break;
        }
    }
    return ans;
}

std::vector <Position> *ChessBasedGame::getDiagonals(const Position& p) const {
    std::vector <Position> *ans = new std::vector <Position>;
    int dx[]={1,-1,1,-1};
    int dy[]={1,1,-1,-1};
    for(int k=0;k<4;k++) {
        for(int i=(p.y+dy[k]),j=(p.x+dx[k]);(i<this->ncolums) and(i>=0) and (j < this->ncolums) and (j>=0);i+=dy[k],j+=dx[k]) {
            Position aux(j,i);
            ans->push_back(aux);
            if( this->gameboard->color(aux) != UNDEFINED)
                break;
        }
    }
    return ans;
}

std::vector <Position> *ChessBasedGame::getpossiblePositions(const Position& position) const {

    std::vector <Position> *p = new std::vector <Position>;
    switch(this->gameboard->getType(position)) {
        case ChessPiece::KNIGHT:
            {
                int posx[]={1,2,2,1,-1,-2,-2,-1};
                int posy[]={2,1,-1,-2,-2,-1,1,2};
                for(int k=0;k<8;k++)
                    p->push_back(Position(posx[k],posy[k])+position);
            }
            break;
        case ChessPiece::PAWN:
            {
                int posx[]={0,0,1,-1,0,0,1,-1};
                int posy[]={1,2,1,1,-1,-2,-1,-1};
                for(int k=0;k<8;k++)
                    p->push_back(Position(posx[k],posy[k])+position);
            }
            break;
        case ChessPiece::ROOK:
            {
                std::vector <Position> *pos = getVerticalandHorizontal(position);
                p->insert(p->end(),pos->begin(),pos->end());
                delete pos;
            }
            break;
        case ChessPiece::BISHOP:
            {
                std::vector <Position> *pos = getDiagonals(position);
                p->insert(p->end(),pos->begin(),pos->end());
                delete pos;
            }
            break;
        case ChessPiece::QUEEN:
            {
                std::vector <Position> *pos = getVerticalandHorizontal(position);
                p->insert(p->end(),pos->begin(),pos->end());
                delete pos;
                pos = getDiagonals(position);
                p->insert(p->end(),pos->begin(),pos->end());
                delete pos;
            }
            break;
        case ChessPiece::KING:
            {
                int posx[]={0,1,1,1,0,-1,-1,-1,2,-2};
                int posy[]={1,1,0,-1,-1,-1,0,1,0,0};
                for(int k=0;k<10;k++)
                    p->push_back(Position(posx[k],posy[k])+position);
            }
            break;
        default:
            break;
    }
    return p;
}
