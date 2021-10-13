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

#ifndef MAYANBOARD_H
#define MAYANBOARD_H
#include "westernboard.h"

namespace Chess {

/*!
 * \brief Tomato's Mayan Chess
 */
class LIB_EXPORT MayanBoard : public WesternBoard
{
	public:
        /*! Creates a new MayanBoard object. */
        MayanBoard();

		// Inherited from WesternBoard
		virtual Board* copy() const;
		virtual QString variant() const;
		virtual QString defaultFenString() const;
        virtual int height() const;
        virtual int width() const;
        virtual void vInitialize();
        virtual int promotionRank() const;
        virtual void generatePawnMoves(int square,
                                                   QVarLengthArray< Move >& moves) const;
        virtual Result result();
        virtual QString lanMoveString(const Move& move);
        virtual Move moveFromLanString(const QString& istr);
        virtual int promotedPieceType(int type) const;


	protected:
        /*! Special piece types for Mayan */
        enum MayanPieceType
		{
            Offering = King + 1,
            Centaur,
            Archbishop,
            Quetzal
		};
		// Inherited from WesternBoard
		void virtual generateMovesForPiece(QVarLengthArray<Move>& moves,
						   int pieceType,
						   int square) const;
		virtual bool inCheck(Side side, int square = 0) const;
		virtual void addPromotions(int sourceSquare,
					   int targetSquare,
					   QVarLengthArray<Move>& moves) const;

        /*! Movement mask for Ferz move pattern. */
        static const unsigned FerzMovement = 16;
        /*! Movement mask for Elephant moves. */
        static const unsigned ElephantMovement = 32;
        /*! Movement mask for Wazir move pattern. */
        static const unsigned WazirMovement = 64;
        /*! Movement mask for Cannon moves. */
        static const unsigned QuetzalMovement = 128;
        /*! Movement mask for Soldier moves. */
        static const unsigned SoldierMovement = 256;
        /*! Movement mask for Dabba moves. */
        static const unsigned DabbaMovement = 512;
        /*! Movement mask for Crane moves. */
        static const unsigned CraneMovement = 1024;

        private:

            QVarLengthArray<int> m_ferzOffsets;
            QVarLengthArray<int> m_wazirOffsets;
            QVarLengthArray<int> m_elephantOffsets;
            QVarLengthArray<int> m_orthogonalOffsets;
            QVarLengthArray<int> m_soldierOffsets[2];
            QVarLengthArray<int> m_craneOffsets;
            QVarLengthArray<int> m_dabbaOffsets;

            bool kingAtEnd(Side side) const;
};

} // namespace Chess
#endif // MAYANBOARD_H
