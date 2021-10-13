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

#ifndef BREAKTHROUGHBOARD_H
#define BREAKTHROUGHBOARD_H

#include "standardboard.h"

namespace Chess {

/*!
 * \brief Breakthrough
 * Breakthrough is an abstract strategy board game invented by Dan Troyka in 2000 and made available as a Zillions of Games file (ZRF).
 * It won the 2001 8x8 Game Design Competition, even though the game was originally played on a 7x7 board, as it is trivially extensible to larger board sizes.
 *
 * \rules https://en.wikipedia.org/wiki/Breakthrough_(board_game)
 */
class LIB_EXPORT BreakthroughBoard : public StandardBoard
{
	public:
        /*! Creates a new BreakthroughBoard object. */
        BreakthroughBoard();

		virtual Board* copy() const;
		virtual QString variant() const;
		virtual QString defaultFenString() const;
		virtual Result result();
        virtual void vInitialize();
        virtual int reversibleMoveCount() const;

	protected:
		// Inherited from StandardBoard
		virtual bool hasCastling() const;
		virtual bool kingsCountAssertion(int whiteKings,
						 int blackKings) const;
		virtual bool inCheck(Side side, int square = 0) const;
		virtual void addPromotions(int sourceSquare,
					   int targetSquare,
					   QVarLengthArray<Move>& moves) const;
        virtual void generateMovesForPiece(QVarLengthArray<Move>& moves,
                                int pieceType,
                                int square) const;
        virtual bool pawnAtEnd(Side side);

		/*! Rules stalemate outcome. */
		virtual Result vResultOfStalemate() const;

	private:
        QVarLengthArray<int> m_ferzOffsets[2];
        QVarLengthArray<int> m_pawnOffsets[2];

}; // namespace Chess

}
#endif // BREAKTHROUGHBOARD_H
