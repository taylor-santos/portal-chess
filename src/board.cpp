//
// Created by taylor-santos on 4/18/2021 at 22:08.
//

#include "board.h"

#include <algorithm>
#include <sstream>

namespace Chess {

std::ostream &
operator<<(std::ostream &os, File file) {
    int fileInt = file;
    if (file < A || H < file) {
        return os << fileInt;
    }
    return os << static_cast<char>('A' + fileInt - 1);
}

Coord::Coord(File file, Rank rank)
    : file{file}
    , rank{rank} {
    if (file < A || H < file || rank < _1 || _8 < rank) {
        std::stringstream ss;
        ss << "Coord constructor called with invalid coordinate (" << file << ", " << rank << ")";
        throw std::invalid_argument(ss.str());
    }
}

bool
Coord::operator==(const Coord &other) const {
    return file == other.file && rank == other.rank;
}

bool
Coord::operator!=(const Coord &other) const {
    return !(*this == other);
}

std::ostream &
operator<<(std::ostream &os, const Coord &coord) {
    return os << coord.file << coord.rank;
}

Board::Board() = default;

Board::~Board() = default;

std::optional<Piece>
Board::getPiece(Coord coord) const {
    return board_(coord);
}

void
Board::addPiece(Piece piece) {
    if (getPiece(piece.coord)) {
        std::stringstream ss;
        ss << "Cannot add piece to " << piece.coord << ": this space is occupied";
        throw invalid_piece(ss.str());
    }
    board_(piece.coord) = piece;
}

void
Board::removePiece(Coord coord) {
    board_(coord).reset();
}

void
Board::movePiece(Coord from, Coord to) {
    auto &optPiece = board_(from);
    if (!optPiece) {
        std::stringstream ss;
        ss << "Cannot move piece from " << from << ": this space is empty";
        throw invalid_piece(ss.str());
    }
    Piece newPiece = *optPiece;
    newPiece.coord = to;
    addPiece(newPiece);
    optPiece.reset();
}

invalid_piece::invalid_piece(const std::string &arg)
    : std::runtime_error(arg) {}

bool
Piece::operator==(const Piece &other) const {
    return type == other.type && color == other.color && coord == other.coord;
}

bool
Piece::operator!=(const Piece &other) const {
    return !(*this == other);
}

const std::optional<Piece> &
Board::BoardState::operator()(Coord coord) const {
    auto [file, rank] = coord;
    return board[file - 1][rank - 1];
}

std::optional<Piece> &
Board::BoardState::operator()(Coord coord) {
    auto [file, rank] = coord;
    return board[file - 1][rank - 1];
}

} // namespace Chess
