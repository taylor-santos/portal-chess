//
// Created by taylor-santos on 4/18/2021 at 22:08.
//

#include "board.h"

#include <sstream>
#include <array>

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

bool
Piece::operator==(const Piece &other) const {
    return type == other.type && color == other.color;
}

bool
Piece::operator!=(const Piece &other) const {
    return !(*this == other);
}

std::ostream &
operator<<(std::ostream &os, const Coord &coord) {
    return os << coord.file << coord.rank;
}

class Board::InitialBoard : public Board {
public:
    explicit InitialBoard(const std::vector<std::pair<Coord, Piece>> &pieces);

    std::optional<Piece>
    at(Coord coord) const override;

private:
    class BoardState {
    public:
        explicit BoardState(std::array<std::array<std::optional<Piece>, 8>, 8> board);

        [[nodiscard]] const std::optional<Piece> &
        operator()(Coord coord) const;

    private:
        const std::array<std::array<std::optional<Piece>, 8>, 8> board_;
    } board_;
};

class Board::AddedPiece : public Board {
public:
    AddedPiece(std::shared_ptr<const Board> board, Coord coord, Piece piece);

    std::optional<Piece>
    at(Coord coord) const override;

private:
    const std::shared_ptr<const Board> board_;
    const Coord                        coord_;
    const Piece                        piece_;
};

class Board::RemovedPiece : public Board {
public:
    RemovedPiece(std::shared_ptr<const Board> board, Coord coord);

    std::optional<Piece>
    at(Coord coord) const override;

private:
    const std::shared_ptr<const Board> board_;
    const Coord                        coord_;
};

class Board::MovedPiece : public Board {
public:
    MovedPiece(std::shared_ptr<const Board> board, Coord from, Coord to);

    std::optional<Piece>
    at(Coord coord) const override;

private:
    const std::shared_ptr<const Board> board_;
    const Coord                        from_;
    const Coord                        to_;
};

std::shared_ptr<const Board>
Board::make(const std::vector<std::pair<Coord, Piece>> &pieces) {
    return std::make_unique<InitialBoard>(pieces);
}

std::shared_ptr<const Board>
Board::addPiece(Coord coord, Piece piece) const {
    return std::make_unique<AddedPiece>(shared_from_this(), coord, piece);
}

std::shared_ptr<const Board>
Board::removePiece(Coord coord) const {
    return std::make_unique<RemovedPiece>(shared_from_this(), coord);
}

std::shared_ptr<const Board>
Board::movePiece(Coord from, Coord to) const {
    return std::make_unique<MovedPiece>(shared_from_this(), from, to);
}

static std::array<std::array<std::optional<Piece>, 8>, 8>
getPieces(const std::vector<std::pair<Coord, Piece>> &pieces) {
    std::array<std::array<std::optional<Piece>, 8>, 8> board;
    for (auto [coord, piece] : pieces) {
        auto [file, rank] = coord;
        auto &prev        = board[file - 1][rank - 1];
        if (prev) {
            std::stringstream ss;
            ss << "Cannot add piece to " << coord << ": this space is occupied";
            throw invalid_piece(ss.str());
        } else {
            prev = piece;
        }
    }
    return board;
}

Board::InitialBoard::InitialBoard(const std::vector<std::pair<Coord, Piece>> &pieces)
    : board_{getPieces(pieces)} {}

std::optional<Piece>
Board::InitialBoard::at(Coord coord) const {
    return board_(coord);
}

Board::InitialBoard::BoardState::BoardState(
    std::array<std::array<std::optional<Piece>, 8>, 8> board)
    : board_{board} {}

const std::optional<Piece> &
Board::InitialBoard::BoardState::operator()(Coord coord) const {
    auto [file, rank] = coord;
    return board_[file - 1][rank - 1];
}

Board::AddedPiece::AddedPiece(std::shared_ptr<const Board> board, Coord coord, Piece piece)
    : board_{std::move(board)}
    , coord_{coord}
    , piece_{piece} {
    if (board_->at(coord)) {
        std::stringstream ss;
        ss << "Cannot add piece to " << coord << ": this space is occupied";
        throw invalid_piece(ss.str());
    }
}

std::optional<Piece>
Board::AddedPiece::at(Coord coord) const {
    return coord_ == coord ? piece_ : board_->at(coord);
}

Board::RemovedPiece::RemovedPiece(std::shared_ptr<const Board> board, Coord coord)
    : board_{std::move(board)}
    , coord_{coord} {
    if (!board_->at(coord)) {
        std::stringstream ss;
        ss << "Cannot remove piece from " << coord << ": this space is empty";
        throw invalid_piece(ss.str());
    }
}

std::optional<Piece>
Board::RemovedPiece::at(Coord coord) const {
    return coord == coord_ ? std::nullopt : board_->at(coord);
}

invalid_piece::invalid_piece(const std::string &arg)
    : std::runtime_error(arg) {}

Board::MovedPiece::MovedPiece(std::shared_ptr<const Board> board, Coord from, Coord to)
    : board_{std::move(board)}
    , from_{from}
    , to_{to} {
    if (board_->at(to_)) {
        std::stringstream ss;
        ss << "Cannot move piece to " << to_ << ": this space is occupied";
        throw invalid_piece(ss.str());
    }
    if (!board_->at(from_)) {
        std::stringstream ss;
        ss << "Cannot move piece from " << from_ << ": this space is empty";
        throw invalid_piece(ss.str());
    }
}

std::optional<Piece>
Board::MovedPiece::at(Coord coord) const {
    if (coord == to_) {
        return board_->at(from_);
    } else if (coord == from_) {
        return std::nullopt;
    } else {
        return board_->at(coord);
    }
}

} // namespace Chess
