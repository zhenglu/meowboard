#ifndef XIANGQI_PIECE_HH
#define XIANGQI_PIECE_HH

#include "Piece.hh"

class XiangqiPiece : public Piece {
	public:
        // elephant=bishop, horse=Knight
		enum XiangqiPieceType {
			NOTYPE=0,KING=1,ADVISOR=2,ELEPHANT=3,HORSE=4,ROOK=5,CANNON=6,PAWN=7
		};
        //white = red
		enum XiangqiPieceColor {
			NOCOLOR=-1,WHITE=0,BLACK=1
		};

		XiangqiPiece(int __type, int __color);

		XiangqiPiece(char name);

		XiangqiPiece(const XiangqiPiece& cp);

		XiangqiPiece();

		static XiangqiPieceType chartotype(char a);

		static XiangqiPieceColor chartocolor(char a);

		virtual int pieceReal() const;

		bool operator==(const XiangqiPiece& cp) const;

	private:
};
#endif
