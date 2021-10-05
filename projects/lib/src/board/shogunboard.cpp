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

#include "shogunboard.h"
#include "crazyhouseboard.h"

namespace Chess {

ShogunBoard::ShogunBoard()
    : CrazyhouseBoard(),
    m_promotionRank(6)
{
    setPieceType(Captain, tr("captain"),"+P",FerzMovement | WazirMovement, "M");
    setPieceType(Mortar, tr("mortar"), "+R", RookMovement | KnightMovement, "SHOM");
    setPieceType(Archbishop, tr("archbishop"), "+B", BishopMovement | KnightMovement, "SHOA");
    setPieceType(General, tr("general"), "+N", KnightMovement | FerzMovement | WazirMovement, "SHOG");
    setPieceType(Duchess, tr("duchess"), "F", FerzMovement, "SHOF");
    setPieceType(Queen, tr("queen"), "+F", RookMovement | BishopMovement, "Q");
}

int ShogunBoard::promotionRank() const
{
    // Promotion zone is opponent's third. Taken from shogi
    return height() - 3;

}
Board* ShogunBoard::copy() const
{
    return new ShogunBoard(*this);
}

QString ShogunBoard::variant() const
{
    return "shogun";
}


QList< Piece > ShogunBoard::reservePieceTypes() const
{
    QList<Piece> list;

    list << Piece(Side::White, Pawn);
    list << Piece(Side::White, Knight);
    list << Piece(Side::White, Bishop);
    list << Piece(Side::White, Rook);
    list << Piece(Side::White, Duchess);

    return list;
}

QString ShogunBoard::defaultFenString() const
{
    return "rnb+fkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNB+FKBNR[] w KQkq - 0 1";
}

int ShogunBoard::reserveType(int pieceType) const
{
    return reverseProm(pieceType);
}

Result ShogunBoard::result()
{
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

    // 3-fold repetition To be fixed for Shogun


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

void ShogunBoard::addPromotions(int sourceSquare,
                 int targetSquare,
                 QVarLengthArray<Move>& moves) const
{
    moves.append(Move(sourceSquare, targetSquare, Captain));
}

void ShogunBoard::vInitialize()
{
    CrazyhouseBoard::vInitialize();
    int arwidth = width() + 2;

    m_promotionRank = 5; //Promotion Rank on the 7th

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


bool ShogunBoard::inPromotionZone(int square) const
{
    Square sq = chessSquare(square);
    int rank = sq.rank();
    int rrank = (sideToMove() == Side::White) ? rank : height() - 1 - rank;

    return rrank >= m_promotionRank;
}

int ShogunBoard::promotedPieceType(int type) const
{
    switch (type)
    {
    case Pawn:
        return Captain;
    case Duchess:
        return Queen;
    case Rook:
        return Mortar;
    case Bishop:
        return Archbishop;
    case Knight:
        return General;
    default:
        return type;
    }
}
int ShogunBoard::reverseProm(int type) const
{
    switch (type)
    {
    case Captain:
        return Pawn;
    case Queen:
        return Duchess;
    case General:
        return Knight;
    case Archbishop:
        return Bishop;
    case Mortar:
        return Rook;
    default:
        return type;
    }
}


QString ShogunBoard::lanMoveString(const Move& move)
{

    int source = move.sourceSquare();
    int target = move.targetSquare();
    int prom = move.promotion();

    if (move.sourceSquare() == kingSquare(sideToMove())) {
            return WesternBoard::lanMoveString(move); //Needed to fix the castling bug
    }

    QString str;
    // Piece drop
    if (source == 0)
    {
        Q_ASSERT(prom != Piece::NoPiece);
        QChar drop = true ? '@' : '*';
        str += pieceSymbol(prom).toUpper() + drop;
        str += squareString(target);
        return str;
    }

    str = squareString(source);
    str += squareString(target);
    if (prom != Piece::NoPiece)
        str += "+";
    return str;
}


Move ShogunBoard::moveFromLanString(const QString& istr)
{

    QString str(istr);
    Side side = sideToMove();
    QString kingSymbol(pieceSymbol(King).toUpper());
    int ksymlen = kingSymbol.length();
    int len = str.length();
    int target = squareIndex(str.mid(ksymlen, len - ksymlen));
    int kingFile = chessSquare(kingSquare(side)).file();

    //conditions for castling
    if (hasCastlingRight(side, QueenSide)
    &&  target == kingSquare(side) + castlingFile(QueenSide) - kingFile)
        return CrazyhouseBoard::moveFromLanString(str);

    if (hasCastlingRight(side, KingSide)
    &&  target == kingSquare(side) + castlingFile(KingSide) - kingFile)
        return CrazyhouseBoard::moveFromLanString(str);

    Move move = CrazyhouseBoard::moveFromLanString(str);
    bool isPromotion = (str.endsWith("+"));
    if (!isPromotion) { return move; }
    int source = move.sourceSquare();
    Piece piece = pieceAt(source);
    int type = piece.type();
    int promotion =  promotedPieceType(type);
    return Move(move.sourceSquare(), move.targetSquare(), promotion);
}



void ShogunBoard::vUndoMove(const Move& move)
{
    Side side = sideToMove();
    int source = move.sourceSquare();
    int prom = move.promotion();
    CrazyhouseBoard::vUndoMove(move);
    // Revert Promotion with function
    if (prom != Piece::NoPiece)
    {
        int normalType = reverseProm(prom);
        if (source != 0)
            setSquare(source, Piece(side, normalType));
    }

}

void ShogunBoard::vMakeMove(const Move& move, BoardTransition* transition)
{
    Side side = sideToMove();
    int target = move.targetSquare();
    int prom = move.promotion();

    CrazyhouseBoard::vMakeMove(move,transition);

    //Set new Promoted Piece
    if (prom != Piece::NoPiece)
        setSquare(target, Piece(side, prom));
}


void ShogunBoard::generateMovesForPiece(QVarLengthArray< Move >& moves,
					      int pieceType,
					      int square) const
{

    QVarLengthArray<Move>  moves1;

    /*
     * 	if (square == 0)
    {
        const int size = arraySize();
        for (int i = 0; i < size; i++)
        {
            Piece tmp = pieceAt(i);
            if (!tmp.isEmpty())
                continue;
            if (pieceType == Pawn)
            {
                Square sq(chessSquare(i));
                if (!pawnDropOkOnRank(sq.rank()))
                    continue;
            }
            moves.append(Move(0, i, pieceType));
        }
    }*/
    Side side = sideToMove();
    if (square == 0)
    {
            const int size = arraySize();
            for (int i=0; i < size; i++) //drops allowed from 1st to 5th rank
            {
                Piece tmp = pieceAt(i);
                if (!tmp.isEmpty())
                    continue;
                Square sq = chessSquare(i);
                if  ((side == side.White && sq.rank() > height() - 4) || (side == side.Black && sq.rank() < height() - 5))
                    continue;
                moves.append(Move(0, i, pieceType));
            }
    }
    else {
        if (pieceType == King)
        {
            CrazyhouseBoard::generateMovesForPiece(moves1,King,square);
        }
        if (pieceHasMovement(pieceType, square, FerzMovement))
            generateHoppingMoves(square,m_ferzOffsets, moves1);
        if (pieceHasMovement(pieceType, square, WazirMovement))
            generateHoppingMoves(square, m_wazirOffsets, moves1);
        if ((pieceHasMovement(pieceType, square, BishopMovement)) || (pieceHasMovement(pieceType, square, RookMovement)) || (pieceHasMovement(pieceType, square, KnightMovement)))
            CrazyhouseBoard::generateMovesForPiece(moves1, pieceType, square);
        if (pieceType == Pawn)
            CrazyhouseBoard::generateMovesForPiece(moves1,Pawn,square);

        int fromPromotionZone = inPromotionZone(square);
        int promotionType = promotedPieceType(pieceType);
        for (const Move m: moves1)
        {
            if (pieceType == promotionType)
            {
                moves.append(m);
                continue;
            }

            int target = m.targetSquare();

            moves.append(m);

            if (rankIsAllowed(pieceType,square) && target != enpassantSquare())
                if ((fromPromotionZone || inPromotionZone(target)) && !(pieceOnBoard(promotionType, sideToMove())))
                    moves.append(Move(square, target, promotionType));
        }
    }
}

bool ShogunBoard::pieceIsPromotion(int typePiece) const {
        return (typePiece==Archbishop || typePiece==Mortar || typePiece==Queen || typePiece==General);

}


bool ShogunBoard::pieceOnBoard(Piece p, Side side) const {
    if (pieceIsPromotion(p.type())) {
        for (int i = 0; i < arraySize(); i++)
        {
            Piece piece = pieceAt(i);
            if (side == piece.side() && p.type() == piece.type())
            {
                return true;
            }
        }
      return false;
    }
    return false;
}

bool ShogunBoard::rankIsAllowed(int pieceType, int square) const
{
    if (pieceType != Pawn)
        return true;

    int rank = chessSquare(square).rank();
    int rrank = (sideToMove() == Side::White) ? rank : height() - 1 - rank;
    int distance = height() - rrank;

    return distance > 2;
}

bool ShogunBoard::inCheck(Side side, int square) const
{
    //Taken from Knightmate
    if (square == 0)
        square = kingSquare(side);

    QVarLengthArray <Move> moves;
    if (sideToMove() == side)
        for (int type = Pawn; type <= Captain; type++)
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
