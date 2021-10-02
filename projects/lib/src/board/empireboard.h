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

#ifndef EMPIREBOARD_H
#define EMPIREBOARD_H

#include "westernboard.h"

namespace Chess {

/*!
    Empire Chess is a chess variant designed in 2020 by Couch Tomato,
    the third asymmetric game. This game pits the army of the "Empire" (gold) against the original chess army (the "Kingdom", black).
    The Empire's unique feature is that its pieces move like queens but attack differently.

    By: Couch Tomato
    https://vchess.club/#/variants/Empire
    https://www.pychess.org/variants/empire
 */


class LIB_EXPORT EmpireBoard : public WesternBoard
{
	public:
        /*! Creates a new EmpireBoard object. */
        EmpireBoard();

		// Inherited from WesternBoard
		virtual Board* copy() const;
        virtual QString variant() const;
        virtual Result result();
        virtual bool inCheck(Side side, int square = 0) const;
        virtual void vInitialize();
		virtual QString defaultFenString() const;

	protected:
		// Inherited from WesternBoard

        enum EmpirePieceType
        {
            Soldier = 7,	// Soldier = Moves like a Janggi Pawn
            Tower,    //  Siege Tower Moves like a Queen Captures like a Rook
            Eagle,     // Eagle = Moves like a Queen, Captures like a Knight
            Cardinal,      // Cardinal = Moves like a Queen, Captures like a Bishop
            Duke        // Duke = Moves like a Queen, Captures like a King
        };

        /*! Movement mask for Ferz move pattern. */
        static const unsigned FerzMovement = 16;
        /*! Movement mask for Wazir move pattern. */
        static const unsigned WazirMovement = 64;
        /*! Movement mask for Soldier move pattern. */
        static const unsigned SoldierMovement = 128;

		virtual void generateMovesForPiece(QVarLengthArray< Move >& moves,
						   int pieceType,
						   int square) const;

        virtual void addPromotions(int sourceSquare,
                         int targetSquare,
                         QVarLengthArray<Move>& moves) const;

private:

    int m_arwidth;

    QVarLengthArray<int> m_ferzOffsets;
    QVarLengthArray<int> m_wazirOffsets;
    QVarLengthArray<int> m_soldierOffsets;

    bool kingAtEnd(Side side) const;
    const QList<int> m_BackRank;


};


} // namespace Chess
#endif // EMPIREBOARD_H
