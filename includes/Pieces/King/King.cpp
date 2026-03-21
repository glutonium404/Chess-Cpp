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

    std::vector<Coordinate> common_lookup_coordinates = {
        {coordinate.row - 1, coordinate.col - 1}, // top left
        {coordinate.row - 1, coordinate.col    }, // top middle
        {coordinate.row - 1, coordinate.col + 1}, // top right

        {coordinate.row + 1, coordinate.col - 1}, // bottom left
        {coordinate.row + 1, coordinate.col    }, // bottom middle
        {coordinate.row + 1, coordinate.col + 1}, // bottom right

        {coordinate.row    , coordinate.col - 1}, // center left
        {coordinate.row    , coordinate.col + 1}  // center right
    };

    for(auto& coord: common_lookup_coordinates) {
        if(coord.is_valid())
            add_common_legal_moves(board, lookup_coordinates, coord.row, coord.col);
    }

    return lookup_coordinates;
}

Piece::TYPE King::get_type() const {
    return Piece::TYPE::KING;
}
