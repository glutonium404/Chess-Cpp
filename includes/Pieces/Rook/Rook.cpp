#include "Rook.hpp"

Rook::Rook(
    sf::RenderWindow&   render_window,
    Coordinate          coordinate,
    sf::FloatRect       board_local_bound,
    Piece::COLOR         color
)
    : Piece(
    render_window,
    color == Piece::COLOR::BLACK ? "assets/images/rook-b.png" : "assets/images/rook-w.png",
    coordinate,
    board_local_bound,
    color
) {}

void Rook::set_legal_moves(Board& board) {
    int col, row;

    // all left squares
    for(col = coordinate.col - 1; col > 0; col--) {
        if(add_common_legal_moves(board, coordinate.row, col))
            break;
    }

    // all right squares
    for(col = coordinate.col + 1; col < 9; col++) {
        if(add_common_legal_moves(board, coordinate.row, col))
            break;
    }

    // all top squares
    for(row = coordinate.row - 1; row > 0; row--) {
        if(add_common_legal_moves(board, row, coordinate.col))
            break;
    }

    // all bottom squares
    for(row = coordinate.row + 1; row < 9; row++) {
        if(add_common_legal_moves(board, row, coordinate.col))
            break;
    }
}

Piece::TYPE Rook::get_type() const {
    return Piece::TYPE::ROOK;
}
