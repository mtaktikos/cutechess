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

#include "pawnsinglestepboard.h"
#include "westernzobrist.h"

namespace Chess {

PawnsinglestepBoard::PawnsinglestepBoard()
    : WesternBoard(new WesternZobrist())
{
}

Board* PawnsinglestepBoard::copy() const
{
    return new PawnsinglestepBoard(*this);
}

QString PawnsinglestepBoard::variant() const
{
    return "pawnsinglestep";
}

QString PawnsinglestepBoard::defaultFenString() const
{
	return "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
}

bool PawnsinglestepBoard::pawnHasDoubleStep() const {
    return false;
}

void PawnsinglestepBoard::vInitialize()
{
    WesternBoard::vInitialize();

}


void PawnsinglestepBoard::generateMovesForPiece(QVarLengthArray< Move >& moves,
                          int pieceType,
                          int square) const
{

     WesternBoard::generateMovesForPiece(moves,pieceType,square);
}


} // namespace Chess
