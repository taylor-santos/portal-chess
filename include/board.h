//
// Created by taylor-santos on 4/18/2021 at 22:09.
//

#ifndef PORTAL_CHESS_INCLUDE_BOARD_H
#define PORTAL_CHESS_INCLUDE_BOARD_H

#include <vector>
#include <iostream>
#include <memory>
#include <optional>

namespace Chess {

class Piece;
class Coord;

class Board {
public:
    Board(const Board &) = delete;
    virtual ~Board()     = default;

    /***
     * Construct a new Board from a list of pieces and return it wrapped in an std::shared_ptr.
     * @param pieces a list of (coord, piece) pairs to be added to the board, where coord is the
     *        location for the piece, and piece is a std::shared_ptr to a Piece
     * @returns a newly constructed Board wrapped in a std::shared_ptr, containing the given pieces
     * @throws invalid_piece if two or more of the given pieces have overlapping coordinates
     */
    [[nodiscard]] static std::shared_ptr<const Board>
    make(const std::vector<std::pair<Coord, std::shared_ptr<Piece>>> &pieces);

    /***
     * Retrieve a piece from the Board at the given coordinate.
     * @param coord the coordinate to retrieve a piece from
     * @returns an std::optional containing the piece at the given coordinate if one exists, or an
     *          empty std::optional otherwise
     */
    [[nodiscard]] virtual std::optional<const Piece *>
    at(Coord coord) const = 0;

    /***
     * Construct a new Board representing this Board's state, plus an added piece.
     * @param coord the coordinate to add the piece at
     * @param piece the piece to add to the new Board
     * @returns a new Board state representing this Board with the piece added
     * @throws invalid_piece if this Board already has a piece at the new piece's coordinates
     */
    [[nodiscard]] std::shared_ptr<const Board>
    addPiece(Coord coord, std::shared_ptr<Piece> piece) const;

    /***
     * Construct a new Board representing this Board's state, with one piece removed from the given
     * coordinate.
     * @param coord the coordinate to remove a piece from
     * @returns a new Board state representing this Board with a piece removed
     * @throws invalid_piece if no piece exists at the given coordinate
     */
    [[nodiscard]] std::shared_ptr<const Board>
    removePiece(Coord coord) const;

    /**
     * Construct a new Board representing this Board's state, with one piece moved from one
     * coordinate to another.
     * @param from the coordinate to move the piece from
     * @param to the coordinate to move the piece to
     * @returns a new Board state representing this Board with one piece moved
     * @throws invalid_piece if either the "from" coordinate is unoccupied,
     *         or if the "to" coordinate is occupied
     */
    [[nodiscard]] std::shared_ptr<const Board>
    movePiece(Coord from, Coord to) const;

private:
    class InitialBoard;
    class AddedPiece;
    class RemovedPiece;
    class MovedPiece;

    Board() = default;

    std::weak_ptr<Board> wptr_;
};

class invalid_piece : public std::runtime_error {
public:
    explicit invalid_piece(const std::string &arg);
};

} // namespace Chess

#endif // PORTAL_CHESS_INCLUDE_BOARD_H
