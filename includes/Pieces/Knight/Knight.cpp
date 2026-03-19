#include "Knight.hpp"

Knight::Knight(
    sf::RenderWindow&   render_window,
    Coordinate          coordinate,
    sf::FloatRect       board_local_bound,
    Piece::COLOR         side
)
    : Piece(
    render_window,
    side == Piece::COLOR::BLACK ? "assets/images/knight-b.png" : "assets/images/knight-w.png",
    coordinate,
    board_local_bound
) {}

std::vector<Coordinate> Knight::get_possible_moves() {
    std::vector<Coordinate> possible_moves;
    possible_moves.reserve(8); // 8 = maximum possible moves

    for(auto& coord: get_lookup_coordinates()) {
        if(!is_coordinate_in_bound(coord)) continue;

        possible_moves.push_back(coord);
    }

    return possible_moves;
}

const std::vector<Coordinate> Knight::get_lookup_coordinates() const {
    std::vector<Coordinate> lookup_coordinates;
    lookup_coordinates.reserve(8);

    lookup_coordinates.push_back( Coordinate( coordinate.row - 2, coordinate.col - 1 ) );
    lookup_coordinates.push_back( Coordinate( coordinate.row - 2, coordinate.col + 1 ) );

    lookup_coordinates.push_back( Coordinate( coordinate.row + 2, coordinate.col - 1 ) );
    lookup_coordinates.push_back( Coordinate( coordinate.row + 2, coordinate.col + 1 ) );

    lookup_coordinates.push_back( Coordinate( coordinate.row + 1, coordinate.col - 2 ) );
    lookup_coordinates.push_back( Coordinate( coordinate.row - 1, coordinate.col - 2 ) );

    lookup_coordinates.push_back( Coordinate( coordinate.row + 1, coordinate.col + 2 ) );
    lookup_coordinates.push_back( Coordinate( coordinate.row - 1, coordinate.col + 2 ) );

    return lookup_coordinates;
}

Piece::TYPE Knight::get_type() const {
    return Piece::TYPE::KNIGHT;
}
