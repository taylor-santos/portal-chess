//
// Created by taylor-santos on 4/19/2021 at 21:55.
//

#include "gtest/gtest.h"
#include "board.h"

#include <stdexcept>

#include "coord.h"
#include "piece.h"

using namespace Chess;

TEST(Board, GetPieceShouldGetInitialPiece) {
    Coord coord{A, _1};
    Piece piece{Type::Bishop, Color::Black};

    std::vector<std::pair<Coord, incomplete_ptr<Piece>>> pieces;
    pieces.emplace_back(coord, std::make_unique<Piece>(piece));
    auto board    = Board::make(std::move(pieces));
    auto optPiece = board->at(coord);
    ASSERT_TRUE(optPiece);
    EXPECT_EQ(piece, **optPiece);
}

TEST(Board, GetPieceShouldGetAddedPiece) {
    auto  board = Board::make({});
    Coord coord{A, _1};
    Piece piece{Type::Bishop, Color::Black};
    board         = board->addPiece(coord, std::make_unique<Piece>(piece));
    auto optPiece = board->at(coord);
    ASSERT_TRUE(optPiece);
    EXPECT_EQ(piece, **optPiece);
}

TEST(Board, AddPieceOnOccupiedSpaceShouldThrowInvalidPiece) {
    Coord coord{A, _1};
    Piece piece1{Type::Bishop, Color::Black};
    Piece piece2{Type::Queen, Color::White};

    std::vector<std::pair<Coord, incomplete_ptr<Piece>>> pieces;
    pieces.emplace_back(coord, std::make_unique<Piece>(piece1));
    auto board = Board::make(std::move(pieces));
    EXPECT_THROW((void)board->addPiece(coord, std::make_unique<Piece>(piece2)), invalid_piece);
}

TEST(Board, RemovePieceShouldRemovePiece) {
    Coord coord{A, _1};
    Piece piece{Type::Bishop, Color::Black};

    std::vector<std::pair<Coord, incomplete_ptr<Piece>>> pieces;
    pieces.emplace_back(coord, std::make_unique<Piece>(piece));
    auto board = Board::make(std::move(pieces));
    board      = board->removePiece(coord);
    {
        auto optPiece = board->at(coord);
        EXPECT_FALSE(optPiece);
    }
}

TEST(Board, MovePieceFromUnoccupiedShouldThrowInvalidPiece) {
    auto board = Board::make({});
    ASSERT_THROW((void)board->movePiece(Coord{A, _1}, Coord{B, _2}), invalid_piece);
}

TEST(Board, MovePieceToOccupiedShouldThrowInvalidPiece) {
    Coord from{A, _1};
    Coord to{B, _2};
    Piece piece1{Type::Rook, Color::White}, piece2 = {Type::Pawn, Color::Black};

    std::vector<std::pair<Coord, incomplete_ptr<Piece>>> pieces;
    pieces.emplace_back(from, std::make_unique<Piece>(piece1));
    pieces.emplace_back(to, std::make_unique<Piece>(piece2));
    auto board = Board::make(std::move(pieces));
    ASSERT_THROW((void)board->movePiece(from, to), invalid_piece);
}

TEST(Board, MovePieceShouldMovePiece) {
    Coord from{A, _1};
    Coord to{B, _2};
    Piece piece{Type::Rook, Color::White};

    std::vector<std::pair<Coord, incomplete_ptr<Piece>>> pieces;
    pieces.emplace_back(from, std::make_unique<Piece>(piece));
    auto board = Board::make(std::move(pieces));
    ASSERT_NO_THROW(board = board->movePiece(from, to));
    auto oldPiece = board->at(from);
    EXPECT_FALSE(oldPiece);
    auto newPiece = board->at(to);
    EXPECT_TRUE(newPiece);
    EXPECT_EQ(piece, **newPiece);
}

TEST(Board, MakeShouldThrowOnOverlappingPieces) {
    Coord coord{A, _1};
    Piece piece1{Type::King, Color::White};
    Piece piece2{Type::Queen, Color::White};

    std::vector<std::pair<Coord, incomplete_ptr<Piece>>> pieces;
    pieces.emplace_back(coord, std::make_unique<Piece>(piece1));
    pieces.emplace_back(coord, std::make_unique<Piece>(piece2));
    ASSERT_THROW((void)Board::make(std::move(pieces)), invalid_piece);
}

TEST(Board, RemovePieceShouldThrowIfUnoccupied) {
    auto board = Board::make({});
    ASSERT_THROW((void)board->removePiece({A, _1}), invalid_piece);
}

TEST(Board, MovingPieceShouldNotAffectOtherPieces) {
    Coord coord1{A, _1}, coord2{B, _2};
    Piece piece1{Type::Bishop, Color::White};
    Piece piece2{Type::Knight, Color::White};

    std::vector<std::pair<Coord, incomplete_ptr<Piece>>> pieces;
    pieces.emplace_back(coord1, std::make_unique<Piece>(piece1));
    pieces.emplace_back(coord2, std::make_unique<Piece>(piece2));
    auto board    = Board::make(std::move(pieces));
    board         = board->movePiece(coord1, {A, _2});
    auto optPiece = board->at(coord2);

    ASSERT_TRUE(optPiece);
    EXPECT_EQ(**optPiece, piece2);
}

TEST(Board, AddPieceDoesNotMutateBoard) {
    auto  board1 = Board::make({});
    Coord coord{A, _1};

    EXPECT_FALSE(board1->at(coord));

    {
        Piece piece{Type::Rook, Color::Black};
        auto  board2 = board1->addPiece(coord, std::make_unique<Piece>(piece));
        EXPECT_TRUE(board2->at(coord));
    }

    EXPECT_FALSE(board1->at(coord));
}

TEST(Board, RemovePieceDoesNotMutateBoard) {
    Coord coord{A, _1};
    Piece piece{Type::Rook, Color::Black};

    std::vector<std::pair<Coord, incomplete_ptr<Piece>>> pieces;
    pieces.emplace_back(coord, std::make_unique<Piece>(piece));
    auto board1 = Board::make(std::move(pieces));

    EXPECT_TRUE(board1->at(coord));

    {
        auto board2 = board1->removePiece(coord);
        EXPECT_FALSE(board2->at(coord));
    }

    EXPECT_TRUE(board1->at(coord));
}

TEST(Board, MovePieceDoesNotMutateBoard) {
    Coord coord1{A, _1};
    Coord coord2{B, _2};
    Piece piece{Type::Pawn, Color::Black};

    std::vector<std::pair<Coord, incomplete_ptr<Piece>>> pieces;
    pieces.emplace_back(coord1, std::make_unique<Piece>(piece));
    auto board1 = Board::make(std::move(pieces));

    EXPECT_TRUE(board1->at(coord1));
    EXPECT_FALSE(board1->at(coord2));

    {
        auto board2 = board1->movePiece(coord1, coord2);
        EXPECT_FALSE(board2->at(coord1));
        EXPECT_TRUE(board2->at(coord2));
    }

    EXPECT_TRUE(board1->at(coord1));
    EXPECT_FALSE(board1->at(coord2));
}
