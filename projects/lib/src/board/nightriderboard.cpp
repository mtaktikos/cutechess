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

#include "nightriderboard.h"
#include "westernzobrist.h"


namespace Chess {

NightriderBoard::NightriderBoard()
	: WesternBoard(new WesternZobrist())
{
    setPieceType(Nightrider, tr("nightrider"), "N", NightriderMovement, "N");
}


void NightriderBoard::vInitialize()
{
    int arwidth = width() + 2;

    m_knightOffsets.resize(8);
    m_knightOffsets[0] = -2 * arwidth - 1;
    m_knightOffsets[1] = -2 * arwidth + 1;
    m_knightOffsets[2] = -arwidth - 2;
    m_knightOffsets[3] = -arwidth + 2;
    m_knightOffsets[4] = arwidth - 2;
    m_knightOffsets[5] = arwidth + 2;
    m_knightOffsets[6] = 2 * arwidth - 1;
    m_knightOffsets[7] = 2 * arwidth + 1;
    WesternBoard::vInitialize();
}

Board* NightriderBoard::copy() const
{
    return new NightriderBoard(*this);
}

QString NightriderBoard::variant() const
{
    return "nightrider";
}

QString NightriderBoard::defaultFenString() const
{
    return "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
}

bool NightriderBoard::inCheck(Side side, int square) const
{
    //Taken from Knightmate
    if (square == 0)
        square = kingSquare(side);

    QVarLengthArray <Move> moves;
    if (sideToMove() == side)
        for (int type = Pawn; type <= King; type++)
        {
            generateMovesForPiece(moves, type, square);
            for (const auto m: moves)
            {
                if (captureType(m) == type)
                    return true;
            }
            moves.clear();
        }
    else
    {
        generateMoves(moves);
        for (const auto m: moves)
        {
            if (m.targetSquare() == square)
                return true;
        }
    }
    return false;
}

void NightriderBoard::generateMovesForPiece(QVarLengthArray< Move >& moves,
                          int pieceType,
                          int square) const
{
    if (pieceHasMovement(pieceType, square, NightriderMovement))
        generateSlidingMoves(square, m_knightOffsets,moves);
    WesternBoard::generateMovesForPiece(moves, pieceType, square);
}

} // namespace Chess
