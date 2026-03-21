#include "King.hpp"

King::King(
    sf::RenderWindow&   render_window,
    Coordinate          coordinate,
    sf::FloatRect       board_local_bound,
    Piece::COLOR        color
)
    : Piece(
    render_window,
    color == Piece::COLOR::BLACK ? "assets/images/king-b.png" : "assets/images/king-w.png",
    coordinate,
    board_local_bound,
    color
) {}

std::vector<Coordinate> King::get_legal_moves(Board& board) {
    std::vector<Coordinate> legal_moves;
    legal_moves.reserve(8);

    for(auto& coord: get_lookup_coordinates(board)) {
        if(!coord.is_valid()) continue;

        legal_moves.push_back(coord);
    }

    return legal_moves;
}

const std::vector<Coordinate> King::get_lookup_coordinates(Board& board) const {
    std::vector<Coordinate> lookup_coordinates;
    lookup_coordinates.reserve(8);

    lookup_coordinates.push_back( Coordinate( coordinate.row - 1, coordinate.col - 1 ) ); // top left
    lookup_coordinates.push_back( Coordinate( coordinate.row - 1, coordinate.col     ) ); // top middle
    lookup_coordinates.push_back( Coordinate( coordinate.row - 1, coordinate.col + 1 ) ); // top right

    lookup_coordinates.push_back( Coordinate( coordinate.row + 1, coordinate.col - 1 ) ); // bottom left
    lookup_coordinates.push_back( Coordinate( coordinate.row + 1, coordinate.col     ) ); // bottom middle
    lookup_coordinates.push_back( Coordinate( coordinate.row + 1, coordinate.col + 1 ) ); // bottom right

    lookup_coordinates.push_back( Coordinate( coordinate.row    , coordinate.col - 1 ) ); // center left
    lookup_coordinates.push_back( Coordinate( coordinate.row    , coordinate.col + 1 ) ); // center right

    return lookup_coordinates;
}

Piece::TYPE King::get_type() const {
    return Piece::TYPE::KING;
}
