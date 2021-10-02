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

#include "shinobiboard.h"
#include "pocketknightboard.h"

namespace Chess {

ShinobiBoard::ShinobiBoard()
    : PocketKnightBoard(),
    m_promotionRank(6)
{
    setPieceGraphics(King, "SHK");
    setPieceGraphics(Bishop,"SHB");
    setPieceGraphics(Pawn, "SHP");
    setPieceGraphics(Knight, "SHN");
    setPieceGraphics(Rook, "SHR");
    setPieceType(Captain, tr("captain"),"C",FerzMovement | WazirMovement, "SHC");
    setPieceType(Ninja, tr("ninja"), "J", KnightMovement | BishopMovement, "SHJ");
    setPieceType(Samurai, tr("samurai"), "S", KnightMovement | RookMovement, "SHS");
    setPieceType(Dragon, tr("dragon"), "D", RookMovement | FerzMovement, "SHD");
    setPieceType(Monk, tr("monk"), "M", FerzMovement, "SHM");
    setPieceType(WoodenHorse, tr("woodenhorse"), "H", WoodenKnightMovement, "SHH");
    setPieceType(Lance, tr("lance"), "L", LanceMovement, "SHL");
}

int ShinobiBoard::promotionRank() const
{
    // Promotion zone is opponent's third. Taken from shogi
    return height() - 3;

}
Board* ShinobiBoard::copy() const
{
    return new ShinobiBoard(*this);
}

QString ShinobiBoard::variant() const
{
    return "shinobi";
}


bool ShinobiBoard::kingAtEnd(Side side) const
{
    Square ksq = chessSquare(kingSquare(side));
    return (side==Side::Black && ksq.rank()==0)|((side==Side::White)&&ksq.rank()==7);
    //Check if either king has reached the opposite rank
}


QList< Piece > ShinobiBoard::reservePieceTypes() const
{
    QList<Piece> list;

    list << Piece(Side::White, Pawn);
    list << Piece(Side::White, Captain);
    list << Piece(Side::White, Ninja);
    list << Piece(Side::White, Samurai);
    list << Piece(Side::White, Dragon);
    list << Piece(Side::White, Monk);
    list << Piece(Side::White, WoodenHorse);
    list << Piece(Side::White, Lance);

    return list;
}

QString ShinobiBoard::defaultFenString() const
{
    return "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/LH1CK1HL[LHMMDJ] w kq - 0 1";
}

Result ShinobiBoard::result()
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

