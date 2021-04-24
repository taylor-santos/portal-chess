//
// Created by taylor-santos on 4/19/2021 at 21:55.
//

#include "gtest/gtest.h"
#include "board.h"

#include <stdexcept>
#include <sstream>

using namespace Chess;

TEST(File, StreamOperatorPrintsFileLetter) {
    std::stringstream ss;
    ss << A << B << C << D << E << F << G << H;
    EXPECT_EQ(ss.str(), "ABCDEFGH");
}

TEST(File, StreamOperatorPrintsInvalidFileAsNumber) {
    std::stringstream ss;
    ss << static_cast<File>(0) << " " << static_cast<File>(9);
    EXPECT_EQ(ss.str(), "0 9");
}

TEST(Coord, ThrowsInvalidArgumentWhenFileIsOutOfRange) {
    EXPECT_THROW(Coord(static_cast<File>(0), _1), std::invalid_argument);
    EXPECT_THROW(Coord(static_cast<File>(9), _1), std::invalid_argument);
    EXPECT_THROW(Coord(static_cast<File>(-1), _1), std::invalid_argument);
    EXPECT_THROW(Coord(std::numeric_limits<File>::min(), _1), std::invalid_argument);
    EXPECT_THROW(Coord(std::numeric_limits<File>::max(), _1), std::invalid_argument);
}

TEST(Coord, ThrowsInvalidArgumentWhenRankIsOutOfRange) {
    EXPECT_THROW(Coord(A, static_cast<Rank>(0)), std::invalid_argument);
    EXPECT_THROW(Coord(A, static_cast<Rank>(9)), std::invalid_argument);
    EXPECT_THROW(Coord(A, static_cast<Rank>(-1)), std::invalid_argument);
    EXPECT_THROW(Coord(A, std::numeric_limits<Rank>::min()), std::invalid_argument);
    EXPECT_THROW(Coord(A, std::numeric_limits<Rank>::max()), std::invalid_argument);
}

TEST(Coord, DoesNotThrowWhenFileIsInRange) {
    EXPECT_NO_THROW(Coord(A, _1));
    EXPECT_NO_THROW(Coord(B, _1));
    EXPECT_NO_THROW(Coord(C, _1));
    EXPECT_NO_THROW(Coord(D, _1));
    EXPECT_NO_THROW(Coord(E, _1));
    EXPECT_NO_THROW(Coord(F, _1));
    EXPECT_NO_THROW(Coord(G, _1));
    EXPECT_NO_THROW(Coord(H, _1));
}

TEST(Coord, DoesNotThrowWhenRankIsInRange) {
    EXPECT_NO_THROW(Coord(A, _1));
    EXPECT_NO_THROW(Coord(A, _2));
    EXPECT_NO_THROW(Coord(A, _3));
    EXPECT_NO_THROW(Coord(A, _4));
    EXPECT_NO_THROW(Coord(A, _5));
    EXPECT_NO_THROW(Coord(A, _6));
    EXPECT_NO_THROW(Coord(A, _7));
    EXPECT_NO_THROW(Coord(A, _8));
}

TEST(Coord, StreamOperatorPrintsFormattedCoord) {
    std::stringstream ss;
    Coord             c{C, _4};
    ss << c;
    EXPECT_EQ(ss.str(), "C4");
}

TEST(Coord, NotEqualIfFilesDiffer) {
    Coord a{A, _1};
    Coord b{B, _1};
    ASSERT_NE(a, b);
}

TEST(Coord, NotEqualIfRanksDiffer) {
    Coord a{A, _1};
    Coord b{A, _2};
    ASSERT_NE(a, b);
}

TEST(Type, NotEqualIfTypesDiffer) {
    Color color = Color::White;
    Piece a{Type::King, color};
    Piece b{Type::Queen, color};
    ASSERT_NE(a, b);
}

