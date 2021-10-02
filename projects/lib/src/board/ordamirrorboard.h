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

#ifndef ORDAMIRRORBOARD_H
#define ORDAMIRRORBOARD_H

#include "westernboard.h"

namespace Chess {

/*!
 * \brief A board for Orda Chess
 *
 * Orda Mirror is a chess variant designed in 2020 by Couch Tomato and is a derivative variant of Orda Chess.
 * Orda Mirror takes the Horde army from Orda Chess and pits them against each other, turning into a Horde vs Horde mirror match.
 * However, the Horde armies in Orda Mirror are not completely identical to the one in Orda Chess;
 * the Yurt is replaced with the new Falcon, a piece that moves like a queen, but attacks like a knight (all other Horde pieces move like a knight).
 * This gives some long range flexibility that the Horde was otherwise missing while preserving the horse motif.
 *
 * \note Rules: https://www.pychess.org/variants/ordamirror
 */


class LIB_EXPORT OrdamirrorBoard : public WesternBoard
{
	public:
        /*! Creates a new OrdamirrorBoard object. */
        OrdamirrorBoard();

		// Inherited from WesternBoard
		virtual Board* copy() const;
        virtual QString variant() const;
        virtual Result result();
        virtual bool inCheck(Side side, int square = 0) const;
        virtual void vInitialize();
		virtual QString defaultFenString() const;

	protected:
		// Inherited from WesternBoard

        enum OrdaMirrorPieceType
        {
            Falcon = 7,	//  Falcon = Moves Like a Queen, Captures like a Knight
            Kheshig,    //  Kheshig = King+Knight
            Archer,     // Archer = Moves like a Knight, Captures like a Bishop
            Lancer      // Lancer = Moves like a Knight, Captures like a Rook
        };

        //static const unsigned YurtMovement = 256;
        /*! Movement mask for Ferz move pattern. */
        static const unsigned FerzMovement = 16;
        /*! Movement mask for Wazir move pattern. */
        static const unsigned WazirMovement = 64;

		virtual void generateMovesForPiece(QVarLengthArray< Move >& moves,
						   int pieceType,
						   int square) const;

        virtual void addPromotions(int sourceSquare,
                         int targetSquare,
                         QVarLengthArray<Move>& moves) const;

private:

    int m_arwidth;

    QVarLengthArray<int> m_bishopOffsets;
    QVarLengthArray<int> m_rookOffsets;
    QVarLengthArray<int> m_knightOffsets;
    QVarLengthArray<int> m_ferzOffsets;
    QVarLengthArray<int> m_wazirOffsets;
    QVarLengthArray<int> m_YurtOffsets[2];

    bool kingAtEnd(Side side) const;
    const QList<int> m_BackRank;


};


} // namespace Chess
#endif // ORDAMIRRORBOARD_H
