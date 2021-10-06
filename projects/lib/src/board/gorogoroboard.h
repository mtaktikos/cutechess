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

#ifndef GOROGOROBOARD_H
#define GOROGOROBOARD_H

#include "minishogiboard.h"

namespace Chess {

/*!
 * \brief A board for Gorogoro (5五将棋 gogo shōgi)
 *
 * A 5x6 version of Dobutsu Shogi
 *
 * \note Rules: http://en.wikipedia.org/wiki/Dōbutsu_shōgi
 *
 * \sa ShogiBoard
 */
class LIB_EXPORT GorogoroBoard : public MiniShogiBoard
{
	public:
        /*! Creates a new GorogoroBoard object. */
        GorogoroBoard();

		// Inherited from ShogiBoard
		virtual Board* copy() const;
		virtual QString variant() const;
		virtual int height() const;
		virtual int width() const;
		virtual QString defaultFenString() const;

	protected:

};

} // namespace Chess
#endif // GOROGOROBOARD_H
