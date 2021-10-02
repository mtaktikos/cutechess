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

#include "stalematewinboard.h"
#include "westernzobrist.h"

namespace Chess {

stalematewinBoard::stalematewinBoard()
	: WesternBoard(new WesternZobrist())
{
}

Board* stalematewinBoard::copy() const
{
    return new stalematewinBoard(*this);
}

QString stalematewinBoard::variant() const
{
    return "stalematewin";
}

QString stalematewinBoard::defaultFenString() const
{
	return "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
}


Result stalematewinBoard::result()
{
    QString str;

    // Checkmate/Stalemate
    if (!canMove())
    {
        if (inCheck(sideToMove()))
        {
            Side winner = sideToMove().opposite();
            str = tr("%1 mates").arg(winner.toString());

            return Result(Result::Win, winner, str);
        }
        else
        {
            str = tr("Win by stalemate");
            return Result(Result::Win, sideToMove().opposite(), str);
        }
    }
    else {
        return WesternBoard::result();
    }

}

} // namespace Chess
