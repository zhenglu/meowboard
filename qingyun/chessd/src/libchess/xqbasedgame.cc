#include "xqboard.hh"
#include "xqbasedgame.hh"
#include "../Util/utils.hh"
#include <cstdlib>

XiangqiBasedGame::XiangqiBasedGame(int n, int m) : BoardGame(n,m) {
	this->gameboard=new XiangqiBoard(n,m);

	this->history = new XiangqiHistory();
}

XiangqiBasedGame::~XiangqiBasedGame() {
	delete this->gameboard;

	delete this->history;
}

int XiangqiBasedGame::numberOfTurns() const {
	return this->history->getHistory().size()-1;
}

const std::vector<State*>& XiangqiBasedGame::getHistory() const {
	return this->history->getHistory();
}

bool XiangqiBasedGame::verifyPieceMove(const XiangqiMove& mv) const {
	Position to = mv.to();
	Position from = mv.from();
	if((to.x >=0) and (to.x < this->ncolums) and (to.y >= 0) and (to.y < this->nlines)) {
		if((mv.color()!=(this->gameboard->color(from))) or (mv.color()==(this->gameboard->color(to))))
			return false;

		//not very much portable
		switch (this->gameboard->getType(from)) {
			case XiangqiPiece::HORSE:
				return this->verifyHorseMove(mv);
			case XiangqiPiece::PAWN:
				return this->verifyPawnMove(mv);
			case XiangqiPiece::ROOK:
				return this->verifyRookMove(mv);
			case XiangqiPiece::ADVISOR:
				return this->verifyAdvisorMove(mv);
			case XiangqiPiece::ELEPHANT:
				return this->verifyElephantMove(mv);
			case XiangqiPiece::KING:
				return this->verifyKingMove(mv);
            case XiangqiPiece::CANNON:
                return this->verifyCannonMove(mv);

			default:
				return false;
		};
	}
	return false;
}

void XiangqiBasedGame::setState(const std::string& board_FEN) const {
	int line=this->nlines-1;/*FIXME: total 9 slashes */
	int column=0;
	for(int i=0;i<(int)board_FEN.size();i++) {
		if((board_FEN[i] >= '0') and (board_FEN[i] <= '9')) {
			for( int k = 0; k < board_FEN[i] - '0';k++) {
				this->gameboard->createPiece(Position(column,line), new XiangqiPiece() );
				column++;
			}
		}
		else if( board_FEN[i]=='/') {
			column=0;
			line--;
		}
		else {
			this->gameboard->createPiece(Position(column,line),new XiangqiPiece(board_FEN[i]));
			column++;
		}
	}
}

