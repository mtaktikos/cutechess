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

#include "pawnsidewaysboard.h"
#include "westernzobrist.h"

namespace Chess {

PawnsidewaysBoard::PawnsidewaysBoard()
    : WesternBoard(new WesternZobrist())
{
}

Board* PawnsidewaysBoard::copy() const
{
    return new PawnsidewaysBoard(*this);
}

QString PawnsidewaysBoard::variant() const
{
    return "pawnsideways";
}

QString PawnsidewaysBoard::defaultFenString() const
{
	return "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
}


void PawnsidewaysBoard::vInitialize()
{
    WesternBoard::vInitialize();

    m_pawnSideOffsets.resize(3);
    m_pawnSideOffsets[0] = 0;
    m_pawnSideOffsets[1] = -1;
    m_pawnSideOffsets[2] = 1;
}


void PawnsidewaysBoard::generateMovesForPiece(QVarLengthArray< Move >& moves,
                          int pieceType,
                          int square) const
{

    if (pieceType == Pawn) {
       QVarLengthArray< Move > testmoves;
       testmoves.clear();
       generateHoppingMoves(square, m_pawnSideOffsets,testmoves);
       for (const auto m: testmoves)
       {
            const bool isCapture = captureType(m) != Piece::NoPiece;
            if (!isCapture) { moves.append(m); }
       }
       testmoves.clear();
    }

     WesternBoard::generateMovesForPiece(moves,pieceType,square);
}

} // namespace Chess
