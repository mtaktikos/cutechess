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

#ifndef OKISAKISHOGIBOARD_H
#define OKISAKISHOGIBOARD_H

#include "shogiboard.h"

namespace Chess {

/*!
 * \brief A board for OkisakiShogi (御妃将棋 okisaki shōgi)
 *
 * Okisaki shogi (御妃将棋 okisaki shōgi "Queen shogi") is a modern variant of shogi (Japanese chess).
 * It was developed by Masayuki Nakayachi c. 1996 from suggestions by German chess player Ralph Blockhaus.
 *
 * Okisaki shogi is similar to standard shogi, but is played on a 10×10 board.
 * There are ten pawns and a queen, which moves like a queen in Western chess. The knights also move as in Western chess.
 *
 * \note Rules: https://en.wikipedia.org/wiki/Okisaki_shogi
 *
 * \sa ShogiBoard
 */
class LIB_EXPORT OkisakiShogiBoard : public ShogiBoard
{
	public:
        /*! Creates a new OkisakiShogiBoard object. */
        OkisakiShogiBoard();

		// Inherited from ShogiBoard
		virtual Board* copy() const;
		virtual QString variant() const;
		virtual int height() const;
		virtual int width() const;
		virtual QString defaultFenString() const;
		virtual Result result();
        virtual void vInitialize();
        virtual void generateMovesForPiece(QVarLengthArray<Move>& moves,
                                                              int pieceType,
                                                              int square) const;

	protected:
        enum ShogiPieceType
        {
            VerticalRook = PromotedRook+1,
            Horse,
            Queen,
            PromotedVerticalRook,
            PromotedHorse
        };

        virtual int normalPieceType(int type) const;
        virtual int promotedPieceType(int type) const;
        virtual bool inPromotionZone(int square) const;
        virtual int promotionRank() const;


        /*! Movement mask for Horse moves. */
        static const unsigned HorseMovement = 1024;
        /*! Movement mask for Vertical Rook moves. */
        static const unsigned VerticalRookMovement = 2048;

    private:
        QVarLengthArray<int> m_verticalRookOffsets;
        QVarLengthArray<int> m_horseOffsets;
};

} // namespace Chess
#endif // OKISAKISHOGIBOARD_H
