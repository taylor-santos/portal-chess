//
// Created by taylor-santos on 4/24/2021 at 13:44.
//

#include "coord.h"

#include <ostream>
#include <sstream>
#include <optional>

namespace Chess {

const File files[8] = {A, B, C, D, E, F, G, H};
const Rank ranks[8] = {_1, _2, _3, _4, _5, _6, _7, _8};

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

std::optional<Coord>
Coord::offset(int fileOffset, int rankOffset) const {
    int newFile = file + fileOffset;
    int newRank = rank + rankOffset;
    if (newFile < 1 || 8 < newFile || newRank < 1 || 8 < newRank) {
        return std::nullopt;
    }
    return Coord(static_cast<File>(newFile), static_cast<Rank>(newRank));
}

std::optional<Coord>
Coord::offset(std::pair<int, int> offsets) const {
    auto [file, rank] = offsets;
    return offset(file, rank);
}

std::ostream &
operator<<(std::ostream &os, const Coord &coord) {
    return os << coord.file << coord.rank;
}

} // namespace Chess
