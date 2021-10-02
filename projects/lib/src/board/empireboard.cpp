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

#include "empireboard.h"
#include "westernzobrist.h"

namespace Chess {

EmpireBoard::EmpireBoard()
    : WesternBoard(new WesternZobrist())
{
    setPieceGraphics(Pawn, "EP");
    setPieceGraphics(King, "EK");
    setPieceGraphics(Queen, "EQ");
    setPieceType(Soldier, tr("soldier"), "S", SoldierMovement, "ES"); //SS
    setPieceType(Cardinal, tr("cardinal"), "C", BishopMovement ,"EC"); //EC
    setPieceType(Duke, tr("duke"), "D", FerzMovement | WazirMovement, "ED"); //ED
    setPieceType(Eagle, tr("eagle"), "E", KnightMovement,"EE"); //EE
    setPieceType(Tower, tr("tower"), "T", RookMovement, "ET"); //ET
}

Board* EmpireBoard::copy() const
{
    return new EmpireBoard(*this);
}

QString EmpireBoard::variant() const
{
    return "empire";
}


QString EmpireBoard::defaultFenString() const
{
    return "rnbqkbnr/pppppppp/8/8/8/PPPSSPPP/8/TECDKCET w kq - 0 1";
}

Result EmpireBoard::result()
{
    if (kingAtEnd(Side::White))
        return Result(Result::Win, Side::White,
                  tr("Camp mate for White"));
    if (kingAtEnd(Side::Black))
        return Result(Result::Win, Side::Black,
                  tr("Camp Mate for Black"));
    QString str;

    // Checkmate/Stalemate
    if (!canMove())
    {
        if (inCheck(sideToMove()))
        {
            Side winner = sideToMove().opposite();
            str = tr("%1 mates").arg(winner.toString());

            return Result(Result::Win, winner, str);
        }
        else
        {
            str = tr("Stalemate");
            return Result(Result::Win, sideToMove().opposite(), str);
        }
    }



    if (reversibleMoveCount() >= 100)
    {
        str = tr("Draw by fifty moves rule");
        return Result(Result::Draw, Side::NoSide, str);
    }

    // 3-fold repetition


    if (repeatCount() >= 2)
    {
        str = tr("3-fold repetition");
        return Result(Result::Draw, Side::NoSide, str);
    }

    return Result();
}

bool EmpireBoard::kingAtEnd(Side side) const
{
    Square ksq = chessSquare(kingSquare(side));
    return (side==Side::Black && ksq.rank()==0)|((side==Side::White)&&ksq.rank()==7);
    //return (ksq.rank() == 7);
}

void EmpireBoard::addPromotions(int sourceSquare,
                 int targetSquare,
                 QVarLengthArray<Move>& moves) const
{
    moves.append(Move(sourceSquare, targetSquare, Queen));
}

inline void rotateAndStoreOffsets(QVarLengthArray<int> a[2])
{
    a[Side::Black].resize(a[Side::White].size());
    for (int i = 0; i < a[Side::White].count(); i++)
        a[Side::Black][i] = -a[Side::White][i];
}

void EmpireBoard::vInitialize()
{
    WesternBoard::vInitialize();
    int arwidth = width() + 2;
    m_soldierOffsets.resize(3);
    m_soldierOffsets[0] = -arwidth;
    m_soldierOffsets[1] = -1;
    m_soldierOffsets[2] = 1;
    //soldier offsets taken from https://github.com/gaintpd/cutechess

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


void EmpireBoard::generateMovesForPiece(QVarLengthArray< Move >& moves,
                          int pieceType,
                          int square) const
{

    if (pieceType != Soldier && pieceType != Duke && pieceType != Cardinal && pieceType != Tower && pieceType != Eagle)
    {
        WesternBoard::generateMovesForPiece(moves, pieceType, square);
        return;
    }

    if (pieceHasMovement(pieceType, square, SoldierMovement)) {
        generateHoppingMoves(square, m_soldierOffsets, moves);
    }

    QVarLengthArray< Move > testmoves;
    testmoves.clear();
    if (pieceType == Cardinal) {
       WesternBoard::generateMovesForPiece(testmoves, Cardinal, square);
        for (const auto m: testmoves)
        {
            const bool isCapture = captureType(m) != Piece::NoPiece;
            if (isCapture) { moves.append(m); }
        }
       testmoves.clear();
       WesternBoard::generateMovesForPiece(testmoves, Queen, square);
       for (const auto m: testmoves)
       {
           const bool isCapture = captureType(m) != Piece::NoPiece;
           if (!isCapture) { moves.append(m); }
       }
    }
    testmoves.clear();

    if (pieceType == Eagle) {
       WesternBoard::generateMovesForPiece(testmoves, Eagle, square);
        for (const auto m: testmoves)
        {
            const bool isCapture = captureType(m) != Piece::NoPiece;
            if (isCapture) { moves.append(m); }
        }
       testmoves.clear();
       WesternBoard::generateMovesForPiece(testmoves, Queen, square);
       for (const auto m: testmoves)
       {
           const bool isCapture = captureType(m) != Piece::NoPiece;
           if (!isCapture) { moves.append(m); }
       }
    }
    testmoves.clear();

    if (pieceType == Tower) {
       WesternBoard::generateMovesForPiece(testmoves, Tower, square);
        for (const auto m: testmoves)
        {
            const bool isCapture = captureType(m) != Piece::NoPiece;
            if (isCapture) { moves.append(m); }
        }
       testmoves.clear();
       WesternBoard::generateMovesForPiece(testmoves, Queen, square);
       for (const auto m: testmoves)
       {
           const bool isCapture = captureType(m) != Piece::NoPiece;
           if (!isCapture) { moves.append(m); }
       }
    }

    testmoves.clear();

    if (pieceType == Duke) {
        generateHoppingMoves(square, m_ferzOffsets, testmoves);
        generateHoppingMoves(square, m_wazirOffsets, testmoves);
        for (const auto m: testmoves)
        {
            const bool isCapture = captureType(m) != Piece::NoPiece;
            if (isCapture) { moves.append(m); }
        }
       testmoves.clear();
       WesternBoard::generateMovesForPiece(testmoves, Queen, square);
       for (const auto m: testmoves)
       {
           const bool isCapture = captureType(m) != Piece::NoPiece;
           if (!isCapture) { moves.append(m); }
       }
    }

}

bool EmpireBoard::inCheck(Side side, int square) const
{
    Piece piece;
    Side opSide = side.opposite();
    if (square == 0)
        square = kingSquare(side);

    QVarLengthArray< Move > testmoves;
    testmoves.clear();
    generateMovesForPiece(testmoves, Rook, kingSquare(side.opposite()));
    for (const auto m: testmoves)
    {
        if (captureType(m) == King) { return true; }
    }


    for (int i = 0; i < m_soldierOffsets.size(); i++)
    {
        piece = pieceAt(square - m_soldierOffsets[i]);
        if (piece.side() == opSide
        &&  pieceHasMovement(piece.type(), square, SoldierMovement))
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