TEST(Type, NotEqualIfColorsDiffer) {
    Type  type = Type::Bishop;
    Piece a{type, Color::White};
    Piece b{type, Color::Black};
    ASSERT_NE(a, b);
}

TEST(Board, GetPieceShouldGetInitialPiece) {
    Coord coord{A, _1};
    Piece piece{Type::Bishop, Color::Black};
    auto  board    = Board::make({{coord, piece}});
    auto  optPiece = board->at(coord);
    ASSERT_TRUE(optPiece);
    EXPECT_EQ(piece, *optPiece);
}

TEST(Board, GetPieceShouldGetAddedPiece) {
    auto  board = Board::make({});
    Coord coord{A, _1};
    Piece piece{Type::Bishop, Color::Black};
    board         = board->addPiece(coord, piece);
    auto optPiece = board->at(coord);
    ASSERT_TRUE(optPiece);
    EXPECT_EQ(piece, *optPiece);
}

TEST(Board, AddPieceOnOccupiedSpaceShouldThrowInvalidPiece) {
    Coord coord{A, _1};
    Piece piece{Type::Bishop, Color::Black};
    auto  board = Board::make({{coord, piece}});
    EXPECT_THROW((void)board->addPiece(coord, {Type::Queen, Color::White}), invalid_piece);
}

TEST(Board, RemovePieceShouldRemovePiece) {
    Coord coord{A, _1};
    Piece piece{Type::Bishop, Color::Black};
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
    Piece piece1{Type::Rook, Color::White}, piece2{Type::Pawn, Color::Black};
    auto  board = Board::make({{from, piece1}, {to, piece2}});
    ASSERT_THROW((void)board->movePiece(from, to), invalid_piece);
}

TEST(Board, MovePieceShouldMovePiece) {
    Coord from{A, _1};
    Coord to{B, _2};
    Piece piece{Type::Rook, Color::White};
    auto  board = Board::make({{from, piece}});
    ASSERT_NO_THROW(board = board->movePiece(from, to));
    auto oldPiece = board->at(from);
    EXPECT_FALSE(oldPiece);
    auto newPiece = board->at(to);
    EXPECT_TRUE(newPiece);
    EXPECT_EQ(piece, *newPiece);
}

TEST(Board, MakeShouldThrowOnOverlappingPieces) {
    Coord coord{A, _1};
    Piece piece1{Type::King, Color::White};
    Piece piece2{Type::Queen, Color::White};
    ASSERT_THROW((void)Board::make({{coord, piece1}, {coord, piece2}}), invalid_piece);
}

TEST(Board, RemovePieceShouldThrowIfUnoccupied) {
    auto board = Board::make({});
    ASSERT_THROW((void)board->removePiece({A, _1}), invalid_piece);
}

TEST(Board, MovingPieceShouldNotAffectOtherPieces) {
    Coord coord1{A, _1}, coord2{B, _2};
    Piece piece1{Type::Bishop, Color::White}, piece2{Type::Knight, Color::White};
    auto  board   = Board::make({{coord1, piece1}, {coord2, piece2}});
    board         = board->movePiece(coord1, {A, _2});
    auto optPiece = board->at(coord2);

    ASSERT_TRUE(optPiece);
    EXPECT_EQ(*optPiece, piece2);
}

TEST(Board, AddPieceDoesNotMutateBoard) {
    auto  board1 = Board::make({});
    Coord coord{A, _1};

    EXPECT_FALSE(board1->at(coord));

    {
        auto board2 = board1->addPiece(coord, {Type::Rook, Color::Black});
        EXPECT_TRUE(board2->at(coord));
    }

    EXPECT_FALSE(board1->at(coord));
}

TEST(Board, RemovePieceDoesNotMutateBoard) {
    Coord coord{A, _1};
    auto  board1 = Board::make({{coord, {Type::Rook, Color::Black}}});

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
    auto  board1 = Board::make({{coord1, {Type::Pawn, Color::Black}}});

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
