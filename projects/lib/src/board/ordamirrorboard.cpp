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

#include "ordamirrorboard.h"
#include "westernzobrist.h"

namespace Chess {

OrdamirrorBoard::OrdamirrorBoard()
	: WesternBoard(new WesternZobrist())
{
    setPieceGraphics(Pawn, "OP");
    setPieceGraphics(King, "O2K");
    setPieceType(Falcon, tr("falcon"), "F", KnightMovement, "OF");
    setPieceType(Kheshig, tr("kheshig"), "H", KnightMovement | FerzMovement | WazirMovement,"OH");
    setPieceType(Archer, tr("archer"), "A", BishopMovement, "OA");
    setPieceType(Lancer, tr("lancer"), "L", RookMovement,"OL");
}

Board* OrdamirrorBoard::copy() const
{
    return new OrdamirrorBoard(*this);
}

QString OrdamirrorBoard::variant() const
{
    return "ordamirror";
}

QString OrdamirrorBoard::defaultFenString() const
{
    return "lhafkahl/8/pppppppp/8/8/PPPPPPPP/8/LHAFKAHL w - 0 1";
}

Result OrdamirrorBoard::result()
{
    if (kingAtEnd(Side::White))
        return Result(Result::Win, Side::White,
                  tr("Camp mate for White"));
    if (kingAtEnd(Side::Black))
        return Result(Result::Win, Side::Black,
                  tr("Camp Mate for Black"));
    return WesternBoard::result();
}

bool OrdamirrorBoard::kingAtEnd(Side side) const
{
    Square ksq = chessSquare(kingSquare(side));
    return (side==Side::Black && ksq.rank()==0)|((side==Side::White)&&ksq.rank()==7);
    //return (ksq.rank() == 7);
}

void OrdamirrorBoard::addPromotions(int sourceSquare,
                 int targetSquare,
                 QVarLengthArray<Move>& moves) const
{
    moves.append(Move(sourceSquare, targetSquare, Kheshig));
    moves.append(Move(sourceSquare, targetSquare, Lancer));
    moves.append(Move(sourceSquare, targetSquare, Falcon));
    moves.append(Move(sourceSquare, targetSquare, Archer));
}


void OrdamirrorBoard::vInitialize()
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


void OrdamirrorBoard::generateMovesForPiece(QVarLengthArray< Move >& moves,
					      int pieceType,
					      int square) const
{

    if (pieceType != Lancer && pieceType != Archer && pieceType != Falcon && pieceType != Kheshig)
	{
		WesternBoard::generateMovesForPiece(moves, pieceType, square);
		return;
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
    testmoves.clear();
    if (pieceType == Falcon) {
       WesternBoard::generateMovesForPiece(testmoves, Falcon, square);
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

bool OrdamirrorBoard::inCheck(Side side, int square) const
{
    Piece piece;
    Side opSide = side.opposite();
    if (square == 0)
        square = kingSquare(side);


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
