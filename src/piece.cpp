//
// Created by taylor-santos on 4/24/2021 at 13:45.
//

#include "piece.h"

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

} // namespace Chess
