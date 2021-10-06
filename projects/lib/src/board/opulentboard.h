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

#ifndef OPULENTBOARD_H
#define OPULENTBOARD_H

#include "grandboard.h"

namespace Chess {

/*!
 * \brief A board for Opulent Chess
 *
 * Opulent Chess is a decimal chess variant similar to Christian Freeling's Grand Chess,
 * but with a stronger Knight, and two new jumping pieces (the Lion, and the Wizard.)
 * In this game, the Bishop, Knight, Lion, and Wizard should all have almost the same material value,
 * while having very different movement charistics. This should lead to a game with more exchanges between pieces of different types,
 * leading naturally to battles between different armies and hopefully giving each game played a more unique character.
 *
 * rules: https://www.chessvariants.com/rules/opulent-chess
 */
class LIB_EXPORT OpulentBoard : public GrandBoard
{
	public:
        /*! Creates a new OpulentBoard object. */
        OpulentBoard();

		// Inherited from AntiBoard
		virtual Board* copy() const;
		virtual QString variant() const;
		virtual QString defaultFenString() const;
        virtual void vInitialize();
        virtual bool inCheck(Side side, int square) const;
        virtual void addPromotions(int sourceSquare,
                        int targetSquare,
                        QVarLengthArray<Move>& moves) const;
        virtual bool vIsLegalMove(const Move& move);

	protected:
        enum OpulentPieceType
        {
            Lion = Chancellor+1, //! Lion
            Wizard               //! Wizard
        };
        static const unsigned wazirMovement    = 16;
        static const unsigned ferzMovement     = 64;
        static const unsigned lionMovement     = 128;
        static const unsigned wizardMovement   = 256;


        virtual void generateMovesForPiece(QVarLengthArray< Move >& moves,
                           int pieceType,
                           int square) const;
    private:

        QVarLengthArray<int> m_knightOffsets;
        QVarLengthArray<int> m_lionOffsets;
        QVarLengthArray<int> m_wizardOffsets;
        QVarLengthArray<int> m_ferzOffsets;
        QVarLengthArray<int> m_wazirOffsets;

}; // namespace Chess

}
#endif // OPULENTBOARD_H
