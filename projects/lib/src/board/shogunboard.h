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

#ifndef SHOGUNBOARD_H
#define SHOGUNBOARD_H

#include "crazyhouseboard.h"

namespace Chess {

/*!
    Shogun Chess is a chess variant designed in 2019-2020 by Couch Tomato.
    While the game itself is a blend of western chess and shogi rules,
    the original idea for the game was a way to introduce the hybrid pieces (commonly known as the archbishop and chancellor)
    in a way different than other variants have done.
    For example, keeping an 8x8 instead of enlarging the board so as to not diminish the value of the minor pieces,
    or introduce them in a board that is not as cluttered as in S-chess.
    The idea evolved to introduce these pieces through the means of promotion from the minor pieces and the rook at an earlier rank than the 8th rank.
    Later, drops were also introduced into the rules as a means to increase the offensive options to offset
    the defensive nature introduced by the need to protect against the threat of promotion.
    Unique pawn and knight promotions as well as even a queen demotion were added to complete the theme and symmetry.

    The name was originally tentatively "General's Chess," based on what was then the name of the pawn's promotion,
    now currently the name of the knight's promotion. However, with the drops and promotion zones based off shogi,
    the Japanese word for "general," shogun sounded more appropriate.
    The sho in shogun being the same as the one in shogi pays tribute to that as well.


    Rules: https://www.pychess.org/variants/shogun
 */


class LIB_EXPORT ShogunBoard : public CrazyhouseBoard
{
	public:
        /*! Creates a new ShogunBoard object. */
        ShogunBoard();

        // Inherited from PocketKnightBoard
		virtual Board* copy() const;
        virtual QString variant() const;
        virtual QList< Piece > reservePieceTypes() const;
        virtual Result result();
        virtual bool inCheck(Side side, int square = 0) const;
        virtual void vInitialize();
		virtual QString defaultFenString() const;
        virtual int promotionRank() const;
        virtual bool inPromotionZone(int square) const;
        virtual int reverseProm(int type) const;
        virtual void vMakeMove(const Move& move, BoardTransition* transition);
        virtual void vUndoMove(const Move& move);
        virtual Move moveFromLanString(const QString& istr);
        virtual QString lanMoveString(const Move& move);
        virtual void addPromotions(int, int,
                         QVarLengthArray<Move>& moves) const;
        virtual int promotedPieceType(int type) const;
        virtual int reserveType(int pieceType) const;
        virtual bool rankIsAllowed(int pieceType, int square) const;
        bool pieceOnBoard(Piece p, Side side) const;
        bool pieceIsPromotion(int typePiece) const;


	protected:
        // Inherited from PocketKnightBoard

        enum ShogunPieceType
        {
            Duchess = King+1,	// Demoted Queen -> Ferz
            Mortar,             // Mortar (R+N)
            Archbishop,         // Archbishop (R+B)
            General,            // General (K+N)
            Captain             // Captain = King's Movement
        };

        /*! Movement mask for Ferz move pattern. */
        static const unsigned FerzMovement = 16;
        /*! Movement mask for Wazir move pattern. */
        static const unsigned WazirMovement = 32;

		virtual void generateMovesForPiece(QVarLengthArray< Move >& moves,
						   int pieceType,
						   int square) const;


private:

    int m_arwidth;
    int m_sign;
    int m_promotionRank;

    QVarLengthArray<int> m_ferzOffsets;
    QVarLengthArray<int> m_wazirOffsets;
    QVarLengthArray<int> m_elephantOffsets;
    QVarLengthArray<int> m_orthogonalOffsets;
    QVarLengthArray<int> m_soldierOffsets;
    QVarLengthArray<int> m_lanceOffsets[2];
    QVarLengthArray<int> m_woodenKnightOffsets[2];

    const QList<int> m_BackRank;
};


} // namespace Chess
#endif // ShogunBOARD_H
