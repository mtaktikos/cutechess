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

#include "centaurboard.h"
#include "westernzobrist.h"

namespace Chess {

CentaurBoard::CentaurBoard()
	: WesternBoard(new WesternZobrist())
{
    setPieceType(Centaur, tr("centaur"), "C", KnightMovement | WazirMovement | FerzMovement, "RN");
}

Board* CentaurBoard::copy() const
{
    return new CentaurBoard(*this);
}

QString CentaurBoard::variant() const
{
    return "centaur";
}

int CentaurBoard::width() const
{
	return 10;
}

QString CentaurBoard::defaultFenString() const
{
    return "rcnbqkbncr/pppppppppp/10/10/10/10/PPPPPPPPPP/RCNBQKBNCR w KQkq - 0 1";
}

void CentaurBoard::addPromotions(int sourceSquare,
				int targetSquare,
				QVarLengthArray<Move>& moves) const
{
	WesternBoard::addPromotions(sourceSquare, targetSquare, moves);

    moves.append(Move(sourceSquare, targetSquare, Centaur));
}

void CentaurBoard::vInitialize()
{
    WesternBoard::vInitialize();
    int arwidth = width() + 2;

    m_ferzOffsets.resize(4);
    m_ferzOffsets[0] = -arwidth - 1;
    m_ferzOffsets[1] = -arwidth + 1;
    m_ferzOffsets[2] = arwidth - 1;
    m_ferzOffsets[3] = arwidth + 1;

    m_wazirOffsets.resize(4);
    m_wazirOffsets[0] = -arwidth;
    m_wazirOffsets[1] = -1;
    m_wazirOffsets[2] = 1;
    m_wazirOffsets[3] = arwidth;


}
void CentaurBoard::generateMovesForPiece(QVarLengthArray< Move >& moves,
                          int pieceType,
                          int square) const
{
        if (pieceHasMovement(pieceType, square, FerzMovement))
            generateHoppingMoves(square,m_ferzOffsets, moves);
        if (pieceHasMovement(pieceType, square, WazirMovement))
            generateHoppingMoves(square, m_wazirOffsets, moves);
        WesternBoard::generateMovesForPiece(moves,pieceType,square);
}

bool CentaurBoard::inCheck(Side side, int square) const
{
    //Taken from Knightmate
    if (square == 0)
        square = kingSquare(side);

    QVarLengthArray <Move> moves;
    if (sideToMove() == side)
        for (int type = Pawn; type <= Centaur; type++)
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

} // namespace Chess
