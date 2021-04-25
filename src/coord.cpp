//
// Created by taylor-santos on 4/24/2021 at 13:44.
//

#include "coord.h"

#include <ostream>
#include <sstream>

namespace Chess {

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

std::ostream &
operator<<(std::ostream &os, const Coord &coord) {
    return os << coord.file << coord.rank;
}

} // namespace Chess
