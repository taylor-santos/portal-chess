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
    auto  piece    = std::make_shared<Piece>(Type::Bishop, Color::Black);
    auto  board    = Board::make({{coord, piece}});
    auto  optPiece = board->at(coord);
    ASSERT_TRUE(optPiece);
    EXPECT_EQ(*piece, **optPiece);
}

TEST(Board, GetPieceShouldGetAddedPiece) {
    auto  board = Board::make({});
    Coord coord{A, _1};
    auto  piece   = std::make_shared<Piece>(Type::Bishop, Color::Black);
    board         = board->addPiece(coord, piece);
    auto optPiece = board->at(coord);
    ASSERT_TRUE(optPiece);
    EXPECT_EQ(*piece, **optPiece);
}

TEST(Board, AddPieceOnOccupiedSpaceShouldThrowInvalidPiece) {
    Coord coord{A, _1};
    auto  piece1 = std::make_shared<Piece>(Type::Bishop, Color::Black);
    auto  piece2 = std::make_shared<Piece>(Type::Queen, Color::White);
    auto  board  = Board::make({{coord, piece1}});
    EXPECT_THROW((void)board->addPiece(coord, piece2), invalid_piece);
}

TEST(Board, RemovePieceShouldRemovePiece) {
    Coord coord{A, _1};
    auto  piece = std::make_shared<Piece>(Type::Bishop, Color::Black);
    auto  board = Board::make({{coord, piece}});
    board       = board->removePiece(coord);
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
    auto  piece1 = std::make_shared<Piece>(Type::Rook, Color::White),
         piece2  = std::make_shared<Piece>(Type::Pawn, Color::Black);
    auto board   = Board::make({{from, piece1}, {to, piece2}});
    ASSERT_THROW((void)board->movePiece(from, to), invalid_piece);
}

TEST(Board, MovePieceShouldMovePiece) {
    Coord from{A, _1};
    Coord to{B, _2};
    auto  piece = std::make_shared<Piece>(Type::Rook, Color::White);
    auto  board = Board::make({{from, piece}});
    ASSERT_NO_THROW(board = board->movePiece(from, to));
    auto oldPiece = board->at(from);
    EXPECT_FALSE(oldPiece);
    auto newPiece = board->at(to);
    EXPECT_TRUE(newPiece);
    EXPECT_EQ(*piece, **newPiece);
}

TEST(Board, MakeShouldThrowOnOverlappingPieces) {
    Coord coord{A, _1};
    auto  piece1 = std::make_shared<Piece>(Type::King, Color::White);
    auto  piece2 = std::make_shared<Piece>(Type::Queen, Color::White);
    ASSERT_THROW((void)Board::make({{coord, piece1}, {coord, piece2}}), invalid_piece);
}

TEST(Board, RemovePieceShouldThrowIfUnoccupied) {
    auto board = Board::make({});
    ASSERT_THROW((void)board->removePiece({A, _1}), invalid_piece);
}

TEST(Board, MovingPieceShouldNotAffectOtherPieces) {
    Coord coord1{A, _1}, coord2{B, _2};
    auto  piece1  = std::make_shared<Piece>(Type::Bishop, Color::White),
         piece2   = std::make_shared<Piece>(Type::Knight, Color::White);
    auto board    = Board::make({{coord1, piece1}, {coord2, piece2}});
    board         = board->movePiece(coord1, {A, _2});
    auto optPiece = board->at(coord2);

    ASSERT_TRUE(optPiece);
    EXPECT_EQ(**optPiece, *piece2);
}

TEST(Board, AddPieceDoesNotMutateBoard) {
    auto  board1 = Board::make({});
    Coord coord{A, _1};

    EXPECT_FALSE(board1->at(coord));

    {
        auto piece  = std::make_shared<Piece>(Type::Rook, Color::Black);
        auto board2 = board1->addPiece(coord, piece);
        EXPECT_TRUE(board2->at(coord));
    }

    EXPECT_FALSE(board1->at(coord));
}

TEST(Board, RemovePieceDoesNotMutateBoard) {
    Coord coord{A, _1};
    auto  piece  = std::make_shared<Piece>(Type::Rook, Color::Black);
    auto  board1 = Board::make({{coord, piece}});

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
    auto  piece  = std::make_shared<Piece>(Type::Pawn, Color::Black);
    auto  board1 = Board::make({{coord1, piece}});

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
