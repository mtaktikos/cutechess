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

#include "tencubedboard.h"

namespace Chess {

TencubedBoard::TencubedBoard()
	: CapablancaBoard()
{
    setPieceType(Chancellor, tr("chancellor"), "M", KnightMovement | RookMovement, "C");
    setPieceType(Wizard, tr("Wizard"), "W", wizardMovement | ferzMovement, "WI");
    setPieceType(Champion, tr("Champion"), "C", dabbaMovement | wazirMovement | alfilMovement, "CHA");
}

Board* TencubedBoard::copy() const
{
    return new TencubedBoard(*this);
}

QString TencubedBoard::variant() const
{
    return "tencubed";
}

int TencubedBoard::height() const
{
	return 10;
}

QString TencubedBoard::defaultFenString() const
{
    return "2cwamwc2/1rnbqkbnr1/pppppppppp/10/10/10/10/PPPPPPPPPP/1RNBQKBNR1/2CWAMWC2 w - - 0 1";
}

bool TencubedBoard::hasCastling() const
{
	return false;
}

inline int TencubedBoard::pawnMoveOffset(const PawnStep& ps, int sign) const
{
	return sign * ps.file - sign * (width() + 2) * 1;
}

void TencubedBoard::addPromotions(int sourceSquare,
                int targetSquare,
                QVarLengthArray<Move>& moves) const
{
    moves.append(Move(sourceSquare, targetSquare, Archbishop));
    moves.append(Move(sourceSquare, targetSquare, Chancellor));
    moves.append(Move(sourceSquare, targetSquare, Queen));
}

void TencubedBoard::generateMovesForPiece(QVarLengthArray< Move >& moves,
				       int pieceType,
				       int square) const
{

	Chess::CapablancaBoard::generateMovesForPiece(moves, pieceType, square);
    if (pieceHasMovement(pieceType, square, wizardMovement))
        generateHoppingMoves(square, m_wizardOffsets, moves);
    if (pieceHasMovement(pieceType, square, ferzMovement))
        generateHoppingMoves(square,m_ferzOffsets, moves);
    if (pieceHasMovement(pieceType, square, wazirMovement))
        generateHoppingMoves(square, m_wazirOffsets, moves);
    if (pieceHasMovement(pieceType, square, dabbaMovement))
        generateHoppingMoves(square, m_dabbaOffsets, moves);
    if (pieceHasMovement(pieceType, square, alfilMovement))
        generateHoppingMoves(square, m_alfilOffsets, moves);
    if (pieceType != Pawn)
		return;

	Side side = sideToMove();

	int sign = (side == Side::White) ? 1 : -1;
	int rank = chessSquare(square).rank();

	// add missing pawn double steps from third rank
	int rank3 = (side == Side::White) ? 2 : height() - 3;
	if (rank == rank3)
	{
		for (const PawnStep& pStep: m_pawnSteps)
		{
			if (pStep.type != FreeStep)
				continue;

			int targetSquare = square + pawnMoveOffset(pStep, sign);
			Piece capture = pieceAt(targetSquare);

			if (capture.isEmpty())
			{
				targetSquare += pawnMoveOffset(pStep, sign);
				capture = pieceAt(targetSquare);
				if (capture.isEmpty())
					moves.append(Move(square, targetSquare));
			}
		}
	}
}

void TencubedBoard::vInitialize()
{
    CapablancaBoard::vInitialize();
    int arwidth = width() + 2;

    m_ferzOffsets.resize(4);
    m_ferzOffsets[0] = -arwidth - 1;
    m_ferzOffsets[1] = -arwidth + 1;
    m_ferzOffsets[2] = arwidth - 1;
    m_ferzOffsets[3] = arwidth + 1;

    m_alfilOffsets.resize(4);
    m_alfilOffsets[0] = -2 * arwidth - 2;
    m_alfilOffsets[1] = -2 * arwidth + 2;
    m_alfilOffsets[2] = 2 * arwidth - 2;
    m_alfilOffsets[3] = 2 * arwidth + 2;

    m_wazirOffsets.resize(4);
    m_wazirOffsets[0] = -arwidth;
    m_wazirOffsets[1] = -1;
    m_wazirOffsets[2] = 1;
    m_wazirOffsets[3] = arwidth;

    m_dabbaOffsets.resize(4);
    m_dabbaOffsets[0] = 2*(-arwidth);
    m_dabbaOffsets[1] = -2;
    m_dabbaOffsets[2] = 2;
    m_dabbaOffsets[3] = 2*arwidth;

    m_wizardOffsets.resize(8);
    m_wizardOffsets[0] = -3 * arwidth - 1;
    m_wizardOffsets[1] = -3 * arwidth + 1;
    m_wizardOffsets[2] = -arwidth - 3;
    m_wizardOffsets[3] = -arwidth + 3;
    m_wizardOffsets[4] = arwidth - 3;
    m_wizardOffsets[5] = arwidth + 3;
    m_wizardOffsets[6] = 3 * arwidth - 1;
    m_wizardOffsets[7] = 3 * arwidth + 1;

}

bool TencubedBoard::inCheck(Side side, int square) const
{
    //Taken from Knightmate
    if (square == 0)
        square = kingSquare(side);

    QVarLengthArray <Move> moves;
    if (sideToMove() == side)
        for (int type = Pawn; type <= Champion; type++)
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
