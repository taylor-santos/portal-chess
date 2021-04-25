//
// Created by taylor-santos on 4/24/2021 at 17:48.
//

#include "gtest/gtest.h"
#include "coord.h"

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
