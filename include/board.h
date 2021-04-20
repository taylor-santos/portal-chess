//
// Created by taylor-santos on 4/18/2021 at 22:09.
//

#ifndef PORTAL_CHESS_INCLUDE_BOARD_H
#define PORTAL_CHESS_INCLUDE_BOARD_H

#include <vector>
#include <memory>
#include <optional>

namespace Chess {

enum class Color { White, Black };

// Use enum, not enum class, to allow implicit int conversion
enum File { A = 1, B, C, D, E, F, G, H };
enum Rank { _1 = 1, _2, _3, _4, _5, _6, _7, _8 };

std::ostream &
operator<<(std::ostream &os, File file);

struct Coord {
    Coord(File file, Rank rank);
    bool
    operator==(const Coord &other) const;
    bool
    operator!=(const Coord &other) const;

    File file;
    Rank rank;
};

std::ostream &
operator<<(std::ostream &os, const Coord &coord);

struct Piece {
    enum class Type { Bisop, King, Knight, Pawn, Portal, Queen, Rook } type;
    Color color;
    Coord coord;

    bool
    operator==(const Piece &other) const;
    bool
    operator!=(const Piece &other) const;
};

class Board {
public:
    Board();
    Board(const Board &) = delete;
    ~Board();

    Board &
    operator=(const Board &) = delete;

    /***
     * Retrieve an optional Piece from a given coordinate. If no piece exists, returns an empty
     * std::optional.
     * @param coord The Coord representing the rank and file to get a piece from
     * @returns An std::optional containing a Piece if one exists, or an empty std::optional
     *         otherwise
     */
    [[nodiscard]] std::optional<Piece>
    getPiece(Coord coord) const;

    /***
     * Inserts a piece onto the board. The placement is taken from the piece's coord field.
     * @param piece the piece to be placed on the board
     * @throws invalid_piece if the piece's coordinate is already occupied by an existing piece
     */
    void
    addPiece(Piece piece);

    /***
     * Remove a piece from the board if it is at the given coordinate, do nothing otherwise.
     * @param coord the coordinate to remove a piece from
     */
    void
    removePiece(Coord coord);

    /***
     * Move a piece from one position to another. This must be invoked from an occupied coordinate
     * to an unoccupied coordinate, otherwise an invalid_piece exception is thrown.
     * @param from the coordinate to move a piece from
     * @param to the coordinate to move the piece to
     * @throws invalid_piece if moving from an unoccupied location or if moving to an occupied
     *         location. If an exception is thrown, the board is guaranteed to be unchanged.
     */
    void
    movePiece(Coord from, Coord to);

private:
    class BoardState {
    public:
        [[nodiscard]] const std::optional<Piece> &
        operator()(Coord coord) const;

        [[nodiscard]] std::optional<Piece> &
        operator()(Coord coord);

    private:
        std::optional<Piece> board[8][8];
    } board_;
};

class invalid_piece : public std::runtime_error {
public:
    invalid_piece(const std::string &arg);
};

} // namespace Chess

#endif // PORTAL_CHESS_INCLUDE_BOARD_H
