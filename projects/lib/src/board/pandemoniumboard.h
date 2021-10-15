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

#ifndef PANDEMONIUMBOARD_H
#define PANDEMONIUMBOARD_H

#include "board.h"

namespace Chess {

/* The code here was taken from directly from Shogi and adapted to
 * Pandemonium (due to annoying bugs and lack of patience to solve them).
 *  All Credits to the authors
*/

/*!
 * \brief Pandemonium (Surajang修羅場)
 *
 * A variant made by Daphne Snowmoon.
 *
 * \note Rules: https://www.chessvariants.com/rules/pandemonium
 *
 */
class LIB_EXPORT PandemoniumBoard : public Board
{
    public:
        /*! Creates a new PandemoniumBoard object. */
        PandemoniumBoard();

        // Inherited from Board
        virtual Board* copy() const;
        virtual QList<Piece> reservePieceTypes() const;
        virtual QString variant() const;
        virtual bool variantHasDrops() const;
        virtual bool variantHasOptionalPromotions() const;
        virtual QString defaultFenString() const;
        virtual CoordinateSystem coordinateSystem() const;
        virtual int width() const;
        virtual int height() const;
        virtual Result result();

    protected:
        /*!
         * Piece types for Pandemonium.
         */
        enum PandemoniumPieceType
        {
            Pawn = 1,
            Knight,
            Rook,
            Bishop,
            Ferz,
            Van,
            Alfil,
            Erlking,
            Bull,
            King,

            PromotedPawn,
            PromotedKnight,
            PromotedBishop,
            PromotedFerz,
            PromotedVan,
            PromotedAlfil,
            PromotedErlking,
            PromotedBull,
            PromotedRook
        };

        /*! Movement mask for Ferz moves. */
        static const unsigned FerzMovement = 2;
        /*! Movement mask for Wazir moves. */
        static const unsigned WazirMovement = 4;
        /*! Movement mask for Dabba move pattern. */
        static const unsigned DabbaMovement = 8;
        /*! Movement mask for Alfil move pattern. */
        static const unsigned AlfilMovement = 16;
        /*! Movement mask for Rook moves. */
        static const unsigned RookMovement = 64;
        /*! Movement mask for Bishop moves. */
        static const unsigned BishopMovement = 128;
        /*! Movement mask for Knight moves. */
        static const unsigned KnightMovement = 256;

        virtual int kingSquare(Side side) const;
        /*!
         * Returns promoted piece type corresponding to normal \a type.
         */
        virtual int promotedPieceType(int type) const;
        /*!
         * Returns the lowest (relative) rank of the promotion zone.
         */
        virtual int promotionRank() const;
        /*!
         * Returns original piece type corresponding to promoted \a type.
         */
        virtual int normalPieceType(int type) const;
        /*!
         * Returns whether the current position has no pieces on
         * forbidden ranks.
         */
        virtual bool ranksAreAllowed() const;
        /*!
         * Returns true if the impassé point rule is active else false.
         */
        virtual bool hasImpassePointRule() const;
        /*!
         * Criteria of impasse rule limit given material value of
         * \a points and the number of \a pieces of the side to move
         * in the promotion zone.
         */
        virtual Result impassePointRule(int points, int pieces) const;

        // Inherited from Board
        virtual int reserveType(int pieceType) const;
        virtual void vInitialize();
        virtual QString vFenString(FenNotation notation) const;
        virtual bool vSetFenString(const QStringList& fen);
        virtual QString lanMoveString(const Move& move);
        virtual QString sanMoveString(const Move& move);
        virtual Move moveFromLanString(const QString& str);
        virtual Move moveFromSanString(const QString& str);
        virtual void vMakeMove(const Move& move,
                       BoardTransition* transition);
        virtual void vUndoMove(const Move& move);
        virtual void generateMovesForPiece(QVarLengthArray<Move>& moves,
                           int pieceType,
                           int square) const;
        virtual bool isLegalPosition();
        virtual bool vIsLegalMove(const Move& move);
        virtual bool inCheck(Side side, int square = 0) const;

    private:
        // Data for reversing/unmaking a move
        struct MoveData
        {
            Piece capture;
        };

        bool rankIsAllowed(int , int) const;
        bool fileIsAllowed(int, int) const;
        bool inPromotionZone(int square) const;
        Result resultFromImpassePointRule() const;

        int m_kingSquare[2];
        int m_promotionRank;
        int m_minIndex;
        int m_maxIndex;
        int m_plyOffset;
        bool m_multiDigitNotation;
        bool m_hasImpassePointRule;

        QVarLengthArray<int> m_bishopOffsets;
        QVarLengthArray<int> m_rookOffsets;
        QVarLengthArray<int> m_dabbaOffsets;
        QVarLengthArray<int> m_alfilOffsets;
        QVarLengthArray<int> m_knightOffsets;
        QVarLengthArray<int> m_soldierOffsets[2];
        QVector<MoveData> m_history;
};

} // namespace Chess
#endif // PandemoniumBOARD_H
