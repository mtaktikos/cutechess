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

#ifndef ORDABOARD_H
#define ORDABOARD_H

#include "westernboard.h"

namespace Chess {

/*!
 * \brief A board for Orda Chess
 *
 * Orda Chess is a chess variant designed in 2020 by Couch Tomato.
 * The idea of the game was to create a true asymmetric chess with two different armies.
 * Ralph Betza’s Chess with Different Armies was an inspiration, but the goal was to be a little more streamlined with the theme here.
 * In this case, the theme of the new army is knight-based movement, where most pieces have an element of knight movement. Given the knight (or horse) theme,
 * this was modeled after the Mongol army and named the Horde. In fact, an orda was a military structure for the people of the Steppes, which also gave rise to the English word “horde.”
 * The original chess army is named the Kingdom for contrast. The game itself is incredibly balanced by engine evaluation (even more than standard chess),
 * with a near 50-50 win ratio for the Kingdom and Horde.
 *
 * \note Rules: https://www.pychess.org/variant/orda
 */


class LIB_EXPORT OrdaBoard : public WesternBoard
{
	public:
        /*! Creates a new OrdaBoard object. */
        OrdaBoard();

		// Inherited from WesternBoard
		virtual Board* copy() const;
        virtual QString variant() const;
        virtual Result result();
        virtual bool inCheck(Side side, int square = 0) const;
        virtual void vInitialize();
		virtual QString defaultFenString() const;

	protected:
		// Inherited from WesternBoard

        enum OrdaPieceType
        {
            Yurt = 7,	//  Yurt = Silver General from Shogi
            Kheshig,    //  Kheshig = King+Knight
            Archer,     // Archer = Moves like a Knight, Captures like a Bishop
            Lancer      // Lancer = Moves like a Knight, Captures like a Rook
        };

        static const unsigned YurtMovement = 256;
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
#endif // ORDABOARD_H
