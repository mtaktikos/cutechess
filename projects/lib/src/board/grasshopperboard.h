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

#ifndef GRASSHOPPERBOARD_H
#define GRASSHOPPERBOARD_H

#include "westernboard.h"

namespace Chess {

/*!
 * \brief A board for Grasshopper Chess
 *
 * Grasshopper chess is a chess variant, in which the pawns are allowed to promote to a fairy piece, the grasshopper.
 * The grasshopper must hop over other pieces in order to move or capture.
 *
 * A piece that is very popular with inventors of fairy chess problems is the grasshopper.
 * This piece was invented in 1912 by the well known fairy chess problem composer, T.R. Dawson.
 * The piece was inspired by the cannon from Chinese chess.
 * In the 1950s, J. Boyer from France, who invented many chess variants, invented a chess variant, where grasshoppers play an important role.
 *
 * \note Rules: https://en.wikipedia.org/wiki/Grasshopper_chess
 *
 */
class LIB_EXPORT GrasshopperBoard : public WesternBoard
{
	public:
        /*! Creates a new GrasshopperBoard object. */
        GrasshopperBoard();

		// Inherited from WesternBoard
		virtual Board* copy() const;
        virtual QString variant() const;
        virtual QString defaultFenString() const;
        virtual void vInitialize();
        virtual bool inCheck(Side side, int square) const;
        virtual void generateGrasshopperMoves(QVarLengthArray< Move >& moves,int square) const;
        virtual void addPromotions(int sourceSquare, int targetSquare, QVarLengthArray<Move>& moves) const;
	protected:
        enum GrasshopperPieceType
		{
            Grasshopper = King+1 //! Grasshopper
		};
        static const unsigned GrasshopperMovement = 16;

        virtual void generateMovesForPiece(QVarLengthArray< Move >& moves,
                           int pieceType,
                           int square) const;
    private:
        QVarLengthArray<int> m_bishopOffsets;
        QVarLengthArray<int> m_rookOffsets;

};

} // namespace Chess
#endif // GRASSHOPPERBOARD_H
