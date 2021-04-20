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

TEST(Piece, NotEqualIfTypesDiffer) {
    Color color = Color::White;
    Coord coord = Coord{D, _5};
    Piece a{Piece::Type::King, color, coord};
    Piece b{Piece::Type::Queen, color, coord};
    ASSERT_NE(a, b);
}

TEST(Piece, NotEqualIfColorsDiffer) {
    Piece::Type type  = Piece::Type::Bisop;
    Coord       coord = Coord{D, _5};
    Piece       a{type, Color::White, coord};
    Piece       b{type, Color::Black, coord};
    ASSERT_NE(a, b);
}
TEST(Piece, NotEqualIfCoordsDiffer) {
    Piece::Type type  = Piece::Type::Bisop;
    Color       color = Color::White;
    Piece       a{type, color, Coord{A, _1}};
    Piece       b{type, color, Coord{B, _2}};
    ASSERT_NE(a, b);
}

TEST(Board, GetPieceValidArgumentShouldNotThrow) {
    Board board;
    EXPECT_NO_THROW((void)board.getPiece(Coord{A, _1}));
}

TEST(Board, GetPieceShouldGetAddedPiece) {
    Board board;
    Piece piece{Piece::Type::Bisop, Color::Black, Coord{A, _1}};
    ASSERT_NO_THROW(board.addPiece(piece));
    auto optPiece = board.getPiece(piece.coord);
    ASSERT_TRUE(optPiece);
    EXPECT_EQ(piece, *optPiece);
}

TEST(Board, AddPieceOnOccupiedSpaceShouldThrowInvalidPiece) {
    Board board;
    Piece piece{Piece::Type::Bisop, Color::Black, Coord{A, _1}};
    ASSERT_NO_THROW(board.addPiece(piece));
    Piece newPiece{Piece::Type::Queen, Color::White, Coord{A, _1}};
    EXPECT_THROW(board.addPiece(newPiece), invalid_piece);
}

TEST(Board, RemovePieceShouldRemovePiece) {
    Board board;
    Coord coord{A, _1};
    Piece piece{Piece::Type::Bisop, Color::Black, coord};
    ASSERT_NO_THROW(board.addPiece(piece));
    {
        auto optPiece = board.getPiece(coord);
        EXPECT_TRUE(optPiece);
    }
    board.removePiece(coord);
    {
        auto optPiece = board.getPiece(coord);
        EXPECT_FALSE(optPiece);
    }
}

TEST(Board, MovePieceFromUnoccupiedShouldThrowInvalidPiece) {
    Board board;
    ASSERT_THROW(board.movePiece(Coord{A, _1}, Coord{B, _2}), invalid_piece);
}

TEST(Board, MovePieceToOccupiedShouldThrowInvalidPiece) {
    Board board;
    Coord from{A, _1};
    Coord to{B, _2};
    ASSERT_NO_THROW(board.addPiece({Piece::Type::Rook, Color::White, from}));
    ASSERT_NO_THROW(board.addPiece({Piece::Type::Pawn, Color::Black, to}));
    ASSERT_THROW(board.movePiece(from, to), invalid_piece);
}

TEST(Board, MovePieceShouldMovePiece) {
    Board board;
    Coord from{A, _1};
    Coord to{B, _2};
    Piece piece{Piece::Type::Rook, Color::White, from};
    ASSERT_NO_THROW(board.addPiece(piece));
    ASSERT_NO_THROW(board.movePiece(from, to));
    auto oldPiece = board.getPiece(from);
    EXPECT_FALSE(oldPiece);
    auto newPiece = board.getPiece(to);
    EXPECT_TRUE(newPiece);
    piece.coord = to;
    EXPECT_EQ(piece, *newPiece);
}
