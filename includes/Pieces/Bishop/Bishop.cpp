#include "Bishop.hpp"
#include "../../Board/Board.hpp"

Bishop::Bishop(
    sf::RenderWindow&   render_window,
    Coordinate          coordinate,
    sf::FloatRect       board_local_bound,
    Piece::COLOR        color
)
    : Piece(
    render_window,
    color == Piece::COLOR::BLACK ? "assets/images/bishop-b.png" : "assets/images/bishop-w.png",
    coordinate,
    board_local_bound,
    color
) {}

void Bishop::set_legal_moves(Board& board) {
    int col, row;

    // all top-left squares
    for(col = coordinate.col - 1, row = coordinate.row - 1; Coordinate::is_valid(row, col); col--, row--) {
        if(add_common_legal_moves(board, row, col)) break;
    }

    // all top-right squares
    for(col = coordinate.col + 1, row = coordinate.row - 1; Coordinate::is_valid(row, col); col++, row--) {
        if(add_common_legal_moves(board, row, col)) break;
    }

    // all bottom-right squares
    for(col = coordinate.col + 1, row = coordinate.row + 1; Coordinate::is_valid(row, col); col++, row++) {
        if(add_common_legal_moves(board, row, col)) break;
    }

    // all bottom-left squares
    for(col = coordinate.col - 1, row = coordinate.row + 1; Coordinate::is_valid(row, col); col--, row++) {
        if(add_common_legal_moves(board, row, col)) break;
    }
}

Piece::TYPE Bishop::get_type() const {
    return Piece::TYPE::BISHOP;
}
