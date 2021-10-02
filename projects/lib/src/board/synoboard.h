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

#ifndef SynoBOARD_H
#define SynoBOARD_H

#include "pocketknightboard.h"

namespace Chess {

/*!
 * \brief A board for Syno Chess
 *
 * Synochess is a variant designed in 2020 by Couch Tomato.
 * The idea is to permit the western chess army (white) to fight against the xiangqi or Chinese chess army (black) in a fair manner.
 * The white army is called the Kingdom, while the black army represents an amalgamation of xiangqi and janggi (Korean chess) and is called the Dynasty.
 *
 * \note Rules: https://www.pychess.org/variant/synochess
 */


class LIB_EXPORT SynoBoard : public PocketKnightBoard
{
	public:
        /*! Creates a new SynoBoard object. */
        SynoBoard();

        // Inherited from PocketKnightBoard
		virtual Board* copy() const;
        virtual QString variant() const;
        virtual QList< Piece > reservePieceTypes() const;
        virtual Result result();
        virtual bool inCheck(Side side, int square = 0) const;
        virtual void vInitialize();
		virtual QString defaultFenString() const;
        virtual bool vIsLegalMove(const Move& move);
        //virtual int repeatCount() const;

	protected:
        // Inherited from PocketKnightBoard

        enum SynoPieceType
        {
            Advisor = 7,	//  Advisor = Like Janggi/Xiangqi moves like a king
            Cannon,    //  Janggi Piece -> Can only move between an obstacle and capture with an obstacle, same with checks
            Soldier,     // The pawns in janggi, can only move+capture forward and to the sides
            Elephant      // Shatranj Piece = Like a knight can leap throught pieces, moves 1 or 2 squares diagonally
        };

        /*! Movement mask for Ferz move pattern. */
        static const unsigned FerzMovement = 16;
        /*! Movement mask for Elephant moves. */
        static const unsigned ElephantMovement = 32;
        /*! Movement mask for Wazir move pattern. */
        static const unsigned WazirMovement = 64;
        /*! Movement mask for Cannon moves. */
        static const unsigned CannonMovement = 128;
        /*! Movement mask for Soldier moves. */
        static const unsigned SoldierMovement = 256;

		virtual void generateMovesForPiece(QVarLengthArray< Move >& moves,
						   int pieceType,
						   int square) const;


private:

    int m_arwidth;

    QVarLengthArray<int> m_ferzOffsets;
    QVarLengthArray<int> m_wazirOffsets;
    QVarLengthArray<int> m_elephantOffsets;
    QVarLengthArray<int> m_orthogonalOffsets;
    QVarLengthArray<int> m_soldierOffsets;

    bool kingAtEnd(Side side) const;
    const QList<int> m_BackRank;
};


} // namespace Chess
#endif // SYNOBOARD_H
