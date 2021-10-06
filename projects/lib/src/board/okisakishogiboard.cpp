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

#include "okisakishogiboard.h"

namespace Chess {

OkisakiShogiBoard::OkisakiShogiBoard()
	: ShogiBoard()
{
	setPieceType(Lance, tr("no-lance"), "", 0);
	setPieceType(Knight, tr("no-knight"), "", 0);
	setPieceType(PromotedLance, tr("no-promoted-lance"), "", 0);
	setPieceType(PromotedKnight, tr("no-promoted-knight"), "", 0);

    setPieceType(VerticalRook, tr("vertical-rook"), "L", VerticalRookMovement, "SHOGIL");
    setPieceType(Horse, tr("knight"), "N", HorseMovement, "SHOGIN");
    setPieceType(Queen, tr("queen"), "Q", BishopMovement | RookMovement, "SHOGIQ");

    setPieceType(PromotedVerticalRook, tr("promoted-vertical-rook"), "+L", GoldMovement, "SHOGIPL");
    setPieceType(PromotedHorse, tr("promoted-horse"), "+N", GoldMovement, "SHOGIPN");
}

Board* OkisakiShogiBoard::copy() const
{
    return new OkisakiShogiBoard(*this);
}

void OkisakiShogiBoard::vInitialize()
{
    ShogiBoard::vInitialize();
    int arwidth = width() + 2;

    m_verticalRookOffsets.resize(2);
    m_verticalRookOffsets[0] = -arwidth;
    m_verticalRookOffsets[1] = arwidth;

    m_horseOffsets.resize(8);
    m_horseOffsets[0] = -2 * arwidth - 1;
    m_horseOffsets[1] = -2 * arwidth + 1;
    m_horseOffsets[2] = -arwidth - 2;
    m_horseOffsets[3] = -arwidth + 2;
    m_horseOffsets[4] = arwidth - 2;
    m_horseOffsets[5] = arwidth + 2;
    m_horseOffsets[6] = 2 * arwidth - 1;
    m_horseOffsets[7] = 2 * arwidth + 1;

}

int OkisakiShogiBoard::promotionRank() const
{
    // Promotion zone is opponent's third.
    return height() - height() / 3;
}


int OkisakiShogiBoard::normalPieceType(int type) const
{
    switch (type)
    {
    case PromotedPawn:
        return Pawn;
    case PromotedVerticalRook:
        return VerticalRook;
    case PromotedHorse:
        return Horse;
    case PromotedSilver:
        return SilverGeneral;
    case PromotedBishop:
        return Bishop;
    case PromotedRook:
        return Rook;

    default:
        return type;
    }
}

int OkisakiShogiBoard::promotedPieceType(int type) const
{
    switch (type)
    {
    case Pawn:
        return PromotedPawn;
    case VerticalRook:
        return PromotedVerticalRook;
    case Horse:
        return PromotedHorse;
    case SilverGeneral:
        return PromotedSilver;
    case Bishop:
        return PromotedBishop;
    case Rook:
        return PromotedRook;

    default:
        return type;
    }
}

QString OkisakiShogiBoard::variant() const
{
    return "okisakishogi";
}

int OkisakiShogiBoard::height() const
{
    return 10;
}

int OkisakiShogiBoard::width() const
{
    return 10;
}

QString OkisakiShogiBoard::defaultFenString() const
{
    return "lnsgkqgsnl/1r6b1/pppppppppp/10/10/10/10/PPPPPPPPPP/1B6R1/LNSGQKGSNL[-] w 0 1";
}

Result OkisakiShogiBoard::result()
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

bool OkisakiShogiBoard::inPromotionZone(int square) const
{
    Square sq = chessSquare(square);
    int rank = sq.rank();
    int rrank = (sideToMove() == Side::White) ? rank : height() - 1 - rank;

    return rrank >= promotionRank();
}

void OkisakiShogiBoard::generateMovesForPiece(QVarLengthArray<Move>& moves,
                        int pieceType,
                        int square) const
{
    QVarLengthArray<Move>  moves1;
    if (pieceHasMovement(pieceType, square, VerticalRookMovement))
        generateSlidingMoves(square, m_verticalRookOffsets, moves1);
    if (pieceHasMovement(pieceType, square, HorseMovement))
        generateHoppingMoves(square, m_horseOffsets, moves1);
    if (pieceType == VerticalRook || pieceType == Horse) {

        int minIndex = 2 * (width() + 2) + 1;

        int maxIndex = arraySize() - minIndex;

        if (square == 0)
        {
            for (int i = minIndex; i < maxIndex; i++)
            {
                if (!pieceAt(i).isEmpty())
                    continue;
                moves.append(Move(0, i, pieceType));
            }
            return;
        }

        int fromPromotionZone = inPromotionZone(square);
        int promotionType = promotedPieceType(pieceType);
        for (const Move m: moves1)
        {
            // No promotions for King, Gold and promoted piece types
            if (pieceType == promotionType)
            {
                moves.append(m);
                continue;
            }

            /*
             * A normal move is allowed if the piece is not a Pawn or a Lance
             * on the highest rank or a Knight on the highest two ranks.
             */
            int target = m.targetSquare();

            moves.append(m);

            // Promotion is allowed in opponent's third of the board.
            if (fromPromotionZone || inPromotionZone(target))
                moves.append(Move(square, target, promotionType));
        }
    }
    else
        ShogiBoard::generateMovesForPiece(moves,pieceType,square);

}

} // namespace Chess
