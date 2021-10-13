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

#include "mayanboard.h"
#include "westernzobrist.h"

namespace Chess {

MayanBoard::MayanBoard()
	: WesternBoard(new WesternZobrist())
{
    setPieceType(King, tr("king"), "K", FerzMovement|WazirMovement|ElephantMovement|DabbaMovement, "K");
    setPieceType(Quetzal, tr("quetzal"), "S", QuetzalMovement, "Q");
    setPieceType(Bishop, tr("bishop"), "B", CraneMovement, "B");
    setPieceType(Centaur, tr("centaur"), "E", KnightMovement|FerzMovement|WazirMovement , "M");
    setPieceType(Offering, tr("offering"), "O", 0 , "F");
    setPieceType(Pawn, tr("pawn"), "P", SoldierMovement, "P");
    setPieceType(Knight, tr("knight"), "J", KnightMovement, "N");
}

bool MayanBoard::kingAtEnd(Side side) const
{
    Square ksq = chessSquare(kingSquare(side));
    return (side==Side::Black && ksq.rank()==1 && ksq.file()==4)|((side==Side::White)&&ksq.rank()==7&&ksq.file()==4);
}

Result MayanBoard::result()
{
    if (kingAtEnd(Side::White))
        return Result(Result::Win, Side::White,
                  tr("Camp mate for White"));
    if (kingAtEnd(Side::Black))
        return Result(Result::Win, Side::Black,
                  tr("Camp Mate for Black"));
    return WesternBoard::result();
}

void MayanBoard::vInitialize()
{
    WesternBoard::vInitialize();
    int arwidth = width() + 2;

    m_craneOffsets.resize(6);
    m_craneOffsets[0] = -arwidth - 1;
    m_craneOffsets[1] = -arwidth + 1;
    m_craneOffsets[2] = arwidth - 1;
    m_craneOffsets[3] = arwidth + 1;
    m_craneOffsets[4] = arwidth;
    m_craneOffsets[5] = -arwidth;


    m_ferzOffsets.resize(4);
    m_ferzOffsets[0] = -arwidth - 1;
    m_ferzOffsets[1] = -arwidth + 1;
    m_ferzOffsets[2] = arwidth - 1;
    m_ferzOffsets[3] = arwidth + 1;

    m_wazirOffsets.resize(4);
    m_wazirOffsets[0] = -arwidth;
    m_wazirOffsets[1] = -1;
    m_wazirOffsets[2] = 1;
    m_wazirOffsets[3] = arwidth;

    m_dabbaOffsets.resize(4);
    m_dabbaOffsets[0] = 2 * (-arwidth);
    m_dabbaOffsets[1] = -2;
    m_dabbaOffsets[2] = 2;
    m_dabbaOffsets[3] = 2*arwidth;

    m_elephantOffsets.resize(4);
    m_elephantOffsets[0] = -2 * arwidth - 2;
    m_elephantOffsets[1] = -2 * arwidth + 2;
    m_elephantOffsets[2] = 2 * arwidth - 2;
    m_elephantOffsets[3] = 2 * arwidth + 2;
    //Elephant Offsets from Shatranj

    m_orthogonalOffsets.resize(4);
    m_orthogonalOffsets[0] = -arwidth;
    m_orthogonalOffsets[1] = -1;
    m_orthogonalOffsets[2] =  1;
    m_orthogonalOffsets[3] =  arwidth;
    //Cannon taken from https://github.com/gaintpd/cutechess

    m_soldierOffsets[Side::Black].resize(3);
    m_soldierOffsets[Side::Black][0] = arwidth;
    m_soldierOffsets[Side::Black][1] = -1;
    m_soldierOffsets[Side::Black][2] = 1;

    m_soldierOffsets[Side::White].resize(3);
    m_soldierOffsets[Side::White][0] = -arwidth;
    m_soldierOffsets[Side::White][1] = -1;
    m_soldierOffsets[Side::White][2] = 1;
    //soldier offsets taken from https://github.com/gaintpd/cutechess

}
Board* MayanBoard::copy() const
{
    return new MayanBoard(*this);
}

QString MayanBoard::variant() const
{
    return "mayan";
}

int MayanBoard::width() const
{
    return 9;
}

int MayanBoard::height() const
{
    return 9;
}

QString MayanBoard::defaultFenString() const
{
    return "rjbeksbjr/4o4/p1p1p1p1p/9/9/9/P1P1P1P1P/4O4/RJBEKSBJR w - - 1 1";
}

void MayanBoard::addPromotions(int,
                    int,
                    QVarLengthArray< Move >&) const
{
}

void MayanBoard::generatePawnMoves(int square,
                    QVarLengthArray< Move >& moves) const
{
    // Generate moves for pawn
    QVarLengthArray< Move > moves1;
    Side side = sideToMove();
    generateHoppingMoves(square, m_soldierOffsets[side], moves1);

    int arwidth = width() + 2;

    // Promote pawn to Captain when reaching 7th rank
    for (const Move& m: moves1)
    {
        int rank = height() + 1 - m.targetSquare() / arwidth;
        int rrank = (side == Side::White) ? rank : height() - 1 - rank;
        moves.append(m);
        if (rrank >= promotionRank()) {
            moves.append(Move(square, m.targetSquare(), Bishop));
            addPromotions(square, m.targetSquare(), moves); }
        /*else if (m.promotion() != 0)
            moves.append(m);*/

    }
}

QString MayanBoard::lanMoveString(const Move& move)
{

    int source = move.sourceSquare();
    int target = move.targetSquare();
    int prom = move.promotion();

    if (move.sourceSquare() == kingSquare(sideToMove())) {
            return WesternBoard::lanMoveString(move); //Needed to fix the castling bug
    }

    QString str;
    // Piece drop
    if (source == 0)
    {
        Q_ASSERT(prom != Piece::NoPiece);
        QChar drop = true ? '@' : '*';
        str += pieceSymbol(prom).toUpper() + drop;
        str += squareString(target);
        return str;
    }

    str = squareString(source);
    str += squareString(target);
    if (prom != Piece::NoPiece)
        str += "+";
    return str;
}

int MayanBoard::promotedPieceType(int type) const
{
    switch (type)
    {
    case Pawn:
        return Bishop;
    default:
        return type;
    }
}

Move MayanBoard::moveFromLanString(const QString& istr)
{

    QString str(istr);
    Side side = sideToMove();
    QString kingSymbol(pieceSymbol(King).toUpper());
    int ksymlen = kingSymbol.length();
    int len = str.length();
    int target = squareIndex(str.mid(ksymlen, len - ksymlen));
    int kingFile = chessSquare(kingSquare(side)).file();

    //conditions for castling
    if (hasCastlingRight(side, QueenSide)
    &&  target == kingSquare(side) + castlingFile(QueenSide) - kingFile)
        return WesternBoard::moveFromLanString(str);

    if (hasCastlingRight(side, KingSide)
    &&  target == kingSquare(side) + castlingFile(KingSide) - kingFile)
        return WesternBoard::moveFromLanString(str);

    Move move = WesternBoard::moveFromLanString(str);
    bool isPromotion = (str.endsWith("+"));
    if (!isPromotion) { return move; }
    int source = move.sourceSquare();
    Piece piece = pieceAt(source);
    int type = piece.type();
    int promotion =  promotedPieceType(type);
    return Move(move.sourceSquare(), move.targetSquare(), promotion);
}

void MayanBoard::generateMovesForPiece(QVarLengthArray<Move>& moves,
					    int pieceType,
					    int square) const
{
    Side opSide = sideToMove().opposite();
    if (pieceType == King)
    {
        generateHoppingMoves(square, m_ferzOffsets, moves);
        generateHoppingMoves(square, m_wazirOffsets, moves);
        for (int i=0; i<m_ferzOffsets.size();i++) {
            int sq= square+m_ferzOffsets[i];
            if (pieceAt(sq) == Piece::NoPiece) {
                int tar_sq=square+m_elephantOffsets[i];
                Piece capture = pieceAt(tar_sq);
                if (capture.isEmpty() || capture.side() == opSide)
                    moves.append(Move(square, tar_sq));
            }
        }
        for (int i=0; i<m_wazirOffsets.size();i++) {
            int sq= square+m_wazirOffsets[i];
            if (pieceAt(sq) == Piece::NoPiece) {
                int tar_sq=square+m_dabbaOffsets[i];
                Piece capture = pieceAt(tar_sq);
                if (capture.isEmpty() || capture.side() == opSide)
                    moves.append(Move(square, tar_sq));
            }
        }
        return;
	}
    if (pieceType == Knight || pieceType == Rook) {
        WesternBoard::generateMovesForPiece(moves,pieceType,square);
    }
    if (pieceType == Pawn) {
        generatePawnMoves(square,moves);
    }
    if (pieceType == Centaur) {
        WesternBoard::generateMovesForPiece(moves,Knight,square);
        generateHoppingMoves(square, m_ferzOffsets, moves);
        generateHoppingMoves(square, m_wazirOffsets, moves);
    }
    if (pieceType == Bishop)
        generateHoppingMoves(square, m_craneOffsets, moves);
    if (pieceType == Quetzal) {
        QVarLengthArray<int> cannonRelOffsets;
        cannonRelOffsets.clear();
        Side side = sideToMove();
        for (int i = 0; i < m_orthogonalOffsets.size(); i++)
        {
            int offset = m_orthogonalOffsets[i];
            int targetSquare = square + offset;

            Piece capture;
            int obstacle = 0;
            while(!(capture = pieceAt(targetSquare)).isWall())
            {
                if(capture.isEmpty())
                {
                    if(obstacle == 1)
                    {
                        // moves.append(Move(sourceSquare, targetSquare));
                        cannonRelOffsets.append(targetSquare - square);
                    }
                }
                else
                {
                    obstacle++;
                    if(obstacle == 1 && capture.type() == Quetzal)
                        break;
                    if(obstacle == 2 && capture.side() != side)
                    {
                        // moves.append(Move(sourceSquare, targetSquare));
                        cannonRelOffsets.append(targetSquare - square);
                        break;
                    }
                }
                targetSquare += offset;
            }
        }

        generateHoppingMoves(square, cannonRelOffsets, moves);

        cannonRelOffsets.clear();
        for (int i = 0; i < m_ferzOffsets.size(); i++)
        {
            int offset = m_ferzOffsets[i];
            int targetSquare = square + offset;

            Piece capture;
            int obstacle = 0;
            while(!(capture = pieceAt(targetSquare)).isWall())
            {
                if(capture.isEmpty())
                {
                    if(obstacle == 1)
                    {
                        // moves.append(Move(sourceSquare, targetSquare));
                        cannonRelOffsets.append(targetSquare - square);
                    }
                }
                else
                {
                    obstacle++;
                    if(obstacle == 1 && capture.type() == Quetzal)
                        break;
                    if(obstacle == 2 && capture.side() != side)
                    {
                        // moves.append(Move(sourceSquare, targetSquare));
                        cannonRelOffsets.append(targetSquare - square);
                        break;
                    }
                }
                targetSquare += offset;
            }
        }

        generateHoppingMoves(square, cannonRelOffsets, moves);
    }


}

int MayanBoard::promotionRank() const
{
    // Promotion zone is opponent's third. Taken from shogi
    return height() - 3;

}

bool MayanBoard::inCheck(Side side, int square) const
{
	if (square == 0)
		square = kingSquare(side);

	QVarLengthArray <Move> moves;
	if (sideToMove() == side)
		// needs symmetry of piece movement of both sides
        for (int type = Pawn; type <= Quetzal; type++)
		{
			generateMovesForPiece(moves, type, square);
			for (const auto m: moves)
			{
				if (captureType(m) == type)
					return true;
			}
			moves.clear();
		}
	else
	{
		generateMoves(moves);
		for (const auto m: moves)
		{
			if (m.targetSquare() == square)
				return true;
		}
	}
	return false;
}


} // namespace Chess
