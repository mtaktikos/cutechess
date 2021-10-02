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

#ifndef SHINOBIBOARD_H
#define SHINOBIBOARD_H

#include "pocketknightboard.h"

namespace Chess {

/*!
    Shinobi Chess is a chess variant designed in 2021 by Couch Tomato,
    fourth in the series of asymmetric games. The chess army (“the Kingdom,” black)
    has invaded the land of the Sakura Clan (pink/sakura). While initially unprepared and having very few pieces available,
    the Clan is very resourceful and can instantly recruit and summon allies to defend at a minute’s notice!
    The Clan starts with almost all of its pieces in hand and can drop them on its side of the board in lieu of moving a piece.
    They can also promote upon reaching the far end of the board –
    these abilities are similar to the game of shogi,
    with the exception that captured pieces do not go into a player’s hand, so each drop counts!

    by: CouchTomato

    (Personal Note: I kept the Samurai piece in the code, as an homage to the original version)

    Rules: https://www.pychess.org/variants/shinobi
 */


class LIB_EXPORT ShinobiBoard : public PocketKnightBoard
{
	public:
        /*! Creates a new ShinobiBoard object. */
        ShinobiBoard();

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
        virtual bool rankIsAllowed(int pieceType, int square) const;
        virtual void addPromotions(int, int,
                         QVarLengthArray<Move>& moves) const;
        virtual void generatePawnMoves(int square, QVarLengthArray< Move >& moves) const;
        virtual void generateMonkMoves(int square, QVarLengthArray< Move >& moves) const;
        virtual void generateWoodenHorseMoves(int square, QVarLengthArray< Move >& moves) const;
        virtual void generateLanceMoves(int square, QVarLengthArray< Move >& moves) const;
        virtual int promotedPieceType(int type) const;

	protected:
        // Inherited from PocketKnightBoard

        enum ShinobiPieceType
        {
            Captain = 7,	// Promoted Pawn = Moves like a King
            Ninja,    //  Archbishop (R+B)
            Samurai,     // Chancellor (R+N) From original setup
            Monk,      // Fers (1 square diagonal) = Promotes to Bishop
            WoodenHorse, //Shogi Knight Promotes to Knight
            Lance,  //Shogi Lance Promotes to Rook
            Dragon //Rook+King
        };

        /*! Movement mask for Ferz move pattern. */
        static const unsigned FerzMovement = 16;
        /*! Movement mask for WoodenKnight moves. */
        static const unsigned WoodenKnightMovement = 32;
        /*! Movement mask for Wazir move pattern. */
        static const unsigned WazirMovement = 64;
        /*! Movement mask for Lance move pattern. */
        static const unsigned LanceMovement = 128;

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

    bool kingAtEnd(Side side) const;
    const QList<int> m_BackRank;
};


} // namespace Chess
#endif // SHINOBIBOARD_H
