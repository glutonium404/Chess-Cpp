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


void Queen::set_legal_moves(Board& board) {
    int col, row;

    // all left squares
    for(col = coordinate.col - 1; col > 0; col--) {
        if(add_common_legal_moves(board, legal_moves, coordinate.row, col))
            break;
    }

    // all right squares
    for(col = coordinate.col + 1; col < 9; col++) {
        if(add_common_legal_moves(board, legal_moves, coordinate.row, col))
            break;
    }

    // all top squares
    for(row = coordinate.row - 1; row > 0; row--) {
        if(add_common_legal_moves(board, legal_moves, row, coordinate.col))
            break;
    }

    // all bottom squares
    for(row = coordinate.row + 1; row < 9; row++) {
        if(add_common_legal_moves(board, legal_moves, row, coordinate.col))
            break;
    }

    // all top-left squares
    for(col = coordinate.col - 1, row = coordinate.row - 1; Coordinate::is_valid(row, col); col--, row--) {
        if(add_common_legal_moves(board, legal_moves, row, col))
            break;
    }

    // all top-right squares
    for(col = coordinate.col + 1, row = coordinate.row - 1; Coordinate::is_valid(row, col); col++, row--) {
        if(add_common_legal_moves(board, legal_moves, row, col))
            break;
    }

    // all bottom-right squares
    for(col = coordinate.col + 1, row = coordinate.row + 1; Coordinate::is_valid(row, col); col++, row++) {
        if(add_common_legal_moves(board, legal_moves, row, col))
            break;
    }

    // all bottom-left squares
    for(col = coordinate.col - 1, row = coordinate.row + 1; Coordinate::is_valid(row, col); col--, row++) {
        if(add_common_legal_moves(board, legal_moves, row, col))
            break;
    }
}

Piece::TYPE Queen::get_type() const {
    return Piece::TYPE::QUEEN;
}
