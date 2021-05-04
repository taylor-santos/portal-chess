//
// Created by taylor-santos on 4/18/2021 at 22:08.
//

#include "board.h"

#include <sstream>
#include <array>

#include "coord.h"

namespace Chess {

template<typename T, int N>
using sqr_array = std::array<std::array<T, N>, N>;

class Board::InitialBoard : public Board {
public:
    InitialBoard(std::vector<std::pair<Coord, incomplete_ptr<Piece>>> &pieces, int round);

    [[nodiscard]] std::optional<const Piece *>
    at(Coord coord) const override;

private:
    std::list<std::pair<int, Coord>>
    moveHistory(Coord coord, std::list<std::pair<int, Coord>> &&moves) const override;

    class BoardState {
    public:
        BoardState(sqr_array<incomplete_ptr<Piece>, 8> board);

        [[nodiscard]] const incomplete_ptr<Piece> &
        operator()(Coord coord) const;

    private:
        const sqr_array<incomplete_ptr<Piece>, 8> board_;
    } board_;
};

class Board::AddedPiece : public Board {
public:
    AddedPiece(
        std::shared_ptr<const Board> board,
        Coord                        coord,
        incomplete_ptr<Piece>        piece,
        int                          round);

    [[nodiscard]] std::optional<const Piece *>
    at(Coord coord) const override;

private:
    std::list<std::pair<int, Coord>>
    moveHistory(Coord coord, std::list<std::pair<int, Coord>> &&moves) const override;

    const std::shared_ptr<const Board> board_;
    const Coord                        coord_;
    const incomplete_ptr<Piece>        piece_;
};

class Board::RemovedPiece : public Board {
public:
    RemovedPiece(std::shared_ptr<const Board> board, Coord coord, int round);

    [[nodiscard]] std::optional<const Piece *>
    at(Coord coord) const override;

private:
    std::list<std::pair<int, Coord>>
    moveHistory(Coord coord, std::list<std::pair<int, Coord>> &&moves) const override;

    const std::shared_ptr<const Board> board_;
    const Coord                        coord_;
};

class Board::MovedPiece : public Board {
public:
    MovedPiece(std::shared_ptr<const Board> board, Coord from, Coord to, int round);

    [[nodiscard]] std::optional<const Piece *>
    at(Coord coord) const override;

