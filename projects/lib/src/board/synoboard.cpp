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

#include "synoboard.h"
#include "pocketknightboard.h"

namespace Chess {

SynoBoard::SynoBoard()
    : PocketKnightBoard()
{
    setPieceType(Advisor, tr("advisor"),"A",FerzMovement | WazirMovement, "SA");
    setPieceType(Elephant, tr("elephant"), "E", ElephantMovement, "SE");
    setPieceType(Cannon, tr("cannon"), "C", CannonMovement, "SC");
    setPieceType(Soldier, tr("soldier"), "S", SoldierMovement, "SS");
}

Board* SynoBoard::copy() const
{
    return new SynoBoard(*this);
}

QString SynoBoard::variant() const
{
    return "synochess";
}


bool SynoBoard::kingAtEnd(Side side) const
{
    Square ksq = chessSquare(kingSquare(side));
    return (side==Side::Black && ksq.rank()==0)|((side==Side::White)&&ksq.rank()==7);
    //return (ksq.rank() == 7);
}


QList< Piece > SynoBoard::reservePieceTypes() const
{
    QList<Piece> list;

    list << Piece(Side::Black, Soldier);

    return list;
}

QString SynoBoard::defaultFenString() const
{
    return "rneakenr/8/1c4c1/1ss2ss1/8/8/PPPPPPPP/RNBQKBNR[ss] w KQ - 0 1";
}

Result SynoBoard::result()
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


inline void rotateAndStoreOffsets(QVarLengthArray<int> a[2])
{
    a[Side::Black].resize(a[Side::White].size());
    for (int i = 0; i < a[Side::White].count(); i++)
        a[Side::Black][i] = -a[Side::White][i];
}

void SynoBoard::vInitialize()
{
    PocketKnightBoard::vInitialize();
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

    m_elephantOffsets.resize(4);
    m_elephantOffsets[0] = -2 * arwidth - 2;
    m_elephantOffsets[1] = -2 * arwidth + 2;
    m_elephantOffsets[2] = 2 * arwidth - 2;
    m_elephantOffsets[3] = 2 * arwidth + 2;
    //Elephant Offsets from Shatranj

    m_orthogonalOffsets.resize(4);
    m_orthogonalOffsets[0] = -arwidth;
    m_orthogonalOffsets[1] = -1;
    m_orthogonalOffsets[2] =  1;
    m_orthogonalOffsets[3] =  arwidth;
    //Cannon taken from https://github.com/gaintpd/cutechess

    m_soldierOffsets.resize(3);
    m_soldierOffsets[0] = arwidth;
    m_soldierOffsets[1] = -1;
    m_soldierOffsets[2] = 1;
    //soldier offsets taken from https://github.com/gaintpd/cutechess

}


void SynoBoard::generateMovesForPiece(QVarLengthArray< Move >& moves,
					      int pieceType,
					      int square) const
{

    if (square == 0)
    {
        for (int i = 51; i < 59; i++) //all squares from a5 to h5 are allowed to do drops for black, not considering custom FENs, Squares -> (x,y) (file,rank)
        {
            Piece tmp = pieceAt(i);
            if (!tmp.isEmpty())
                continue;
            moves.append(Move(0, i, pieceType));
        }
    }
    else
    WesternBoard::generateMovesForPiece(moves, pieceType, square);
    if (pieceHasMovement(pieceType, square, FerzMovement | WazirMovement)) {
        generateHoppingMoves(square, m_ferzOffsets, moves);
        generateHoppingMoves(square, m_wazirOffsets, moves);
       }
    if (pieceHasMovement(pieceType, square, ElephantMovement)) {
        generateHoppingMoves(square, m_elephantOffsets, moves);
        generateHoppingMoves(square, m_ferzOffsets, moves);
    }
    if (pieceHasMovement(pieceType, square, SoldierMovement)) {
        generateHoppingMoves(square, m_soldierOffsets, moves);
    }
    if (pieceHasMovement(pieceType, square, CannonMovement)) { // Code taken from https://github.com/gaintpd/cutechess, the janggi adaptation
        QVarLengthArray<int> cannonRelOffsets;
        cannonRelOffsets.clear();
        Side side = sideToMove();
        for (int i = 0; i < m_orthogonalOffsets.size(); i++)
        {
            int offset = m_orthogonalOffsets[i];
            int targetSquare = square + offset;

            Piece capture;
            int obstacle = 0;
            while(!(capture = pieceAt(targetSquare)).isWall())
            {
                if(capture.isEmpty())
                {
                    if(obstacle == 1)
                    {
                        // moves.append(Move(sourceSquare, targetSquare));
                        cannonRelOffsets.append(targetSquare - square);
                    }
                }
                else
                {
                    obstacle++;
                    if(obstacle == 1 && capture.type() == Cannon)
                        break;
                    if(obstacle == 2 && capture.side() != side &&
                       capture.type() != Cannon)
                    {
                        // moves.append(Move(sourceSquare, targetSquare));
                        cannonRelOffsets.append(targetSquare - square);
                        break;
                    }
                }
                targetSquare += offset;
            }
        }

        generateHoppingMoves(square, cannonRelOffsets, moves);
    }



}

bool SynoBoard::vIsLegalMove(const Move& move)
{

    return PocketKnightBoard::vIsLegalMove(move);
}
bool SynoBoard::inCheck(Side side, int square) const
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

    //Cannon checks from https://github.com/gaintpd/cutechess
    for (int i = 0; i < m_orthogonalOffsets.size(); i++)
    {
        int offset = m_orthogonalOffsets[i];
        int checkSquare = square + offset;

        int obstacle = 0;
        while (!(piece = pieceAt(checkSquare)).isWall())
        {
            if (!piece.isEmpty())
            {
                obstacle++;
                if (obstacle == 1 && piece.type() == Cannon)
                    break;
                if (obstacle == 2 && piece.side() == opSide &&
                   pieceHasMovement(piece.type(), square, CannonMovement))
                    return true;
            }
            checkSquare += offset;
        }
    }
    // Elephant attacks from shatranj
    for (int i = 0; i < m_elephantOffsets.size(); i++)
    {
        int target = square + m_elephantOffsets[i];
        if (!isValidSquare(chessSquare(target)))
            continue;
        piece = pieceAt(target);
        if (piece.side() == opSide
        &&  pieceHasMovement(piece.type(), square, ElephantMovement))
            return true;
    }

    for (int i = 0; i < m_ferzOffsets.size(); i++)
    {
        piece = pieceAt(square + m_ferzOffsets[i]);
        if (piece.side() == opSide
        &&  pieceHasMovement(piece.type(), square, FerzMovement) | pieceHasMovement(piece.type(), square, ElephantMovement))
            return true;
    }

    for (int i = 0; i < m_wazirOffsets.size(); i++)
    {
        piece = pieceAt(square + m_wazirOffsets[i]);
        if (piece.side() == opSide
        &&  pieceHasMovement(piece.type(), square, WazirMovement))
            return true;
    }

    //Soldier Checks

    for (int i = 0; i < m_soldierOffsets.size(); i++)
    {
        piece = pieceAt(square - m_soldierOffsets[i]);
        if (piece.side() == opSide
        &&  pieceHasMovement(piece.type(), square, SoldierMovement))
            return true;
    }

    return PocketKnightBoard::inCheck(side, square);
}


} // namespace Chess
