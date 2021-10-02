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

#ifndef ATOMARBOARD_H
#define ATOMARBOARD_H

#include "westernboard.h"

namespace Chess {

/*!
 * \brief A board for Atomar chess
 *
 * Atomar chess is an Atomic Variant where king have Royal Immunity (Like pawns they do not get caught in explosions unless directly captured)
 * https://chronatog.com/wp-content/uploads/2021/09/atomar-chess-rules.pdf
 *
 * Available at: https://chessvariants.training/Variant960
 *
 * A Variant designed by Chronatog.
 */
class LIB_EXPORT AtomarBoard : public WesternBoard
{
	public:
        /*! Creates a new AtomarBoard object. */
        AtomarBoard();

		// Inherited from WesternBoard
		virtual Board* copy() const;
		virtual QString variant() const;
		virtual QString defaultFenString() const;
		virtual Result result();

	protected:
		// Inherited from WesternBoard
		virtual void vInitialize();
		virtual bool inCheck(Side side, int square = 0) const;
		virtual bool kingCanCapture() const;
		virtual bool vSetFenString(const QStringList& fen);
		virtual bool vIsLegalMove(const Move& move);
		virtual void vMakeMove(const Move& move,
				       BoardTransition* transition);
		virtual void vUndoMove(const Move& move);

	private:
		struct MoveData
		{
			bool isCapture;
			Piece piece;
			Piece captures[8];
		};

		QVector<MoveData> m_history;
		int m_offsets[8];
};

} // namespace Chess
#endif // ATOMARBOARD_H
