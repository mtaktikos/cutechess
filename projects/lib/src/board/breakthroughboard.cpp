/*
    This file is part of Cute Chess.
    Copyright (C) 2008-2018 Cute Chess authors

    Cute Chess is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Cute Chess is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Cute Chess.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "breakthroughboard.h"

namespace Chess {

BreakthroughBoard::BreakthroughBoard()
    : StandardBoard()
{
    setPieceType(King, tr("no-king"), 0);
    setPieceType(Rook, tr("no-rook"), 0);
    setPieceType(Bishop, tr("no-bishop"), 0);
    setPieceType(Knight, tr("no-knight"), 0);
    setPieceType(Pawn, tr("no-queen"), 0);
    setPieceType(Queen, tr("pawn"),"P",0,"P");
}

Board* BreakthroughBoard::copy() const
{
    return new BreakthroughBoard(*this);
}

QString BreakthroughBoard::variant() const
{
    return "breakthrough";
}

bool BreakthroughBoard::hasCastling() const
{
	return false;
}

bool BreakthroughBoard::kingsCountAssertion( int whiteKings,
				     int blackKings) const
{
	return whiteKings + blackKings >= 0;
}

void BreakthroughBoard::addPromotions(int,
                int,
                QVarLengthArray<Move>&) const
{
}

void BreakthroughBoard::vInitialize()
{
    StandardBoard::vInitialize();
    int arwidth = width() + 2;

    m_ferzOffsets[Side::White].resize(2);
    m_ferzOffsets[Side::White][0] = -arwidth - 1;
    m_ferzOffsets[Side::White][1] = -arwidth + 1;
    m_ferzOffsets[Side::Black].resize(2);
    m_ferzOffsets[Side::Black][0] = arwidth - 1;
    m_ferzOffsets[Side::Black][1] = arwidth + 1;

    m_pawnOffsets[Side::White].resize(1);
    m_pawnOffsets[Side::White][0] = -arwidth;
    m_pawnOffsets[Side::Black].resize(1);
    m_pawnOffsets[Side::Black][0] = arwidth;
}
QString BreakthroughBoard::defaultFenString() const
{
    return "pppppppp/pppppppp/8/8/8/8/PPPPPPPP/PPPPPPPP w - - 0 1";
}

bool BreakthroughBoard::inCheck(Side, int) const
{
	return false;
}

int BreakthroughBoard::reversibleMoveCount() const {
    return 1;
}

Result BreakthroughBoard::vResultOfStalemate() const
{
    Side winner = sideToMove().opposite();
	QString str { tr("%1 wins").arg(winner.toString()) };
	return Result(Result::Win, winner, str);
}

void BreakthroughBoard::generateMovesForPiece(QVarLengthArray<Move>& moves,
                        int pieceType,
                        int square) const
{
    Side side = sideToMove();
    if (pieceType == Queen)
    {
       generateHoppingMoves(square, m_ferzOffsets[side], moves);
       QVarLengthArray< Move > testmoves;
       testmoves.clear();
       generateHoppingMoves(square, m_pawnOffsets[side], testmoves);
        for (const auto m: testmoves)
        {
            const bool isCapture = captureType(m) != Piece::NoPiece;
            if (!isCapture) { moves.append(m); }
        }
    }
}

bool BreakthroughBoard::pawnAtEnd(Side side) {
    Square sq;
    sq.setFile(0);
    if (side == side.White)
        sq.setRank(7);
    else
        sq.setRank(0);
    for (int i=0; i<9 ;i++) {
        sq.setFile(i);
        if (pieceAt(sq).side() == side)
            return true;
    }
    return false;
}
Result BreakthroughBoard::result()
{

    QString str;
    bool blackFinished = pawnAtEnd(Side::Black);
    bool whiteFinished = pawnAtEnd(Side::White);

    // Finishing on eighth rank
    if (whiteFinished)
    {
        str = tr("White wins Pawn at the end");
        return Result(Result::Win, Side::White, str);
    }

    if (blackFinished)
    {
        str = tr("Black wins Pawn at the end");
        return Result(Result::Win, Side::Black, str);
    }

    // stalemate or no pieces left
	if (!canMove())
		return vResultOfStalemate();


	// 3-fold repetition
	if (repeatCount() >= 2)
	{
		str = tr("Draw by 3-fold repetition");
		return Result(Result::Draw, Side::NoSide, str);
	}

	return Result();
}

} // namespace Chess
