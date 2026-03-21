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

std::vector<Coordinate> Bishop::get_legal_moves(Board& board) {
    std::vector<Coordinate> legal_moves;
    legal_moves.reserve(15); // 15 = maximum possible moves

    for(auto& coord: get_lookup_coordinates(board)) {
        legal_moves.push_back(coord);
    }

    return legal_moves;
}

const std::vector<Coordinate> Bishop::get_lookup_coordinates(Board& board) const {
    std::vector<Coordinate> lookup_coordinates;
    lookup_coordinates.reserve(15);

    int col, row;

    // all top-left squares
    for(col = coordinate.col - 1, row = coordinate.row - 1; Coordinate::is_valid(row, col); col--, row--) {
        if(add_common_legal_moves(board, lookup_coordinates, row, col)) break;
    }

    // all top-right squares
    for(col = coordinate.col + 1, row = coordinate.row - 1; Coordinate::is_valid(row, col); col++, row--) {
        if(add_common_legal_moves(board, lookup_coordinates, row, col)) break;
    }

    // all bottom-right squares
    for(col = coordinate.col + 1, row = coordinate.row + 1; Coordinate::is_valid(row, col); col++, row++) {
        if(add_common_legal_moves(board, lookup_coordinates, row, col)) break;
    }

    // all bottom-left squares
    for(col = coordinate.col - 1, row = coordinate.row + 1; Coordinate::is_valid(row, col); col--, row++) {
        if(add_common_legal_moves(board, lookup_coordinates, row, col)) break;
    }

    return lookup_coordinates;
}

Piece::TYPE Bishop::get_type() const {
    return Piece::TYPE::BISHOP;
}
