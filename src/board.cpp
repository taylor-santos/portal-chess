//
// Created by taylor-santos on 4/18/2021 at 22:08.
//

#include "board.h"

#include <sstream>
#include <array>

#include "coord.h"

namespace Chess {

class Board::InitialBoard : public Board {
public:
    explicit InitialBoard(const std::vector<std::pair<Coord, std::shared_ptr<Piece>>> &pieces);

    [[nodiscard]] std::optional<const Piece *>
    at(Coord coord) const override;

private:
    class BoardState {
    public:
        explicit BoardState(std::array<std::array<std::shared_ptr<Piece>, 8>, 8> board);

        [[nodiscard]] const std::shared_ptr<Piece> &
        operator()(Coord coord) const;

    private:
        const std::array<std::array<std::shared_ptr<Piece>, 8>, 8> board_;
    } board_;
};

class Board::AddedPiece : public Board {
public:
    AddedPiece(std::shared_ptr<const Board> board, Coord coord, std::shared_ptr<Piece> piece);

    [[nodiscard]] std::optional<const Piece *>
    at(Coord coord) const override;

private:
    const std::shared_ptr<const Board> board_;
    const Coord                        coord_;
    const std::shared_ptr<Piece>       piece_;
};

class Board::RemovedPiece : public Board {
public:
    RemovedPiece(std::shared_ptr<const Board> board, Coord coord);

    [[nodiscard]] std::optional<const Piece *>
    at(Coord coord) const override;

private:
    const std::shared_ptr<const Board> board_;
    const Coord                        coord_;
};

class Board::MovedPiece : public Board {
public:
    MovedPiece(std::shared_ptr<const Board> board, Coord from, Coord to);

    [[nodiscard]] std::optional<const Piece *>
    at(Coord coord) const override;

private:
    const std::shared_ptr<const Board> board_;
    const Coord                        from_;
    const Coord                        to_;
};

std::shared_ptr<const Board>
Board::make(const std::vector<std::pair<Coord, std::shared_ptr<Piece>>> &pieces) {
    auto ptr   = std::make_shared<InitialBoard>(pieces);
    ptr->wptr_ = ptr;
    return ptr;
}

std::shared_ptr<const Board>
Board::addPiece(Coord coord, std::shared_ptr<Piece> piece) const {
    auto ptr   = std::make_shared<AddedPiece>(wptr_.lock(), coord, std::move(piece));
    ptr->wptr_ = ptr;
    return ptr;
}

std::shared_ptr<const Board>
Board::removePiece(Coord coord) const {
    auto ptr   = std::make_shared<RemovedPiece>(wptr_.lock(), coord);
    ptr->wptr_ = ptr;
    return ptr;
}

std::shared_ptr<const Board>
Board::movePiece(Coord from, Coord to) const {
    auto ptr   = std::make_shared<MovedPiece>(wptr_.lock(), from, to);
    ptr->wptr_ = ptr;
    return ptr;
}

static std::array<std::array<std::shared_ptr<Piece>, 8>, 8>
getPieces(const std::vector<std::pair<Coord, std::shared_ptr<Piece>>> &pieces) {
    std::array<std::array<std::shared_ptr<Piece>, 8>, 8> board;
    for (auto &[coord, piece] : pieces) {
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

Board::InitialBoard::InitialBoard(
    const std::vector<std::pair<Coord, std::shared_ptr<Piece>>> &pieces)
    : board_{getPieces(pieces)} {}

std::optional<const Piece *>
Board::InitialBoard::at(Coord coord) const {
    auto &optPiece = board_(coord);
    return optPiece ? std::optional(optPiece.get()) : std::nullopt;
}

Board::InitialBoard::BoardState::BoardState(
    std::array<std::array<std::shared_ptr<Piece>, 8>, 8> board)
    : board_{std::move(board)} {}

const std::shared_ptr<Piece> &
Board::InitialBoard::BoardState::operator()(Coord coord) const {
    auto [file, rank] = coord;
    return board_[file - 1][rank - 1];
}

Board::AddedPiece::AddedPiece(
    std::shared_ptr<const Board> board,
    Coord                        coord,
    std::shared_ptr<Piece>       piece)
    : board_{std::move(board)}
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

Board::RemovedPiece::RemovedPiece(std::shared_ptr<const Board> board, Coord coord)
    : board_{std::move(board)}
    , coord_{coord} {
    if (!board_->at(coord)) {
        std::stringstream ss;
        ss << "Cannot remove piece from " << coord << ": this space is empty";
        throw invalid_piece(ss.str());
    }
}

std::optional<const Piece *>
Board::RemovedPiece::at(Coord coord) const {
    std::optional<std::reference_wrapper<Piece>> optRef;
    return coord == coord_ ? std::nullopt : board_->at(coord);
}

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

invalid_piece::invalid_piece(const std::string &arg)
    : std::runtime_error(arg) {}

} // namespace Chess
