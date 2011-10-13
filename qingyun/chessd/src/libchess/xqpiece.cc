
#include <ctype.h>
#include <cstdlib>
#include "xqpiece.hh"

char XiangqiTabelaChar[]=".KAEHRCP";

XiangqiPiece::XiangqiPiece(int __type, int __color) : Piece(__type,__color) {
}

XiangqiPiece::XiangqiPiece(char name) : Piece(chartotype(name),chartocolor(name)) {
}

XiangqiPiece::XiangqiPiece(const XiangqiPiece& cp) : Piece(cp.type(),cp.color()) {
}

XiangqiPiece::XiangqiPiece() : Piece(NOTYPE,NOCOLOR) {
}

XiangqiPiece::XiangqiPieceType XiangqiPiece::chartotype(char a) {
	a=toupper(a);
	switch(a) {
		case '.':
			return NOTYPE;
		case 'K':
			return KING;
		case 'A':
			return ADVISOR;
		case 'E':
			return ELEPHANT;
		case 'H':
			return HORSE;
		case 'R':
			return ROOK;
        case 'C':
            return CANNON;
		case 'P':
			return PAWN;
	}
	return NOTYPE;
}

XiangqiPiece::XiangqiPieceColor XiangqiPiece::chartocolor(char name) {
	return (name=='.')?NOCOLOR:((name>='a') and (name<='z'))?BLACK:WHITE;
}

int XiangqiPiece::pieceReal() const {
	return (int)(this->color() == WHITE ? (XiangqiTabelaChar[this->type()]) : (tolower(XiangqiTabelaChar[this->type()])));
}

bool XiangqiPiece::operator==(const XiangqiPiece& cp) const {
	return this->pieceReal()==cp.pieceReal();
}

