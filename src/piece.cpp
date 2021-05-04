//
// Created by taylor-santos on 4/24/2021 at 13:45.
//

#include "piece.h"

#include <vector>
#include <optional>

#include "board.h"

namespace Chess {

Piece::Piece(Type type, Color color)
    : type{type}
    , color{color} {}

bool
Piece::operator==(const Piece &other) const {
    return type == other.type && color == other.color;
}

bool
Piece::operator!=(const Piece &other) const {
    return !(*this == other);
}

/***
 * Check if moving a piece to a given coordinate is considered a move or a capture, then add it to
 * the vector of moves. Return true if further movement would be blocked.
 * @param moves
 * @param color
 * @param to
 * @param board
 * @return
 */

static bool
addTake(std::vector<Move> &moves, Color color, Coord to, const Board &board) {
    auto optPiece = board.at(to);
    if (optPiece) {
        if ((*optPiece)->color != color) {
            moves.push_back({to, to});
        }
        return true;
    }
    return false;
}

static bool
addMove(std::vector<Move> &moves, Color color, Coord to, const Board &board) {
    if (addTake(moves, color, to, board)) {
        return true;
    }
    moves.push_back({to});
    return false;
}

std::ostream &
operator<<(std::ostream &os, const Piece &piece) {
    static const std::string whiteNames[] = {"B", "K", "N", "P", "O", "Q", "R"};
    static const std::string blackNames[] = {"b", "k", "n", "p", "o", "Q", "r"};
    auto name = piece.color == Color::White ? whiteNames[static_cast<int>(piece.type)]
                                            : blackNames[static_cast<int>(piece.type)];
    os << name;
    return os;
}

Bishop::Bishop(Color color)
    : Piece(Type::Bishop, color) {}

std::vector<Move>
Bishop::validMoves(Coord from, const Board &board, int) const {
    static const std::vector<std::pair<int, int>> deltas{{1, 1}, {1, -1}, {-1, 1}, {-1, -1}};

    std::vector<Move> moves;
    for (auto &delta : deltas) {
        auto optTo = from.offset(delta);
        for (; optTo; optTo = optTo->offset(delta)) {
            if (addMove(moves, color, *optTo, board)) {
                break;
            }
        }
    }

    return moves;
}

King::King(Color color)
    : Piece(Type::King, color) {}

std::vector<Move>
King::validMoves(Coord from, const Board &board, int) const {
    // TODO: Add castling
    static const std::vector<std::pair<int, int>>
        deltas{{0, 1}, {1, 1}, {1, 0}, {1, -1}, {0, -1}, {-1, -1}, {-1, 0}, {-1, 1}};

    std::vector<Move> moves;
    for (auto &delta : deltas) {
        auto optTo = from.offset(delta);
        if (optTo) {
            addMove(moves, color, *optTo, board);
        }
    }

    return moves;
}

Knight::Knight(Color color)
    : Piece(Type::Knight, color) {}

std::vector<Move>
Knight::validMoves(Coord from, const Board &board, int) const {
    static const std::vector<std::pair<int, int>>
        deltas{{1, 2}, {2, 1}, {2, -1}, {1, -2}, {-1, -2}, {-2, -1}, {-2, 1}, {-1, 2}};

    std::vector<Move> moves;
    for (auto &delta : deltas) {
        auto optTo = from.offset(delta);
        if (optTo) {
            addMove(moves, color, *optTo, board);
        }
    }
    return moves;
}

Pawn::Pawn(Color color)
    : Piece(Type::Pawn, color) {}

std::vector<Move>
Pawn::validMoves(Coord from, const Board &board, int round) const {
    std::vector<Move> moves;
    // TODO: Add promotion

    // Forward movement
    auto offset1 = from.offset(0, static_cast<int>(color));
    if (offset1 && !board.at(*offset1)) {
        moves.push_back({*offset1});
        // Allow double spaces if on first move
        auto pawnRank = color == Color::White ? _2 : _7;
        if (from.rank == pawnRank) {
            auto offset2 = *from.offset(0, 2 * static_cast<int>(color));
            if (!board.at(offset2)) {
                moves.push_back({offset2});
            }
        }
    }

    // Take on diagonals
    for (auto side : {-1, 1}) {
        auto offset = from.offset(side, static_cast<int>(color));
        if (!offset) continue;

        auto piece = board.at(*offset);
        if (piece && (*piece)->color != color) {
            moves.push_back({*offset, *offset});
        }

        // En Passant
        // Pawn can only en passent from the 5th rank as white, or 4th rank as black
        auto enPassFromRank = color == Color::White ? _5 : _4;
        if (from.rank != enPassFromRank) continue;
        // Target pawn must now be directly adjacent to attacking pawn
        auto enPassCoord = from.offset(side, 0);
        if (!enPassCoord) continue;
        // Get target piece
        auto enPassPiece = board.at(*enPassCoord);
        if (!enPassPiece) continue;
        // Target piece must be an enemy pawn
        if ((*enPassPiece)->color == color || (*enPassPiece)->type != Type::Pawn) continue;
        auto moveHist = board.getMoveHistory(*enPassCoord);
        // Target piece must have moved exactly once
        if (moveHist.size() != 2) continue;
        auto moveIt = moveHist.rbegin();
        // Target pawn's last move must have been on the previous round
        if (moveIt->first != round - 1) continue;
        moveIt++;
        auto targetRank = color == Color::White ? _7 : _2;
        // Target pawn's last move must have originated on the pawn rank
        if (moveIt->second.rank != targetRank) continue;
        moves.push_back({*offset, *enPassCoord});
    }

    return moves;
}

Queen::Queen(Color color)
    : Piece(Type::Queen, color) {}

std::vector<Move>
Queen::validMoves(Coord from, const Board &board, int) const {
    static const std::vector<std::pair<int, int>>
        deltas{{0, 1}, {1, 1}, {1, 0}, {1, -1}, {0, -1}, {-1, -1}, {-1, 0}, {-1, 1}};

    std::vector<Move> moves;
    for (auto &delta : deltas) {
        auto optTo = from.offset(delta);
        for (; optTo; optTo = optTo->offset(delta)) {
            if (addMove(moves, color, *optTo, board)) {
                break;
            }
        }
    }

    return moves;
}

Rook::Rook(Color color)
    : Piece(Type::Rook, color) {}

std::vector<Move>
Rook::validMoves(Coord from, const Board &board, int) const {
    // TODO: Add castling
    static const std::vector<std::pair<int, int>> deltas{{0, 1}, {1, 0}, {0, -1}, {-1, 0}};

    std::vector<Move> moves;
    for (auto &delta : deltas) {
        auto optTo = from.offset(delta);
        for (; optTo; optTo = optTo->offset(delta)) {
            if (addMove(moves, color, *optTo, board)) {
                break;
            }
        }
    }

    return moves;
}

} // namespace Chess
