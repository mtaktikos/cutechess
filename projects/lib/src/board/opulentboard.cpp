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

#include "opulentboard.h"

namespace Chess {

OpulentBoard::OpulentBoard()
    : GrandBoard()
{
    setPieceType(Knight, tr("knight"), "N", KnightMovement | wazirMovement, "N");
    setPieceType(Lion, tr("lion"), "L", ferzMovement | lionMovement, "LION");
    setPieceType(Wizard, tr("wizard"), "W", ferzMovement | wizardMovement, "WI");
}

Board* OpulentBoard::copy() const
{
    return new OpulentBoard(*this);
}

void OpulentBoard::addPromotions(int sourceSquare,
                int targetSquare,
                QVarLengthArray<Move>& moves) const
{
    GrandBoard::addPromotions(sourceSquare, targetSquare, moves);

    moves.append(Move(sourceSquare, targetSquare, Lion));
    moves.append(Move(sourceSquare, targetSquare, Wizard));
}

QString OpulentBoard::variant() const
{
    return "opulent";
}

void OpulentBoard::vInitialize()
{
    GrandBoard::vInitialize();
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

    m_lionOffsets.resize(8);
    m_lionOffsets[0] = 2 * (-arwidth);
    m_lionOffsets[1] = 3 * (-arwidth);
    m_lionOffsets[2] = -2;
    m_lionOffsets[3] = -3;
    m_lionOffsets[4] = 2;
    m_lionOffsets[5] = 3;
    m_lionOffsets[6] = 2 * arwidth;
    m_lionOffsets[7] = 3 * arwidth;

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
void OpulentBoard::generateMovesForPiece(QVarLengthArray< Move >& moves,
                          int pieceType,
                          int square) const
{
        if (pieceHasMovement(pieceType, square, wizardMovement))
            generateHoppingMoves(square, m_wizardOffsets, moves);
        if (pieceHasMovement(pieceType, square, ferzMovement))
            generateHoppingMoves(square,m_ferzOffsets, moves);
        if (pieceHasMovement(pieceType, square, wazirMovement))
            generateHoppingMoves(square, m_wazirOffsets, moves);
        if (pieceHasMovement(pieceType, square, lionMovement))
            generateHoppingMoves(square, m_lionOffsets, moves);
        GrandBoard::generateMovesForPiece(moves,pieceType,square);
}

bool OpulentBoard::inCheck(Side side, int square) const
{
    //Taken from Knightmate
    if (square == 0)
        square = kingSquare(side);

    QVarLengthArray <Move> moves;
    if (sideToMove() == side)
        for (int type = Pawn; type <= Wizard; type++)
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

bool OpulentBoard::vIsLegalMove(const Move& move)
{
    int promotion = move.promotion();

    if (promotion == Piece::NoPiece)
        return Chess::GrandBoard::vIsLegalMove(move);

    // only allow promotion to already captured piece
    Side side = sideToMove();
    int count = 1;

    for (int i = 0; i < arraySize(); i++)
    {
        Piece piece = pieceAt(i);
        if (piece.side() == side && piece.type() == promotion)
            count++;
    }

    if (promotion == Wizard
    ||  promotion == Lion)
        return count <= 2
        &&     CapablancaBoard::vIsLegalMove(move);

    return GrandBoard::vIsLegalMove(move);
}

QString OpulentBoard::defaultFenString() const
{
    return "rw6wr/clbnqknbla/pppppppppp/10/10/10/10/PPPPPPPPPP/CLBNQKNBLA/RW6WR w - - 0 1";
}

} // namespace Chess
