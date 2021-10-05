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

#ifndef TORISHOGIBOARD_H
#define TORISHOGIBOARD_H

#include "shogiboard.h"

namespace Chess {

/*!
 * \brief A board for ToriShogi (5五将棋 gogo shōgi)
 *
 * Tori Shogi (禽将棋/鳥将棋), Japanese for Bird Chess/Shogi, is a Shogi variant invented by Toyota Genryu in 1799,
 * although traditionally attributed to his master Ōhashi Sōei. The game is played on a 7x7 board and is one of the oldest Shogi variants to use the drop rule.
 * The game enjoys a relative degree of popularity to this day, including English-language books and tournaments.
 *
 * \note Rules: https://www.pychess.org/variants/torishogi
 * https://www.chessvariants.com/shogivariants.dir/tori.html
 * https://chess.fandom.com/wiki/Tori_Shogi
 *
 * Notation used here is the same as Shogi's Board
 *
 */
class LIB_EXPORT ToriShogiBoard : public ShogiBoard
{
    public:
        /*! Creates a new ToriShogiBoard object. */
        ToriShogiBoard();

        // Inherited from ShogiBoard
        virtual Board* copy() const;
        virtual QString variant() const;
        virtual int height() const;
        virtual int width() const;
        virtual QString defaultFenString() const;
        virtual Result result();
        virtual int promotionRank() const;
        virtual int normalPieceType(int type) const;
        virtual int promotedPieceType(int type) const;
        virtual bool variantHasOptionalPromotions() const;
        virtual void generateMovesForPiece(QVarLengthArray<Move>& moves,
                                int pieceType,
                                int square) const;
        virtual void vInitialize();

protected:
        enum ToriShogiPieceType
        {
            Falcon = PromotedRook+1,
            Crane,
            LeftQuail,
            RightQuail,
            Pheasant,
            Goose,
            Eagle
        };

        /*! Movement mask for Falcon moves. */
        static const unsigned FalconMovement = 2;
        /*! Movement mask for Crane moves. */
        static const unsigned CraneMovement = 4;
        /*! Movement mask for Left Quail move pattern. */
        static const unsigned LeftQuailMovement = 8;
        /*! Movement mask for RightQuail move pattern. */
        static const unsigned RightQuailMovement = 16;
        /*! Movement mask for Goose moves. */
        static const unsigned GooseMovement = 64;
        /*! Movement mask for Eagle moves. */
        static const unsigned EagleMovement = 128;
        /*! Movement mask for Pheasant moves. */
        static const unsigned PheasantMovement = 256;
private:
        virtual bool fileIsAllowed(int pieceType, int square) const;
        virtual bool rankIsAllowed(int pieceType, int square) const;
        virtual bool inPromotionZone(int square) const;
        QVarLengthArray<int> m_bishopOffsets;
        QVarLengthArray<int> m_rookOffsets;
        QVarLengthArray<int> m_lanceOffsets[2];
        QVarLengthArray<int> m_craneOffsets;
        QVarLengthArray<int> m_falconOffsets[2];
        QVarLengthArray<int> m_pheasantOffsets[2];
        QVarLengthArray<int> m_rightquailSlideOffsets[2];
        QVarLengthArray<int> m_rightquailHopOffsets[2];
        QVarLengthArray<int> m_leftquailSlideOffsets[2];
        QVarLengthArray<int> m_leftquailHopOffsets[2];
        QVarLengthArray<int> m_gooseOffsets[2];
        QVarLengthArray<int> m_eagleSlideOffsets[2];
        QVarLengthArray<int> m_eagleHopOffsets[2];
        QVarLengthArray<int> m_eagleDiagOffsets[2];
        QVarLengthArray<int> m_eagleLongDiagOffsets[2];
};

} // namespace Chess
#endif // TORISHOGIBOARD_H
