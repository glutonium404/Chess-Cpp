#include "Queen.hpp"

Queen::Queen(
    sf::RenderWindow&   render_window,
    Coordinate          coordinate,
    sf::FloatRect       board_local_bound,
    Piece::COLOR         side
)
    : Piece(
    render_window,
    side == Piece::COLOR::BLACK ? "assets/images/queen-b.png" : "assets/images/queen-w.png",
    coordinate,
    board_local_bound
) {}

std::vector<Coordinate> Queen::get_possible_moves() {
    std::vector<Coordinate> possible_moves;
    possible_moves.reserve(27); // 27 = maximum possible moves

    for(auto& coord: get_lookup_coordinates()) {
        possible_moves.push_back(coord);
    }

    return possible_moves;
}

const std::vector<Coordinate> Queen::get_lookup_coordinates() const {
    std::vector<Coordinate> lookup_coordinates;
    lookup_coordinates.reserve(27);

    int col, row;

    // all left squares
    for(col = coordinate.col - 1; col > 0; col--) {
        lookup_coordinates.push_back({ coordinate.row, col });
    }

    // all right squares
    for(col = coordinate.col + 1; col < 9; col++) {
        lookup_coordinates.push_back({ coordinate.row, col });
    }

    // all top squares
    for(row = coordinate.row - 1; row > 0; row--) {
        lookup_coordinates.push_back({ row, coordinate.col });
    }

    // all bottom squares
    for(row = coordinate.row + 1; row < 9; row++) {
        lookup_coordinates.push_back({ row, coordinate.col });
    }

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

Piece::TYPE Queen::get_type() const {
    return Piece::TYPE::QUEEN;
}
