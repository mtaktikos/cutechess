/*
    This file is part of Cute Chess.

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

#ifndef ChaturangaBOARD_H
#define ChaturangaBOARD_H

#include "shatranjboard.h"

namespace Chess {

/*!
 * \brief A board for Chaturanga
 * Chaturanga is first known from the Gupta Empire in India around the 6th century CE. In the 7th century,
 * it was adopted as chatrang (shatranj) in Sassanid Persia, which in turn was the form of chess brought to late-medieval Europe.
 * Archeological remains from 2000-3000 B.C have been found from the city of Lothal (of the Indus Valley Civilisation) of pieces on a board that resemble chess.
 * According to Stewart Culin, chaturanga was first described in the Hindu text Bhavishya Purana.
 *
 * The exact rules of chaturanga are unknown. Chess historians suppose that the game had similar rules to those of its successor, shatranj.
 * In particular, there is uncertainty as to the moves of the Gaja (elephant).
 *
 * The rules applied here are the same as described on chess.com and Fairy-Stockfish' code
 *
 * https://en.wikipedia.org/wiki/Chaturanga
 *
 */
class LIB_EXPORT ChaturangaBoard : public ShatranjBoard
{
	public:
        /*! Creates a new ChaturangaBoard object. */
        ChaturangaBoard();

		virtual Board* copy() const;
		virtual QString variant() const;
		virtual QString defaultFenString() const;
        virtual Result result();

    private:
        bool bareKing(Side side, int count = 0) const;
};

} // namespace Chess
#endif // ChaturangaBOARD_H
