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

#include "chaturangaboard.h"

namespace Chess {

ChaturangaBoard::ChaturangaBoard()
    : ShatranjBoard()
{

}

Board* ChaturangaBoard::copy() const
{
    return new ChaturangaBoard(*this);
}

QString ChaturangaBoard::variant() const
{
    return "chaturanga";
}

QString ChaturangaBoard::defaultFenString() const
{
    return "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w - - 0 1";
}


Result ChaturangaBoard::result()
{
    Side side = sideToMove();

    // mate or stalemate
    if (!canMove())
    {
        Side winner = side.opposite();
        bool check = inCheck(side);
        QString str{ tr("%1 %2")
                 .arg(winner.toString(),
                      check ? tr("mates") : tr("wins by stalemate")) };
        return Result(Result::Win, winner, str);
    }

    // bare king
    if (bareKing(side))
    {
        if (bareKing(side.opposite()))
        {
            QString str{ tr("Both kings bare") };
            return Result(Result::Draw, Side::NoSide, str);
        }
    }

    // 70 moves rule
    if (reversibleMoveCount() >= 140)
    {
        QString str{ tr("Draw by seventy moves rule") };
        return Result(Result::Draw, Side::NoSide, str);
    }

    // 3-fold repetition
    if (repeatCount() >= 2)
    {
        QString str{ tr("Draw by 3-fold repetition") };
        return Result(Result::Draw, Side::NoSide, str);
    }

    return Result();
}

bool ChaturangaBoard::bareKing(Side side, int count) const
{
    for (int i = 0; count < 2 && i < arraySize(); i++)
    {
        if (side == pieceAt(i).side())
            ++count;
    }
    return count < 2;
}


} // namespace Chess
