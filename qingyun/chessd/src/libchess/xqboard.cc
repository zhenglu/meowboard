
#include "xqboard.hh"
#include <string>
#include <typeinfo>



XiangqiBoard::XiangqiBoard(int n, int m) : Board(n,m) {
	for(int i=0;i<n;i++)
		for(int j=0;j<m;j++) {
			board[i][j]=new XiangqiPiece();
		}
}

XiangqiBoard::~XiangqiBoard() {
	for(int i=0;i<nlines;i++)
		for(int j=0;j<ncolums;j++)
			delete board[i][j];
}

void XiangqiBoard::movePiece(const Position& from, const Position& to)
{
	int tox=to.x,toy=to.y;
	int fromx=from.x,fromy=from.y;
	delete this->board[toy][tox];
	this->board[toy][tox] = board[fromy][fromx];
	this->board[fromy][fromx]=new XiangqiPiece();
}

