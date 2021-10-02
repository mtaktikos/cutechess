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

#ifndef NIGHTRIDERBOARD_H
#define NIGHTRIDERBOARD_H

#include "westernboard.h"

namespace Chess {

/*!
 * \brief A board for Nightrider Chess
 *
 * The Nightrider is one of the two most popular fairy chess pieces by chess problemists.
 * It was invented by the famous British chess problem composer T.R. Dawson.
 * The technical name for the piece is Knightrider, but nightrider was already a meaningful English word,
 * making it a more suitable name for the piece.
 *
 * This piece has been used in such recent variants as Cavalier Chess and Grand Cavalier Chess by Fergus Duniho, and in Xhess by David Howe.
 *
 *
 * \note Rules: https://en.wikipedia.org/wiki/Nightrider_(chess)
 *
 */
class LIB_EXPORT NightriderBoard : public WesternBoard
{
	public:
        /*! Creates a new NightriderBoard object. */
        NightriderBoard();

		// Inherited from WesternBoard
		virtual Board* copy() const;
        virtual QString variant() const;
        virtual QString defaultFenString() const;
        virtual void vInitialize();
        virtual bool inCheck(Side side, int square) const;
	protected:
        enum NightriderPieceType
		{
            Nightrider = Knight //! Nightrider Replaces Knight
		};
        static const unsigned NightriderMovement = 16;

        virtual void generateMovesForPiece(QVarLengthArray< Move >& moves,
                           int pieceType,
                           int square) const;
    private:

        QVarLengthArray<int> m_knightOffsets;
};

} // namespace Chess
#endif // NIGHTRIDERBOARD_H
