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

#include "atomarboard.h"
#include "westernzobrist.h"
#include "boardtransition.h"


namespace Chess {

AtomarBoard::AtomarBoard()
	: WesternBoard(new WesternZobrist())
{
	for (int i = 0; i < 8; i++)
		m_offsets[i] = 0;
}

Board* AtomarBoard::copy() const
{
    return new AtomarBoard(*this);
}

QString AtomarBoard::variant() const
{
    return "atomar";
}

QString AtomarBoard::defaultFenString() const
{
	return "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
}

bool AtomarBoard::kingCanCapture() const
{
    return true;
}

void AtomarBoard::vInitialize()
{
	int arwidth = width() + 2;
	m_offsets[0] = -arwidth - 1;
	m_offsets[1] = -arwidth;
	m_offsets[2] = -arwidth + 1;
	m_offsets[3] = -1;
	m_offsets[4] = 1;
	m_offsets[5] = arwidth - 1;
	m_offsets[6] = arwidth;
	m_offsets[7] = arwidth + 1;

	WesternBoard::vInitialize();
}

bool AtomarBoard::vSetFenString(const QStringList& fen)
{
	m_history.clear();
	return WesternBoard::vSetFenString(fen);
}

bool AtomarBoard::inCheck(Side side, int square) const
{
    return false;
}

bool AtomarBoard::vIsLegalMove(const Move& move)
{
	Q_ASSERT(!move.isNull());

    if (pieceAt(move.sourceSquare()).type()==King && pieceAt(move.targetSquare()).type()==King)
    {
            return false;
    }

	return WesternBoard::vIsLegalMove(move);
}

void AtomarBoard::vMakeMove(const Move& move, BoardTransition* transition)
{
	MoveData md;
	md.isCapture = (captureType(move) != Piece::NoPiece);
    md.piece = pieceAt(move.sourceSquare());
    Piece wasKing = pieceAt(move.sourceSquare());
	WesternBoard::vMakeMove(move, transition);

	if (md.isCapture)
	{
		int target = move.targetSquare();
        if (wasKing.type() != King) {
            setSquare(target, Piece::NoPiece);
         }
		for (int i = 0; i < 8; i++)
		{
			int sq = target + m_offsets[i];
			Piece& pc = md.captures[i];
			pc = pieceAt(sq);
            if (pc.isWall() || pc.type() == Pawn || pc.type() == King)
				continue;
			
            removeCastlingRights(sq);
            setSquare(sq, Piece::NoPiece);

			if (transition != nullptr)
				transition->addSquare(chessSquare(sq));
		}
	}
	m_history << md;
}

void AtomarBoard::vUndoMove(const Move& move)
{
	int source = move.sourceSquare();
	int target = move.targetSquare();

	WesternBoard::vUndoMove(move);

	const MoveData& md = m_history.last();
	if (md.isCapture)
	{
		setSquare(source, md.piece);
		for (int i = 0; i < 8; i++)
		{
			int sq = target + m_offsets[i];
			if (md.captures[i].isValid())
				setSquare(sq, md.captures[i]);
		}
	}

	m_history.pop_back();
}

Result AtomarBoard::result()
{
	Side side(sideToMove());
	if (pieceAt(kingSquare(side)).isEmpty())
	{
		Side winner = side.opposite();
		QString str = tr("%1's king exploded").arg(side.toString());
		return Result(Result::Win, winner, str);
	}

	return WesternBoard::result();
}

} // namespace Chess