    // 3-fold repetition To be fixed for Shinobi


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

void ShinobiBoard::addPromotions(int,
                 int,
                 QVarLengthArray<Move>&) const
{
        //Delete all the default promotions
}

void ShinobiBoard::vInitialize()
{
    PocketKnightBoard::vInitialize();
    int arwidth = width() + 2;

    m_promotionRank = 6; //Promotion Rank on the 7th

    m_lanceOffsets[Side::White].resize(1);
    m_lanceOffsets[Side::White][0] = -arwidth;
    m_lanceOffsets[Side::Black].resize(1);
    m_lanceOffsets[Side::Black][0] = arwidth;
    //Taken from Shogi

    m_woodenKnightOffsets[Side::White].resize(2);
    m_woodenKnightOffsets[Side::White][0] = -2 * arwidth - 1;
    m_woodenKnightOffsets[Side::White][1] = -2 * arwidth + 1;
    //Taken from Shogi's Knight

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

    m_orthogonalOffsets.resize(4);
    m_orthogonalOffsets[0] = -arwidth;
    m_orthogonalOffsets[1] = -1;
    m_orthogonalOffsets[2] =  1;
    m_orthogonalOffsets[3] =  arwidth;

}


bool ShinobiBoard::inPromotionZone(int square) const
{
    Square sq = chessSquare(square);
    int rank = sq.rank();
    int rrank = (sideToMove() == Side::White) ? rank : height() - 1 - rank;

    return rrank >= m_promotionRank;
}

int ShinobiBoard::promotedPieceType(int type) const
{
    switch (type)
    {
    case Pawn:
        return Captain;
    case Lance:
        return Rook;
    case WoodenHorse:
        return Knight;
    case Monk:
        return Bishop;
    default:
        return type;
    }
}
int ShinobiBoard::reverseProm(int type) const
{
    switch (type)
    {
    case Captain:
        return Pawn;
    case Rook:
        return Lance;
    case Knight:
        return WoodenHorse;
    case Bishop:
        return Monk;
    default:
        return type;
    }
}

bool ShinobiBoard::rankIsAllowed(int pieceType, int square) const
{
    if (pieceType != Pawn && pieceType != Lance && pieceType != WoodenHorse)
        return true;

    int rank = chessSquare(square).rank();
    int rrank = (sideToMove() == Side::White) ? rank : height() - 1 - rank;
    int distance = height() - rrank;

    return distance > 2 || (distance > 1 && pieceType != WoodenHorse);
}

void ShinobiBoard::generatePawnMoves(int square,
                    QVarLengthArray< Move >& moves) const
{
    // Generate moves for pawn
    QVarLengthArray< Move > moves1;
    PocketKnightBoard::generateMovesForPiece(moves1, Pawn, square);

    Side side = sideToMove();
    int arwidth = width() + 2;

    // Promote pawn to Captain when reaching 7th rank
    for (const Move& m: moves1)
    {
        int rank = height() + 1 - m.targetSquare() / arwidth;
        int rrank = (side == Side::White) ? rank : height() - 1 - rank;
        if (rrank < m_promotionRank)
            moves.append(m);
        else if (m.promotion() != 0)
            moves.append(m);
        else {
            moves.append(Move(square, m.targetSquare(), Captain));
            addPromotions(square, m.targetSquare(), moves); }
    }
}


QString ShinobiBoard::lanMoveString(const Move& move)
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


Move ShinobiBoard::moveFromLanString(const QString& istr)
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
        return PocketKnightBoard::moveFromLanString(str);

    if (hasCastlingRight(side, KingSide)
    &&  target == kingSquare(side) + castlingFile(KingSide) - kingFile)
        return PocketKnightBoard::moveFromLanString(str);

    Move move = PocketKnightBoard::moveFromLanString(str);
    bool isPromotion = (str.endsWith("+"));
    if (!isPromotion) { return move; }
    int source = move.sourceSquare();
    Piece piece = pieceAt(source);
    int type = piece.type();
    int promotion =  promotedPieceType(type);
    return Move(move.sourceSquare(), move.targetSquare(), promotion);
}


void ShinobiBoard::generateMonkMoves(int square, QVarLengthArray< Move >& moves) const
{
    // Generate moves for Monk
    QVarLengthArray< Move > moves1;
    generateHoppingMoves(square, m_ferzOffsets, moves1);
    Side side = sideToMove();
    int arwidth = width() + 2;
    // Promote to Rook at the 7th rank
    for (const Move& m: moves1)
    {
        int rank = height() + 1 - m.targetSquare() / arwidth;
        int rrank = (side == Side::White) ? rank : height() - 1 - rank;

        if (rrank < m_promotionRank)
            moves.append(m);
        else if (m.promotion() != 0)
            moves.append(m);
        else if (rrank >= m_promotionRank) {
            moves.append(Move(square, m.targetSquare(), Bishop));
            //addPromotions(square, m.targetSquare(), moves);
        }
    }
}

