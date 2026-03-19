#include "Bishop.hpp"

Bishop::Bishop(
    sf::RenderWindow&   render_window,
    Coordinate          coordinate,
    sf::FloatRect       board_local_bound,
    Piece::COLOR        side
)
    : Piece(
    render_window,
    side == Piece::COLOR::BLACK ? "assets/images/bishop-b.png" : "assets/images/bishop-w.png",
    coordinate,
    board_local_bound
) {}

std::vector<Coordinate> Bishop::get_possible_moves() {
    std::vector<Coordinate> possible_moves;
    possible_moves.reserve(15); // 15 = maximum possible moves

    for(auto& coord: get_lookup_coordinates()) {
        possible_moves.push_back(coord);
    }

    return possible_moves;
}

const std::vector<Coordinate> Bishop::get_lookup_coordinates() const {
    std::vector<Coordinate> lookup_coordinates;
    lookup_coordinates.reserve(15);

    int col, row;

    // all top-left squares
    for(col = coordinate.col - 1, row = coordinate.row - 1; is_coordinate_in_bound(row, col); col--, row--) {
        lookup_coordinates.push_back({ row, col });
    }

    // all top-right squares
    for(col = coordinate.col + 1, row = coordinate.row - 1; is_coordinate_in_bound(row, col); col++, row--) {
        lookup_coordinates.push_back({ row, col });
    }

    // all bottom-right squares
    for(col = coordinate.col + 1, row = coordinate.row + 1; is_coordinate_in_bound(row, col); col++, row++) {
        lookup_coordinates.push_back({ row, col });
    }

    // all bottom-left squares
    for(col = coordinate.col - 1, row = coordinate.row + 1; is_coordinate_in_bound(row, col); col--, row++) {
        lookup_coordinates.push_back({ row, col });
    }

    return lookup_coordinates;
}

Piece::TYPE Bishop::get_type() const {
    return Piece::TYPE::BISHOP;
}
