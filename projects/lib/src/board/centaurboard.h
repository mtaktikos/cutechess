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

#ifndef CENTAURBOARD_H
#define CENTAURBOARD_H

#include "westernboard.h"

namespace Chess {

/*!
 * \brief A board for Centaur chess or Royal Court
 *
 * The board comprises 80 squares, 8 ranks and 10 files, colored alternately light and dark.
 * The players, designated white and black, sit on opposite sides.
 * The board is placed so that there is a light square in the left hand corner of each player.
 *
 * \note Rules: https://www.chessvariants.com/large.dir/contest/royalcourt.html
 */
class LIB_EXPORT CentaurBoard : public WesternBoard
{
	public:
        /*! Creates a new CentaurBoard object. */
        CentaurBoard();

		// Inherited from WesternBoard
		virtual Board* copy() const;
		virtual QString variant() const;
		virtual int width() const;
		virtual QString defaultFenString() const;
        virtual void generateMovesForPiece(QVarLengthArray< Move >& moves,
                                  int pieceType,
                                  int square) const;
        virtual bool inCheck(Side side, int square) const;
        virtual void vInitialize();

	protected:
        /*! Special piece types for Centaur variants. */
        enum CentaurPieceType
		{
            Centaur = King+1 // Centaur Piece (KNIGHT+KING)
		};
        static const unsigned WazirMovement    = 16;
        static const unsigned FerzMovement     = 64;

		// Inherited from WesternBoard
		virtual void addPromotions(int sourceSquare,
					   int targetSquare,
					   QVarLengthArray<Move>& moves) const;
    private:

        QVarLengthArray<int> m_ferzOffsets;
        QVarLengthArray<int> m_wazirOffsets;
};

} // namespace Chess
#endif // CENTAURBOARD_H
