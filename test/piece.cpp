//
// Created by taylor-santos on 4/24/2021 at 17:47.
//

#include "gtest/gtest.h"
#include "piece.h"

using namespace Chess;

TEST(Piece, EqualityOperator) {
    auto colors = {Color::White, Color::Black};
    auto types =
        {Type::Bishop, Type::King, Type::Knight, Type::Pawn, Type::Portal, Type::Queen, Type::Rook};

    for (auto color : colors) {
        for (auto type : types) {
            Piece piece1{type, color};
            Piece piece2{type, color};
            EXPECT_EQ(piece1, piece2);
        }
    }
}

TEST(Piece, InequalityOperator) {
    auto colors = {Color::White, Color::Black};
    auto types =
        {Type::Bishop, Type::King, Type::Knight, Type::Pawn, Type::Portal, Type::Queen, Type::Rook};
    for (auto color1 : colors) {
        for (auto type1 : types) {
            Piece piece1{type1, color1};
            for (auto color2 : colors) {
                for (auto type2 : types) {
                    if (color1 != color2 || type1 != type2) {
                        Piece piece2{type2, color2};
                        EXPECT_NE(piece1, piece2);
                    }
                }
            }
        }
    }
}
