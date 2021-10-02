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

#include "pawnbackboard.h"
#include "westernzobrist.h"

namespace Chess {

PawnbackBoard::PawnbackBoard()
    : WesternBoard(new WesternZobrist())
{
}

Board* PawnbackBoard::copy() const
{
    return new PawnbackBoard(*this);
}

QString PawnbackBoard::variant() const
{
    return "pawnback";
}

QString PawnbackBoard::defaultFenString() const
{
	return "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
}


void PawnbackBoard::vInitialize()
{
    WesternBoard::vInitialize();

    int arwidth = width() + 2;

    m_pawnBackOffsets.resize(4); //WAZIR
    m_pawnBackOffsets[0] = -arwidth;
    m_pawnBackOffsets[1] = 0;
    m_pawnBackOffsets[2] = 0;
    m_pawnBackOffsets[3] = arwidth;
}


void PawnbackBoard::generateMovesForPiece(QVarLengthArray< Move >& moves,
                          int pieceType,
                          int square) const
{

    if (pieceType == Pawn) {
       QVarLengthArray< Move > testmoves;
       testmoves.clear();
       generateHoppingMoves(square, m_pawnBackOffsets,testmoves);
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
