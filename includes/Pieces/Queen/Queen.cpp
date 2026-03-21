#include "Queen.hpp"

Queen::Queen(
    sf::RenderWindow&   render_window,
    Coordinate          coordinate,
    sf::FloatRect       board_local_bound,
    Piece::COLOR         color
)
    : Piece(
    render_window,
    color == Piece::COLOR::BLACK ? "assets/images/queen-b.png" : "assets/images/queen-w.png",
    coordinate,
    board_local_bound,
    color
) {}

std::vector<Coordinate> Queen::get_legal_moves(Board& board) {
    std::vector<Coordinate> legal_moves;
    legal_moves.reserve(27); // 27 = maximum possible moves

    for(auto& coord: get_lookup_coordinates(board)) {
        legal_moves.push_back(coord);
    }

    return legal_moves;
}

const std::vector<Coordinate> Queen::get_lookup_coordinates(Board& board) const {
    std::vector<Coordinate> lookup_coordinates;
    lookup_coordinates.reserve(27);

    int col, row;

    // all left squares
    for(col = coordinate.col - 1; col > 0; col--) {
        if(add_common_legal_moves(board, lookup_coordinates, coordinate.row, col))
            break;
    }

    // all right squares
    for(col = coordinate.col + 1; col < 9; col++) {
        if(add_common_legal_moves(board, lookup_coordinates, coordinate.row, col))
            break;
    }

    // all top squares
    for(row = coordinate.row - 1; row > 0; row--) {
        if(add_common_legal_moves(board, lookup_coordinates, row, coordinate.col))
            break;
    }

    // all bottom squares
    for(row = coordinate.row + 1; row < 9; row++) {
        if(add_common_legal_moves(board, lookup_coordinates, row, coordinate.col))
            break;
    }

    // all top-left squares
    for(col = coordinate.col - 1, row = coordinate.row - 1; Coordinate::is_valid(row, col); col--, row--) {
        if(add_common_legal_moves(board, lookup_coordinates, row, col))
            break;
    }

    // all top-right squares
    for(col = coordinate.col + 1, row = coordinate.row - 1; Coordinate::is_valid(row, col); col++, row--) {
        if(add_common_legal_moves(board, lookup_coordinates, row, col))
            break;
    }

    // all bottom-right squares
    for(col = coordinate.col + 1, row = coordinate.row + 1; Coordinate::is_valid(row, col); col++, row++) {
        if(add_common_legal_moves(board, lookup_coordinates, row, col))
            break;
    }

    // all bottom-left squares
    for(col = coordinate.col - 1, row = coordinate.row + 1; Coordinate::is_valid(row, col); col--, row++) {
        if(add_common_legal_moves(board, lookup_coordinates, row, col))
            break;
    }

    return lookup_coordinates;
}

Piece::TYPE Queen::get_type() const {
    return Piece::TYPE::QUEEN;
}
