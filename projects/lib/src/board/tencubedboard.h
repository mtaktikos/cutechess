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

#ifndef TENCUBEDBOARD_H
#define TENCUBEDBOARD_H

#include "capablancaboard.h"

namespace Chess {

/*!
 * \brief A board for Tencubed Chess
 *
 * Tencubed Chess is a variant of standard chess on a 10 x 10 board with
 * four new pieces
 *
 * TenCubed Chess uses ten different piece-types on a 10x10 board.
 * The Queen combines the moves of rook and bishop. The Marshall combines the moves of rook and knight.
 * The Archbishop combines the moves of bishop and knight. The Champion combines the moves of dabbabah and alfil and wazir.
 * The Wizard combines the moves of camel and ferz.
 *
 * \note Rules: https://www.chessvariants.com/contests/10/tencubedchess.html
 *
 *
 * \sa TencubedBoard
 */
class LIB_EXPORT TencubedBoard : public CapablancaBoard
{
	public:
        /*! Creates a new TencubedBoard object. */
        TencubedBoard();

        // Inherited from CapablancaBoard
		virtual Board* copy() const;
		virtual QString variant() const;
		virtual int height() const;
		virtual QString defaultFenString() const;
		virtual bool hasCastling() const;
		virtual void generateMovesForPiece(QVarLengthArray< Move >& moves,
						   int pieceType,
						   int square) const;
        virtual void vInitialize();
        virtual bool inCheck(Side side, int square) const;
        virtual void addPromotions(int sourceSquare,
                        int targetSquare,
                        QVarLengthArray<Move>& moves) const;
    protected:
    /*! Special piece types for Capablanca variants. */
    enum TencubedPieceType
    {
        Wizard = Chancellor+1,	//!< Wizard (Camel+Ferz)
        Champion            	//!< Champion (Dabba+Alfil+Ferz)
    };

    static const unsigned wazirMovement    = 16;
    static const unsigned ferzMovement     = 64;
    static const unsigned alfilMovement    = 128;
    static const unsigned wizardMovement   = 256;
    static const unsigned dabbaMovement    = 512;

	private:
        /*! Helper method for Pawn moves. Returns square offset for
         *  the given \a step with orientation \a sign. */
        inline int pawnMoveOffset(const PawnStep& ps,
                      int sign) const;

        QVarLengthArray<int> m_dabbaOffsets;
        QVarLengthArray<int> m_alfilOffsets;
        QVarLengthArray<int> m_wizardOffsets;
        QVarLengthArray<int> m_ferzOffsets;
        QVarLengthArray<int> m_wazirOffsets;


};

} // namespace Chess
#endif // TENCUBEDBOARD_H
