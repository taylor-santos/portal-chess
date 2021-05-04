//
// Created by taylor-santos on 4/24/2021 at 13:36.
//

#ifndef PORTAL_CHESS_INCLUDE_PIECE_H
#define PORTAL_CHESS_INCLUDE_PIECE_H

#include <vector>

#include "coord.h"

namespace Chess {

class Board;

// Encode the direction each player faces in the value of each color.
// White moves in the positive direction, Black in the negative.
enum class Color { White = +1, Black = -1 };
enum class Type { Bishop, King, Knight, Pawn, Portal, Queen, Rook };

class Move {
public:
    Coord                to;
    std::optional<Coord> take{};
};

class Piece {
public:
    Type  type;
    Color color;

    Piece(Type type, Color color);
    Piece(const Piece &piece) = default;
    virtual ~Piece()          = default;

    [[nodiscard]] virtual std::vector<Move>
    validMoves(Coord from, const Board &board, int round) const = 0;

    bool
    operator==(const Piece &other) const;

    bool
    operator!=(const Piece &other) const;
};

std::ostream &
operator<<(std::ostream &os, const Piece &piece);

class Bishop final : public Piece {
public:
    explicit Bishop(Color color);

    [[nodiscard]] std::vector<Move>
    validMoves(Coord from, const Board &board, int round) const override;
};

class King final : public Piece {
public:
    explicit King(Color color);

    [[nodiscard]] std::vector<Move>
    validMoves(Coord from, const Board &board, int round) const override;
};

class Knight final : public Piece {
public:
    explicit Knight(Color color);

    [[nodiscard]] std::vector<Move>
    validMoves(Coord from, const Board &board, int round) const override;
};

class Pawn final : public Piece {
public:
    explicit Pawn(Color color);

    [[nodiscard]] std::vector<Move>
    validMoves(Coord from, const Board &board, int round) const override;
};

class Queen final : public Piece {
public:
    explicit Queen(Color color);

    [[nodiscard]] std::vector<Move>
    validMoves(Coord from, const Board &board, int round) const override;
};

class Rook final : public Piece {
public:
    explicit Rook(Color color);

    [[nodiscard]] std::vector<Move>
    validMoves(Coord from, const Board &board, int round) const override;
};

} // namespace Chess

#endif // PORTAL_CHESS_INCLUDE_PIECE_H
