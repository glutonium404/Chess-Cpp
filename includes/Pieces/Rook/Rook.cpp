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

std::vector<Coordinate> Rook::get_legal_moves() {
    std::vector<Coordinate> legal_moves;
    legal_moves.reserve(16); // 16 = maximum possible moves

    for(auto& coord: get_lookup_coordinates()) {
        legal_moves.push_back(coord);
    }

    return legal_moves;
}

const std::vector<Coordinate> Rook::get_lookup_coordinates() const {
    std::vector<Coordinate> lookup_coordinates;
    lookup_coordinates.reserve(16);

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

    return lookup_coordinates;
}

Piece::TYPE Rook::get_type() const {
    return Piece::TYPE::ROOK;
}