std::string XiangqiBasedGame::getPosForFEN() const {
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

bool XiangqiBasedGame::beingAttacked(const Position &p, int player) const {
	for(int i=0;i<this->nlines;i++)
		for(int j=0;j<this->ncolums;j++)
			if((*this->gameboard)[i][j]->color() == player)
				if(this->verifyPieceMove(XiangqiMove(Position(j,i),p,player)))
					return true;
	return false;
}

Position XiangqiBasedGame::findKing(int player) const {
	for(int i=0; i<this->nlines;i++)
		for(int j=0;j<this->ncolums;j++) {
			if( ((*this->gameboard)[i][j]->type() == XiangqiPiece::KING) and ((*this->gameboard)[i][j]->color() == player) )
				return Position(j,i);
		}
	return Position(-1,-1);
}

bool XiangqiBasedGame::verifyCheck(int player) const {
	if(beingAttacked(findKing(player),player^1))
    {
        return true;
    }   
    // The king of one player should not face King from the opposite player 
    else
    {
        Position from = findKing(player);
        Position to = findKing(player^1);
        if(from.x == to.x)
        {
            return VerifyVertical(XiangqiMove(from,to,player));
        }
    }
    return false;
}

std::vector <Position> *XiangqiBasedGame::getPositions(const Position& p) const {
	int jogador = this->gameboard->color(p);
	std::vector <Position> *pos = this->getpossiblePositions(p);
	std::vector <Position> *ans = new std::vector <Position>;
	for(int i=0;i<(int)pos->size();i++) {
		if(verifyMove( XiangqiMove(p,(*pos)[i],jogador) ))
			ans->push_back( (*pos)[i] );
	}
	delete pos;
	return ans;
}
//Verify if in the palace
// true - in the palace
// false - not in the palace
bool XiangqiBasedGame::VerifyJiuGongGeMove(const XiangqiMove& mv) const {
    int colordiff;
    if(mv.color() == XiangqiPiece::WHITE)
    {
        colordiff = 0;
    }
    else if(mv.color() == XiangqiPiece::BLACK)
    {
        colordiff = 7;
    }
    else
    {
        return false;
    }
    // x axis
    if((mv.from().x<=5) and (mv.from().x>=3))
    {
        if((mv.to().x<=5) and (mv.to().x>=3))
        {
        //y axis
            if((mv.from().y<=2+colordiff) and (mv.from().y>=colordiff))
            {
                if((mv.to().y<=2+colordiff) and (mv.to().y>=colordiff))
                {
                    return true;
                }            
            }            
        }
        
    }
	return false;
}


bool XiangqiBasedGame::VerifyDiagonal(const XiangqiMove& mv) const {
	Position to = mv.to();
	Position from = mv.from();
	int i,j;
	int k=(to.x-from.x)>0?1:-1;
	int l=(to.y-from.y)>0?1:-1;
	for(i=from.x+k,j=from.y+l;Position(i,j)!=to;i+=k,j+=l)
		if(this->gameboard->color(Position(i,j)) != XiangqiPiece::NOCOLOR)
			return false;
	return true;
}


bool XiangqiBasedGame::VerifyHorizontal(const XiangqiMove& mv) const {
	Position to = mv.to();
	Position from = mv.from();
	int k=(to.x-from.x)>0?1:-1;
	for(int i=from.x+k; i!=to.x ; i+=k)
		//access board and verify
		if(this->gameboard->color(Position(i,from.y)) != XiangqiPiece::NOCOLOR)
			return false;
	return true;
}

bool XiangqiBasedGame::VerifyVertical(const XiangqiMove& mv) const {
	Position to = mv.to();
	Position from = mv.from();
	int l=(to.y-from.y)>0?1:-1;

	for(int j=from.y+l; j!= to.y ; j+=l)
		if(this->gameboard->color(Position(from.x,j)) != XiangqiPiece::NOCOLOR)
			return false;
	return true;
}

bool XiangqiBasedGame::verifyHorseMove(const XiangqiMove& mv) const {
	int distx,disty;
    Position blocker;
    int searchblocker = 0;
	distx=abs(mv.from().x-mv.to().x);
	disty=abs(mv.from().y-mv.to().y);
	if(this->gameboard->color(mv.to()) == mv.color())
			return false;
// is there a piece blocking the horse.
	if( distx+disty == 3 )
    {   
		if( (distx==2 and disty==1))
        {
            if(mv.from().x<mv.to().x)
            {
                searchblocker = 1;
                blocker.x = mv.from().x+1;
                blocker.y = mv.from().y;
            }
            else if(mv.from().x>mv.to().x)
            {
                searchblocker = 1;
                blocker.x = mv.from().x-1;
                blocker.y = mv.from().y;
            }
        }
        else if(distx==1 and disty==2)
        {
            if(mv.from().y<mv.to().y)
            {
                searchblocker = 1;
                blocker.x = mv.from().x;
                blocker.y = mv.from().y+1;
            }
            else if(mv.from().y>mv.to().y)
            {
                searchblocker = 1;
                blocker.x = mv.from().x;
                blocker.y = mv.from().y-1;
            }
        }
        if(1 == searchblocker)
        {
            if(XiangqiPiece::NOCOLOR== this->gameboard->color(blocker))
                return true;
        }
        // the scenario that searchblocker==0, must return false.
    }
	return false;
}

bool XiangqiBasedGame::verifyPawnMove(const XiangqiMove& mv) const {
	int distx,disty;
	const Position& from = mv.from();
	const Position& to = mv.to();
	distx=abs(from.x-to.x);
	disty=(to.y-from.y);

	if(this->gameboard->color(to) != mv.color())
    {
        // vertical for black
        if((-1 == disty) and (0== distx))
        {
            if((mv.color() == XiangqiPiece::BLACK))
            {
                return true;
            }
        }
        // vertical for white
        else if((1 == disty) and (0== distx))
        {
            if((mv.color() == XiangqiPiece::WHITE))
            {
                return true;
            }
        }
        // horizon
        else if((0 == disty) and ((1 == distx) or (-1 == distx)))
        {
            if((mv.color() == XiangqiPiece::BLACK ) and (mv.from().y <this->nlines/2))
            {
                return true;
            }
            else if((mv.color() == XiangqiPiece::WHITE ) and (mv.from().y >=this->nlines/2))
            {
                return true;
            }
        }
    }   
    return false;
}

bool XiangqiBasedGame::verifyRookMove(const XiangqiMove& mv) const {
	int distx,disty;
	distx=abs(mv.from().x-mv.to().x);
	disty=abs(mv.from().y-mv.to().y);
    /* FIXME: why twice judgement*/
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

bool XiangqiBasedGame::verifyAdvisorMove(const XiangqiMove& mv) const {
	int distx,disty;
	const Position& to = mv.to();
	const Position& from = mv.from();
	distx=abs(from.x-to.x);
	disty=abs(from.y-to.y);
	if(this->gameboard->color(mv.to()) != mv.color())
		if( (distx == disty) and (distx == 1) ) 
			return VerifyJiuGongGeMove(mv);
	
	return false;
}

bool XiangqiBasedGame::verifyElephantMove(const XiangqiMove& mv) const {
	int distx,disty;
	distx=abs(mv.from().x-mv.to().x);
	disty=abs(mv.from().y-mv.to().y);

	if(this->gameboard->color(mv.to()) != mv.color()) {
		if((distx == 2) and (disty == 2)) {
			if( VerifyDiagonal(mv)) {
            /* only in its aspect, canot jump to another side*/
                if((mv.from().y<5) and(mv.to().y<5) and (mv.color()==XiangqiPiece::WHITE))
                {
                    return true;
                }
                else if((mv.from().y>=5) and(mv.to().y>=5) and (mv.color()==XiangqiPiece::BLACK))
                {
                    return true;
                }
            }
		}
	}
	return false;
}

bool XiangqiBasedGame::verifyKingMove(const XiangqiMove& mv) const {
	if(this->gameboard->color(mv.to()) != mv.color()) {
		int distx=abs(mv.from().x-mv.to().x);
		int disty =abs(mv.from().y-mv.to().y);
        if(distx+disty == 1)
        {
    		if( (distx ==1) or ( disty == 1))
            {
                if(VerifyJiuGongGeMove(mv))
                {
                    return true;
                }
            }      
        }
     }
    return false;
}


bool XiangqiBasedGame::verifyCannonMove(const XiangqiMove& mv) const {
	int distx,disty;
    Position to = mv.to();
    Position from = mv.from();
	distx=abs(mv.from().x-mv.to().x);
	disty=abs(mv.from().y-mv.to().y);

	if(this->gameboard->color(mv.to()) == XiangqiPiece::NOCOLOR) 
    {
        return verifyRookMove(mv);
    }
	else if(this->gameboard->color(mv.to()) != mv.color()) 
    {
		if( (distx == 0) and (disty > 0) ) 
        {
            int len;
            int blockernum =0;
            if(mv.to().y-mv.from().y>0)
                len = 1;
            else
                len = -1;
            for(int j=mv.from().y+len; j!= mv.to().y ; j+=len)
            {
                if(this->gameboard->color(Position(mv.from().x,j)) != XiangqiPiece::NOCOLOR)
                {
                    blockernum= blockernum+1;
                }
            }

            if(blockernum == 1)
                return true;
        }
		else if( (distx > 0) and (disty == 0) ) {
            int blockernum =0;
            int k=(mv.to().x-mv.from().x)>0?1:-1;
            for(int i=mv.from().x+k; i!=mv.to().x ; i+=k)
                //access board and verify
                if(this->gameboard->color(Position(i,mv.from().y)) != XiangqiPiece::NOCOLOR)
                    blockernum++;

            if(blockernum == 1)
                return true;
		}
	}
	return false;
}

std::vector <Position> *XiangqiBasedGame::getVerticalandHorizontal(const Position& p) const {
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

std::vector <Position> *XiangqiBasedGame::getCannonNextStep(const Position& p) const {
	std::vector <Position> *ans=new std::vector <Position>;
	int py[]={1,-1,0,0};
	int px[]={0,0,1,-1};
    int blockernum;
	for(int k=0;k<4;k++) {
        blockernum=0;
		for(int i=(p.y+py[k]),j=(p.x+px[k]);(i<this->nlines) and (i>=0) and (j < this->ncolums) and (j>=0);i+=py[k],j+=px[k]) {
			Position aux(j,i);
            if(blockernum>=2)
                break;
			ans->push_back(aux);
			if(this->gameboard->color(aux) != UNDEFINED)
				blockernum++;
		}
	}
	return ans;
}

std::vector <Position> *XiangqiBasedGame::getpossiblePositions(const Position& position) const {

	std::vector <Position> *p = new std::vector <Position>;
	switch(this->gameboard->getType(position)) {
		case XiangqiPiece::HORSE:
			{
				int posx[]={1,2,2,1,-1,-2,-2,-1};
				int posy[]={2,1,-1,-2,-2,-1,1,2};
				for(int k=0;k<8;k++)
                {
                    int i=position.y+posy[k];
                    int j=position.x+posx[k];
                    if((i<this->nlines) and (i>=0) and (j < this->ncolums) and (j>=0))
                    {
                        Position aux(j,i);
    					p->push_back(aux);
                    }
                }
			}
			break;
		case XiangqiPiece::PAWN:
			{
				int posx[]={0,1,0,-1};
				int posy[]={1,0,-1,0};
				for(int k=0;k<4;k++)
                {
                    int i=position.y+posy[k];
                    int j=position.x+posx[k];
                    if((i<this->nlines) and (i>=0) and (j < this->ncolums) and (j>=0))
                    {
                        Position aux(j,i);
                        p->push_back(aux);
                    }
                }
			}
			break;
		case XiangqiPiece::ROOK:
			{
				std::vector <Position> *pos = getVerticalandHorizontal(position);
				p->insert(p->end(),pos->begin(),pos->end());
				delete pos;
			}
			break;
		case XiangqiPiece::ADVISOR:
			{
				int posx[]={1,1,-1,-1};
				int posy[]={1,-1,-1,1};
				for(int k=0;k<4;k++)
                {
                    int i=position.y+posy[k];
                    int j=position.x+posx[k];
                    if((i<this->nlines) and (i>=0) and (j < this->ncolums) and (j>=0))
                    {
                        Position aux(j,i);
                        p->push_back(aux);
                    }
                }
			}
			break;
		case XiangqiPiece::ELEPHANT:
			{
				int posx[]={2,2,-2,-2};
				int posy[]={2,-2,-2,2};
				for(int k=0;k<4;k++)
                {
                    int i=position.y+posy[k];
                    int j=position.x+posx[k];
                    if((i<this->nlines) and (i>=0) and (j < this->ncolums) and (j>=0))
                    {
                        Position aux(j,i);
                        p->push_back(aux);
                    }
                }
			}
			break;
		case XiangqiPiece::KING:
			{
				int posx[]={0,1,0,-1};
				int posy[]={1,0,-1,0};
				for(int k=0;k<4;k++)
                {
                    int i=position.y+posy[k];
                    int j=position.x+posx[k];
                    if((i<this->nlines) and (i>=0) and (j < this->ncolums) and (j>=0))
                    {
                        Position aux(j,i);
                        p->push_back(aux);
                    }
                }
			}
			break;
        case XiangqiPiece::CANNON:
			{
				std::vector <Position> *pos = getCannonNextStep(position);
				p->insert(p->end(),pos->begin(),pos->end());
				delete pos;
			}
            break;
		default:
			break;
	}
	return p;
}