    std::list<std::pair<int, Coord>>
    moveHistory(Coord coord, std::list<std::pair<int, Coord>> &&moves) const override;

private:
    const std::shared_ptr<const Board> board_;
    const Coord                        from_;
    const Coord                        to_;
};

std::shared_ptr<const Board>
Board::make(std::vector<std::pair<Coord, incomplete_ptr<Piece>>> pieces, int round) {
    auto ptr   = std::make_shared<InitialBoard>(pieces, round);
    ptr->wptr_ = ptr;
    return ptr;
}

std::shared_ptr<const Board>
Board::addPiece(Coord coord, incomplete_ptr<Piece> piece, int round) const {
    auto ptr   = std::make_shared<AddedPiece>(wptr_.lock(), coord, std::move(piece), round);
    ptr->wptr_ = ptr;
    return ptr;
}

std::shared_ptr<const Board>
Board::removePiece(Coord coord, int round) const {
    auto ptr   = std::make_shared<RemovedPiece>(wptr_.lock(), coord, round);
    ptr->wptr_ = ptr;
    return ptr;
}

std::shared_ptr<const Board>
Board::movePiece(Coord from, Coord to, int round) const {
    auto ptr   = std::make_shared<MovedPiece>(wptr_.lock(), from, to, round);
    ptr->wptr_ = ptr;
    return ptr;
}

std::list<std::pair<int, Coord>>
Board::getMoveHistory(Coord coord) const {
    return moveHistory(coord, {});
}

Board::Board(int round)
    : round_{round} {}

static sqr_array<incomplete_ptr<Piece>, 8>
getPieces(std::vector<std::pair<Coord, incomplete_ptr<Piece>>> &pieces) {
    sqr_array<incomplete_ptr<Piece>, 8> board;
    for (auto &[coord, piece] : pieces) {
        auto [file, rank] = coord;
        auto &prev        = board[file - 1][rank - 1];
        if (prev) {
            std::stringstream ss;
            ss << "Cannot add piece to " << coord << ": this space is occupied";
            throw invalid_piece(ss.str());
        } else {
            prev = std::move(piece);
        }
    }
    return board;
}

Board::InitialBoard::InitialBoard(
    std::vector<std::pair<Coord, incomplete_ptr<Piece>>> &pieces,
    int                                                   round)
    : Board(round)
    , board_{getPieces(pieces)} {}

std::optional<const Piece *>
Board::InitialBoard::at(Coord coord) const {
    auto &optPiece = board_(coord);
    return optPiece ? std::optional(optPiece.get()) : std::nullopt;
}

std::list<std::pair<int, Coord>>
Board::InitialBoard::moveHistory(Coord coord, std::list<std::pair<int, Coord>> &&moves) const {
    if (!board_(coord)) {
        return {};
    }
    moves.push_front({round_, coord});
    return std::move(moves);
}

Board::InitialBoard::BoardState::BoardState(sqr_array<incomplete_ptr<Piece>, 8> board)
    : board_{std::move(board)} {}

const incomplete_ptr<Piece> &
Board::InitialBoard::BoardState::operator()(Coord coord) const {
    auto [file, rank] = coord;
    return board_[file - 1][rank - 1];
}

Board::AddedPiece::AddedPiece(
    std::shared_ptr<const Board> board,
    Coord                        coord,
    incomplete_ptr<Piece>        piece,
    int                          round)
    : Board(round)
    , board_{std::move(board)}
    , coord_{coord}
    , piece_{std::move(piece)} {
    if (board_->at(coord)) {
        std::stringstream ss;
        ss << "Cannot add piece to " << coord << ": this space is occupied";
        throw invalid_piece(ss.str());
    }
}

std::optional<const Piece *>
Board::AddedPiece::at(Coord coord) const {
    return coord_ == coord ? piece_.get() : board_->at(coord);
}
std::list<std::pair<int, Coord>>
Board::AddedPiece::moveHistory(Coord coord, std::list<std::pair<int, Coord>> &&moves) const {
    if (coord == coord_) {
        moves.push_front({round_, coord});
        return std::move(moves);
    }
    return board_->moveHistory(coord, std::move(moves));
}

Board::RemovedPiece::RemovedPiece(std::shared_ptr<const Board> board, Coord coord, int round)
    : Board(round)
    , board_{std::move(board)}
    , coord_{coord} {
    if (!board_->at(coord)) {
        std::stringstream ss;
        ss << "Cannot remove piece from " << coord << ": this space is empty";
        throw invalid_piece(ss.str());
    }
}

std::optional<const Piece *>
Board::RemovedPiece::at(Coord coord) const {
    return coord == coord_ ? std::nullopt : board_->at(coord);
}
std::list<std::pair<int, Coord>>
Board::RemovedPiece::moveHistory(Coord coord, std::list<std::pair<int, Coord>> &&moves) const {
    if (coord_ == coord) {
        return {};
    } else {
        return board_->moveHistory(coord, std::move(moves));
    }
}

Board::MovedPiece::MovedPiece(std::shared_ptr<const Board> board, Coord from, Coord to, int round)
    : Board(round)
    , board_{std::move(board)}
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

std::optional<const Piece *>
Board::MovedPiece::at(Coord coord) const {
    if (coord == to_) {
        return board_->at(from_);
    } else if (coord == from_) {
        return std::nullopt;
    } else {
        return board_->at(coord);
    }
}
std::list<std::pair<int, Coord>>
Board::MovedPiece::moveHistory(Coord coord, std::list<std::pair<int, Coord>> &&moves) const {
    if (to_ == coord) {
        moves.push_front({round_, coord});
        return board_->moveHistory(from_, std::move(moves));
    } else {
        return board_->moveHistory(coord, std::move(moves));
    }
}

std::ostream &
operator<<(std::ostream &os, const Piece &piece);

std::ostream &
operator<<(std::ostream &os, const Board &board) {
    for (auto rank = std::rbegin(ranks); rank != std::rend(ranks); rank++) {
        os << *rank;
        for (auto file : files) {
            Coord coord{file, *rank};
            auto  piece = board.at(coord);
            if (piece) {
                os << " " << **piece;
            } else {
                os << "  ";
            }
        }
        os << std::endl;
    }
    os << " ";
    for (auto file : files) {
        os << " " << file;
    }
    os << std::endl;

    return os;
}

invalid_piece::invalid_piece(const std::string &arg)
    : std::runtime_error(arg) {}

} // namespace Chess
