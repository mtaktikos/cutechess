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

#include "grasshopperboard.h"
#include "westernzobrist.h"


namespace Chess {

GrasshopperBoard::GrasshopperBoard()
	: WesternBoard(new WesternZobrist())
{
    setPieceType(Grasshopper, tr("Grasshopper"), "G", GrasshopperMovement, "G");
}


void GrasshopperBoard::vInitialize()
{
    int arwidth = width() + 2;
    m_bishopOffsets.resize(4);
    m_bishopOffsets[0] = -arwidth - 1;
    m_bishopOffsets[1] = -arwidth + 1;
    m_bishopOffsets[2] = arwidth - 1;
    m_bishopOffsets[3] = arwidth + 1;

    m_rookOffsets.resize(4);
    m_rookOffsets[0] = -arwidth;
    m_rookOffsets[1] = -1;
    m_rookOffsets[2] = 1;
    m_rookOffsets[3] = arwidth;

    WesternBoard::vInitialize();

}

Board* GrasshopperBoard::copy() const
{
    return new GrasshopperBoard(*this);
}

QString GrasshopperBoard::variant() const
{
    return "grasshopper";
}

void GrasshopperBoard::addPromotions(int sourceSquare,
                int targetSquare,
                QVarLengthArray<Move>& moves) const
{
    WesternBoard::addPromotions(sourceSquare, targetSquare, moves);

    moves.append(Move(sourceSquare, targetSquare, Grasshopper));
}
QString GrasshopperBoard::defaultFenString() const
{
    return "rnbqkbnr/gggggggg/pppppppp/8/8/PPPPPPPP/GGGGGGGG/RNBQKBNR w KQkq - 0 1";
}

bool GrasshopperBoard::inCheck(Side side, int square) const
{
    //Taken from Knightmate
    if (square == 0)
        square = kingSquare(side);

    QVarLengthArray <Move> moves;
    if (sideToMove() == side)
        for (int type = Pawn; type <= Grasshopper; type++)
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

void GrasshopperBoard::generateMovesForPiece(QVarLengthArray< Move >& moves,
                          int pieceType,
                          int square) const
{
    if (pieceHasMovement(pieceType, square, GrasshopperMovement))
        generateGrasshopperMoves(moves, square);
    WesternBoard::generateMovesForPiece(moves, pieceType, square);
}

void GrasshopperBoard::generateGrasshopperMoves(QVarLengthArray< Move >& moves,int square) const {

    //Rook Moves
    bool obstacle=false;
    QVarLengthArray<int> grasshopperRelOffsets;
    Side side = sideToMove();
    grasshopperRelOffsets.clear();
    for (int i = 0; i < m_rookOffsets.size(); i++)
    {
        int offset = m_rookOffsets[i];
        int targetSquare = square + offset;

        Piece capture;
        while(!(capture = pieceAt(targetSquare)).isWall())
        {
            if (obstacle)
            {
               if (capture.isEmpty() || capture.side() != side)
                   grasshopperRelOffsets.append(targetSquare - square);
               break;
            }
            else {
                if(!capture.isEmpty())
                    obstacle=true;
            }
         targetSquare += offset;
        }
        obstacle=false;

     }
    generateHoppingMoves(square, grasshopperRelOffsets, moves);
    grasshopperRelOffsets.clear();
    obstacle=false;

    //Bishop Moves
    for (int i = 0; i < m_bishopOffsets.size(); i++)
    {
        int offset = m_bishopOffsets[i];
        int targetSquare = square + offset;

        Piece capture;
        while(!(capture = pieceAt(targetSquare)).isWall())
        {
            if (obstacle)
            {
               if (capture.isEmpty() || capture.side() != side)
                   grasshopperRelOffsets.append(targetSquare - square);
               break;
            }
            else {
                if(!capture.isEmpty())
                    obstacle=true;
            }
         targetSquare += offset;
        }
        obstacle=false;
     }


    generateHoppingMoves(square, grasshopperRelOffsets, moves);
     //Diagonal Moves
}

} // namespace Chess
