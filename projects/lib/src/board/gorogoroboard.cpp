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

#include "gorogoroboard.h"

namespace Chess {

GorogoroBoard::GorogoroBoard()
    : MiniShogiBoard()
{
}

Board* GorogoroBoard::copy() const
{
    return new GorogoroBoard(*this);
}

QString GorogoroBoard::variant() const
{
    return "gorogoro";
}

int GorogoroBoard::height() const
{
    return 6;
}

int GorogoroBoard::width() const
{
    return 5;
}

QString GorogoroBoard::defaultFenString() const
{
    return "sgkgs/5/1ppp1/1PPP1/5/SGKGS[-] w 0 1";
}


} // namespace Chess
