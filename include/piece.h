//
// Created by taylor-santos on 4/24/2021 at 13:36.
//

#ifndef PORTAL_CHESS_INCLUDE_PIECE_H
#define PORTAL_CHESS_INCLUDE_PIECE_H

namespace Chess {

enum class Color { White, Black };
enum class Type { Bishop, King, Knight, Pawn, Portal, Queen, Rook };

class Piece {
public:
    Type  type;
    Color color;

    Piece(Type type, Color color);
    Piece(const Piece &piece) = default;

    bool
    operator==(const Piece &other) const;

    bool
    operator!=(const Piece &other) const;
};

} // namespace Chess

#endif // PORTAL_CHESS_INCLUDE_PIECE_H