void ShinobiBoard::generateWoodenHorseMoves(int square, QVarLengthArray< Move >& moves) const
{
    // Generate moves for Wooden Horse
    QVarLengthArray< Move > moves1;
    generateHoppingMoves(square, m_woodenKnightOffsets[sideToMove()], moves1);
    Side side = sideToMove();
    int arwidth = width() + 2;
    // Promote pawn to Knight when reaching 7th rank
    for (const Move& m: moves1)
    {
        int rank = height() + 1 - m.targetSquare() / arwidth;
        int rrank = (side == Side::White) ? rank : height() - 1 - rank;

        if (rrank < m_promotionRank)
            moves.append(m);
        else if (m.promotion() != 0)
            moves.append(m);
        else if (rrank >= m_promotionRank) {
            moves.append(Move(square, m.targetSquare(), Knight));
            //addPromotions(square, m.targetSquare(), moves);
        }
    }
}

void ShinobiBoard::generateLanceMoves(int square, QVarLengthArray< Move >& moves) const
{
    // Generate moves for Lance
    QVarLengthArray< Move > moves1;
    generateSlidingMoves(square, m_lanceOffsets[sideToMove()], moves1);
    Side side = sideToMove();
    int arwidth = width() + 2;
    // Promote to Rook at the 6th rank
    for (const Move& m: moves1)
    {
        int rank = height() + 1 - m.targetSquare() / arwidth;
        int rrank = (side == Side::White) ? rank : height() - 1 - rank;

        if (rrank < m_promotionRank)
            moves.append(m);
        else if (m.promotion() != 0)
            moves.append(m);
        else if (rrank >= m_promotionRank) {
            moves.append(Move(square, m.targetSquare(), Rook));
            //addPromotions(square, m.targetSquare(), moves);
        }
    }
}

void ShinobiBoard::vUndoMove(const Move& move)
{
    Side side = sideToMove();
    int source = move.sourceSquare();
    int prom = move.promotion();
    PocketKnightBoard::vUndoMove(move);
    // Revert Promotion with function
    if (prom != Piece::NoPiece)
    {
        int normalType = reverseProm(prom);
        if (source != 0)
            setSquare(source, Piece(side, normalType));
    }

}

void ShinobiBoard::vMakeMove(const Move& move, BoardTransition* transition)
{
    Side side = sideToMove();
    int target = move.targetSquare();
    int prom = move.promotion();

    PocketKnightBoard::vMakeMove(move,transition);

    //Set new Promoted Piece
    if (prom != Piece::NoPiece)
        setSquare(target, Piece(side, prom));
}

void ShinobiBoard::generateMovesForPiece(QVarLengthArray< Move >& moves,
					      int pieceType,
					      int square) const
{

    if (square == 0)

    {
        const int size = arraySize();
        for (int i = 61; i < size; i++) //promotions allowed from 1st to 4th rank
        {
            Piece tmp = pieceAt(i);
            if (!tmp.isEmpty())
                continue;
            moves.append(Move(0, i, pieceType));
        }
    }
    else {
        if (pieceType == Captain) {
            generateHoppingMoves(square, m_ferzOffsets, moves);
            generateHoppingMoves(square, m_wazirOffsets, moves);
        }
        if (pieceType == Dragon) {
            generateHoppingMoves(square, m_ferzOffsets, moves);
            generateMovesForPiece(moves, Rook, square);
        }
        if (pieceType == Samurai) {
            generateMovesForPiece(moves, Bishop, square);
            generateMovesForPiece(moves, Rook, square);
        }
        if (pieceType != Pawn && pieceType != WoodenHorse && pieceType != Monk && pieceType != Lance && pieceType != Dragon && pieceType != Samurai)
        {
            WesternBoard::generateMovesForPiece(moves, pieceType, square);
            return;
        }
        if (pieceHasMovement(pieceType, square, WoodenKnightMovement)) {
            generateWoodenHorseMoves(square, moves);
        }
        if (pieceHasMovement(pieceType, square, LanceMovement)) {
            generateLanceMoves(square, moves);
        }
        if (pieceType == Monk) {
            generateMonkMoves(square, moves);
        }
        if (pieceType == Pawn)
            generatePawnMoves(square, moves);

    }
}

bool ShinobiBoard::inCheck(Side side, int square) const
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



    return PocketKnightBoard::inCheck(side, square);
}


} // namespace Chess
