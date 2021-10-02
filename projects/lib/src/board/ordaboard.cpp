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

#include "ordaboard.h"
#include "westernzobrist.h"

namespace Chess {

OrdaBoard::OrdaBoard()
    : WesternBoard(new WesternZobrist())
{
    setPieceGraphics(Pawn, "OP");
    setPieceGraphics(King, "OK");
    setPieceType(Yurt, tr("yurt"), "Y", YurtMovement, "OY");
    setPieceType(Kheshig, tr("kheshig"), "H", KnightMovement | FerzMovement | WazirMovement,"OH");
    setPieceType(Archer, tr("archer"), "A", BishopMovement, "OA");
    setPieceType(Lancer, tr("lancer"), "L", RookMovement,"OL");
}

Board* OrdaBoard::copy() const
{
    return new OrdaBoard(*this);
}

QString OrdaBoard::variant() const
{
    return "orda";
}


QString OrdaBoard::defaultFenString() const
{
    return "lhaykahl/8/pppppppp/8/8/8/PPPPPPPP/RNBQKBNR w KQ - 0 1";
}

Result OrdaBoard::result()
{
    if (kingAtEnd(Side::White))
        return Result(Result::Win, Side::White,
                  tr("Camp mate for White"));
    if (kingAtEnd(Side::Black))
        return Result(Result::Win, Side::Black,
                  tr("Camp Mate for Black"));
    return WesternBoard::result();
}

bool OrdaBoard::kingAtEnd(Side side) const
{
    Square ksq = chessSquare(kingSquare(side));
    return (side==Side::Black && ksq.rank()==0)|((side==Side::White)&&ksq.rank()==7);
    //return (ksq.rank() == 7);
}

void OrdaBoard::addPromotions(int sourceSquare,
                 int targetSquare,
                 QVarLengthArray<Move>& moves) const
{
    moves.append(Move(sourceSquare, targetSquare, Kheshig));
    moves.append(Move(sourceSquare, targetSquare, Queen));
}

inline void rotateAndStoreOffsets(QVarLengthArray<int> a[2])
{
    a[Side::Black].resize(a[Side::White].size());
    for (int i = 0; i < a[Side::White].count(); i++)
        a[Side::Black][i] = -a[Side::White][i];
}

void OrdaBoard::vInitialize()
{
    WesternBoard::vInitialize();
    int arwidth = width() + 2;
    m_YurtOffsets[Side::White].resize(5);
    m_YurtOffsets[Side::White][0] = -arwidth - 1;
    m_YurtOffsets[Side::White][1] = -arwidth + 1;
    m_YurtOffsets[Side::White][2] = arwidth - 1;
    m_YurtOffsets[Side::White][3] = arwidth + 1;
    m_YurtOffsets[Side::White][4] = -arwidth;

    rotateAndStoreOffsets(m_YurtOffsets);

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


void OrdaBoard::generateMovesForPiece(QVarLengthArray< Move >& moves,
                          int pieceType,
                          int square) const
{

    if (pieceType != Lancer && pieceType != Archer && pieceType != Yurt && pieceType != Kheshig)
    {
        WesternBoard::generateMovesForPiece(moves, pieceType, square);
        return;
    }
    if (pieceHasMovement(pieceType, square, YurtMovement)) {
        generateHoppingMoves(square, m_YurtOffsets[sideToMove()], moves);
       }
    if (pieceType == Kheshig) {
        WesternBoard::generateMovesForPiece(moves, Knight, square);
        generateHoppingMoves(square, m_wazirOffsets, moves);
        generateHoppingMoves(square, m_ferzOffsets, moves);
    }
    // Lancer and Archer
    QVarLengthArray< Move > testmoves;
    testmoves.clear();
    if (pieceType == Lancer) {
       WesternBoard::generateMovesForPiece(testmoves, Lancer, square);
        for (const auto m: testmoves)
        {
            const bool isCapture = captureType(m) != Piece::NoPiece;
            if (isCapture) { moves.append(m); }
        }
       testmoves.clear();
       WesternBoard::generateMovesForPiece(testmoves, Knight, square);
       for (const auto m: testmoves)
       {
           const bool isCapture = captureType(m) != Piece::NoPiece;
           if (!isCapture) { moves.append(m); }
       }
    }
    testmoves.clear();

    if (pieceType == Archer) {
       WesternBoard::generateMovesForPiece(testmoves, Archer, square);
        for (const auto m: testmoves)
        {
            const bool isCapture = captureType(m) != Piece::NoPiece;
            if (isCapture) { moves.append(m); }
        }
       testmoves.clear();
       WesternBoard::generateMovesForPiece(testmoves, Knight, square);
       for (const auto m: testmoves)
       {
           const bool isCapture = captureType(m) != Piece::NoPiece;
           if (!isCapture) { moves.append(m); }
       }
    }

}

bool OrdaBoard::inCheck(Side side, int square) const
{
    Piece piece;
    Side opSide = side.opposite();
    if (square == 0)
        square = kingSquare(side);

    for (int i = 0; i < m_YurtOffsets[side].size(); i++)
    {
        piece = pieceAt(square + m_YurtOffsets[side][i]);
        if (piece.side() == opSide
        &&  pieceHasMovement(piece.type(), square, YurtMovement))
            return true;
    }

    for (int i = 0; i < m_ferzOffsets.size(); i++)
    {
        piece = pieceAt(square + m_ferzOffsets[i]);
        if (piece.side() == opSide
        &&  pieceHasMovement(piece.type(), square, FerzMovement))
            return true;
    }

    for (int i = 0; i < m_wazirOffsets.size(); i++)
    {
        piece = pieceAt(square + m_wazirOffsets[i]);
        if (piece.side() == opSide
        &&  pieceHasMovement(piece.type(), square, WazirMovement))
            return true;
    }

    return WesternBoard::inCheck(side, square);
}

} // namespace Chess
