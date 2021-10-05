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

#include "torishogiboard.h"

namespace Chess {

ToriShogiBoard::ToriShogiBoard()
    : ShogiBoard()
{
    //Clean all the Shogi Pieces from ShogiBoard()
    setPieceType(Lance, tr("no-lance"), "", 0);
    setPieceType(Knight, tr("no-knight"), "", 0);
    setPieceType(SilverGeneral, tr("no-silver"), "", 0);
    setPieceType(GoldGeneral, tr("no-gold"), "", 0);
    setPieceType(Bishop, tr("no-bishop"), "", 0);
    setPieceType(Rook, tr("no-rook"), "", 0);

    setPieceType(PromotedPawn, tr("no-tokin"), "", 0);
    setPieceType(PromotedLance, tr("no-promoted lance"), "", 0);
    setPieceType(PromotedKnight, tr("no-promoted knight"), "", 0);
    setPieceType(PromotedSilver, tr("no-promoted silver"), "", 0);
    setPieceType(PromotedBishop, tr("no-horse"), "", 0);
    setPieceType(PromotedRook, tr("no-dragon"), "", 0);

    //Re-define pawns and all the other non-shogi pieces
    setPieceType(Pawn, tr("pawn"), "S");
    setPieceGraphics(Pawn, "TORIS");
    setPieceType(Falcon, tr("falcon"), "F", FalconMovement, "TORIF");
    setPieceType(Crane, tr("crane"), "C", CraneMovement, "TORIC");
    setPieceType(LeftQuail, tr("leftquail"), "L", LeftQuailMovement, "TORIL");
    setPieceType(RightQuail, tr("rightquail"), "R", RightQuailMovement, "TORIR");
    setPieceType(Pheasant, tr("pheasant"), "P", PheasantMovement, "TORIP");
    setPieceType(King, tr("king"),"K");
    setPieceGraphics(King, "TORIK");
    setPieceType(Goose, tr("goose"), "+S", GooseMovement, "TORIG");
    setPieceType(Eagle, tr("eagle"), "+F", EagleMovement, "TORIE");
}

inline void rotateAndStoreOffsets(QVarLengthArray<int> a[2])
{
    a[Side::Black].resize(a[Side::White].size());
    for (int i = 0; i < a[Side::White].count(); i++)
        a[Side::Black][i] = -a[Side::White][i];
}

void ToriShogiBoard::vInitialize()
{
    ShogiBoard::vInitialize();
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

    m_lanceOffsets[Side::White].resize(1);
    m_lanceOffsets[Side::White][0] = -arwidth;
    m_lanceOffsets[Side::Black].resize(1);
    m_lanceOffsets[Side::Black][0] = arwidth;

    m_craneOffsets.resize(6);
    m_craneOffsets[0] = -arwidth - 1;
    m_craneOffsets[1] = -arwidth + 1;
    m_craneOffsets[2] = arwidth - 1;
    m_craneOffsets[3] = arwidth + 1;
    m_craneOffsets[4] = arwidth;
    m_craneOffsets[5] = -arwidth;

    m_pheasantOffsets[Side::White].resize(3);
    m_pheasantOffsets[Side::White][0] = arwidth - 1;
    m_pheasantOffsets[Side::White][1] = arwidth + 1;
    m_pheasantOffsets[Side::White][2] = -arwidth*2;

    rotateAndStoreOffsets(m_pheasantOffsets);

    m_rightquailSlideOffsets[Side::White].resize(2);
    m_rightquailSlideOffsets[Side::White][0] = -arwidth;
    m_rightquailSlideOffsets[Side::White][1] = arwidth-1;

    //RIGHT QUAIL SLIDE+HOPPING
    rotateAndStoreOffsets(m_rightquailSlideOffsets);

    m_rightquailHopOffsets[Side::White].resize(1);
    m_rightquailHopOffsets[Side::White][0] = arwidth+1;

    rotateAndStoreOffsets(m_rightquailHopOffsets);

    //LEFT QUAIL SLIDE+HOPPING
    m_leftquailSlideOffsets[Side::White].resize(2);
    m_leftquailSlideOffsets[Side::White][0] = -arwidth;
    m_leftquailSlideOffsets[Side::White][1] = arwidth+1;

    rotateAndStoreOffsets(m_leftquailSlideOffsets);

    m_leftquailHopOffsets[Side::White].resize(1);
    m_leftquailHopOffsets[Side::White][0] = arwidth-1;

    rotateAndStoreOffsets(m_leftquailHopOffsets);

    m_gooseOffsets[Side::White].resize(3);
    m_gooseOffsets[Side::White][0] = -(arwidth - 1) * 2;
    m_gooseOffsets[Side::White][1] = -(arwidth + 1) * 2;
    m_gooseOffsets[Side::White][2] = arwidth*2;

    rotateAndStoreOffsets(m_gooseOffsets);

    m_eagleSlideOffsets[Side::White].resize(3);
    m_eagleSlideOffsets[Side::White][0] = -(arwidth - 1);
    m_eagleSlideOffsets[Side::White][1] = -(arwidth + 1);
    m_eagleSlideOffsets[Side::White][2] = arwidth;

    rotateAndStoreOffsets(m_eagleSlideOffsets);

    m_eagleHopOffsets[Side::White].resize(3);
    m_eagleHopOffsets[Side::White][0] = -arwidth;
    m_eagleHopOffsets[Side::White][1] = 1;
    m_eagleHopOffsets[Side::White][2] = -1;

    rotateAndStoreOffsets(m_eagleHopOffsets);

    m_eagleDiagOffsets[Side::White].resize(2);
    m_eagleDiagOffsets[Side::White][0] = (arwidth - 1);
    m_eagleDiagOffsets[Side::White][1] = (arwidth + 1);

    rotateAndStoreOffsets(m_eagleDiagOffsets);

    m_eagleLongDiagOffsets[Side::White].resize(2);
    m_eagleLongDiagOffsets[Side::White][0] = (arwidth - 1)*2;
    m_eagleLongDiagOffsets[Side::White][1] = (arwidth + 1)*2;

    rotateAndStoreOffsets(m_eagleLongDiagOffsets);


    m_falconOffsets[Side::White].resize(7);
    m_falconOffsets[Side::White][0] = arwidth - 1;
    m_falconOffsets[Side::White][1] = arwidth + 1;
    m_falconOffsets[Side::White][2] = -arwidth;
    m_falconOffsets[Side::White][3] = 1;
    m_falconOffsets[Side::White][4] = -1;
    m_falconOffsets[Side::White][5] = -arwidth - 1;
    m_falconOffsets[Side::White][6] = -arwidth + 1;

    rotateAndStoreOffsets(m_falconOffsets);

}

Board* ToriShogiBoard::copy() const
{
    return new ToriShogiBoard(*this);
}

QString ToriShogiBoard::variant() const
{
    return "torishogi";
}

int ToriShogiBoard::height() const
{
    return 7;
}

int ToriShogiBoard::width() const
{
    return 7;
}

bool ToriShogiBoard::variantHasOptionalPromotions() const
{
    return false;
}

QString ToriShogiBoard::defaultFenString() const
{
    return "rpckcpl/3f3/sssssss/2s1S2/SSSSSSS/3F3/LPCKCPR[-] w 0 1";
}

Result ToriShogiBoard::result()
{
    Side side = sideToMove();
    QString str;

    // 4-fold repetition
    if (repeatCount() >= 3)
    {
        if (inCheck(side))
        {
              str = tr("Illegal perpetual");
              return Result(Result::Win, side, str);
        }
        str = tr("Fourfold repetition - Gote wins");
        return Result(Result::Win, Side::Black, str);
    }
    return ShogiBoard::result();
}

bool ToriShogiBoard::rankIsAllowed(int pieceType, int square) const
{
    if (pieceType != Pawn && pieceType != Lance && pieceType != Knight)
        return true;

    int rank = chessSquare(square).rank();
    int rrank = (sideToMove() == Side::White) ? rank : height() - 1 - rank;
    int distance = height() - rrank;

    return distance > 2 || (distance > 1 && pieceType != Knight);
}


/*
 * Rule of Three Pawns :
 * A Pawn must not be dropped onto a file where two Pawn are already present (Different from Shogi with one pawn)
 */
bool ToriShogiBoard::fileIsAllowed(int pieceType, int square) const
{
    if (pieceType != Pawn)
        return true;

    Side side = sideToMove();
    int arwidth = width() + 2;

    int minIndex = 2 * (width() + 2) + 1;

    int maxIndex = arraySize() - minIndex;

    int start = minIndex - 1 + square % arwidth;

    int num_pawns=0;
    for (int i = start; i < maxIndex; i += arwidth)
    {
        if (pieceAt(i) == Piece(side, Pawn))
            num_pawns++;
    }
    return (!(num_pawns>1));
}

bool ToriShogiBoard::inPromotionZone(int square) const
{
    Square sq = chessSquare(square);
    int rank = sq.rank();
    int rrank = (sideToMove() == Side::White) ? rank : height() - 1 - rank;

    return rrank >= promotionRank();
}

int ToriShogiBoard::promotionRank() const
{
    return height() - height() / 3;
}

int ToriShogiBoard::normalPieceType(int type) const
{
    switch (type)
    {
    case Goose:
        return Pawn;
    case Eagle:
        return Falcon;

    default:
        return type;
    }
}

int ToriShogiBoard::promotedPieceType(int type) const
{
    switch (type)
    {
    case Pawn:
        return Goose;
    case Falcon:
        return Eagle;

    default:
        return type;
    }
}

void ToriShogiBoard::generateMovesForPiece(QVarLengthArray<Move>& moves,
                        int pieceType,
                        int square) const
{

    int minIndex = 2 * (width() + 2) + 1;

    int maxIndex = arraySize() - minIndex;
    // Generate drops
    if (square == 0)
    {
        for (int i = minIndex; i < maxIndex; i++)
        {
            if (!pieceAt(i).isEmpty())
                continue;

            if (rankIsAllowed(pieceType, i)
            &&  fileIsAllowed(pieceType, i))
                moves.append(Move(0, i, pieceType));
        }
        return;
    }

    Side side = sideToMove();
    Side opSide = sideToMove().opposite();

    if (pieceType == King)
    {
        generateHoppingMoves(square, m_bishopOffsets, moves);
        generateHoppingMoves(square, m_rookOffsets, moves);
        return;
    }
    QVarLengthArray<Move>  moves1;

    if (pieceType == Pawn)
        generateHoppingMoves(square, m_lanceOffsets[side], moves1);
    if (pieceHasMovement(pieceType, square, CraneMovement))
        generateHoppingMoves(square, m_craneOffsets, moves1);
    if (pieceHasMovement(pieceType, square, PheasantMovement))
        generateHoppingMoves(square, m_pheasantOffsets[side], moves1);
    if (pieceHasMovement(pieceType, square, RightQuailMovement)) {
        generateSlidingMoves(square, m_rightquailSlideOffsets[side], moves1);
        generateHoppingMoves(square, m_rightquailHopOffsets[side],moves1);
    }
    if (pieceHasMovement(pieceType, square, LeftQuailMovement)) {
        generateSlidingMoves(square, m_leftquailSlideOffsets[side], moves1);
        generateHoppingMoves(square, m_leftquailHopOffsets[side],moves1);
    }
    if (pieceHasMovement(pieceType, square, GooseMovement))
        generateHoppingMoves(square, m_gooseOffsets[side], moves1);
    if (pieceHasMovement(pieceType, square, EagleMovement)) {
        generateSlidingMoves(square, m_eagleSlideOffsets[side], moves1);
        generateHoppingMoves(square, m_eagleHopOffsets[side],moves1);
        generateHoppingMoves(square, m_eagleDiagOffsets[side],moves1);
        for (int i=0; i<m_eagleDiagOffsets[side].size();i++) {
            int sq= square+m_eagleDiagOffsets[side][i];
            if (pieceAt(sq) == Piece::NoPiece) {
                int tar_sq=square+m_eagleLongDiagOffsets[side][i];
                Piece capture = pieceAt(tar_sq);
                if (capture.isEmpty() || capture.side() == opSide)
                    moves1.append(Move(square, tar_sq));
                //moves1.append(Move(square, square+m_eagleLongDiagOffsets[side][i]));
            }
        }
    }
    if (pieceHasMovement(pieceType, square, FalconMovement))
        generateHoppingMoves(square, m_falconOffsets[side], moves1);

    int fromPromotionZone = inPromotionZone(square);
    int promotionType = promotedPieceType(pieceType);
    for (const Move m: moves1)
    {
        // Promotions only for the Swallow and the Falcon
        if (pieceType == promotionType)
        {
            moves.append(m);
            continue;
        }
        int target = m.targetSquare();
        // Promotion is forced for the Swallow and the Falcon
        if (fromPromotionZone || inPromotionZone(target)) {
            moves.append(Move(square, target, promotionType));
        }
        else if (m.promotion() != 0) {
            if (rankIsAllowed(pieceType, target))
                moves.append(m);
        }
        else {
            if (rankIsAllowed(pieceType, target))
                moves.append(m);
        }
    }
}


} // namespace Chess
