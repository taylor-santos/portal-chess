//
// Created by taylor-santos on 4/24/2021 at 13:43.
//

#ifndef PORTAL_CHESS_INCLUDE_COORD_H
#define PORTAL_CHESS_INCLUDE_COORD_H

#include <ostream>
#include <optional>

namespace Chess {

enum File { A = 1, B, C, D, E, F, G, H };
enum Rank { _1 = 1, _2, _3, _4, _5, _6, _7, _8 };

extern const File files[8];
extern const Rank ranks[8];

std::ostream &
operator<<(std::ostream &os, File file);

class Coord {
public:
    /***
     * Construct a Coord instance from the given file and rank values.
     * @param file the horizontal coordinate, letters A-H representing values [1,8]
     * @param rank the vertical coordinate, representing values [1,8]
     * @throws std::invalid_argument if either argument is outside of the interval [1,8]
     */
    Coord(File file, Rank rank);

    bool
    operator==(const Coord &other) const;

    bool
    operator!=(const Coord &other) const;

    [[nodiscard]] std::optional<Coord>
    offset(int fileOffset, int rankOffset) const;

    [[nodiscard]] std::optional<Coord>
    offset(std::pair<int, int> offsets) const;

    File file;
    Rank rank;
};

std::ostream &
operator<<(std::ostream &os, const Coord &coord);

} // namespace Chess

#endif // PORTAL_CHESS_INCLUDE_COORD_H
