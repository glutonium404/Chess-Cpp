#include "Knight.hpp"

Knight::Knight(
    sf::RenderWindow&   render_window,
    Coordinate          coordinate,
    sf::FloatRect       board_local_bound,
    Piece::COLOR        color
)
    : Piece(
    render_window,
    color == Piece::COLOR::BLACK ? "assets/images/knight-b.png" : "assets/images/knight-w.png",
    coordinate,
    board_local_bound,
    color
) {}

std::vector<Coordinate> Knight::get_legal_moves(Board& board) {
    std::vector<Coordinate> legal_moves;
    legal_moves.reserve(8); // 8 = maximum possible moves

    for(auto& coord: get_lookup_coordinates(board)) {
        if(!coord.is_valid()) continue;

        legal_moves.push_back(coord);
    }

    return legal_moves;
}

const std::vector<Coordinate> Knight::get_lookup_coordinates(Board& board) const {
    std::vector<Coordinate> lookup_coordinates;
    lookup_coordinates.reserve(8);

    std::vector<Coordinate> common_lookup_coordinates = {
        {coordinate.row - 2, coordinate.col - 1},
        {coordinate.row - 2, coordinate.col + 1},

        {coordinate.row + 2, coordinate.col - 1},
        {coordinate.row + 2, coordinate.col + 1},

        {coordinate.row + 1, coordinate.col - 2},
        {coordinate.row - 1, coordinate.col - 2},

        {coordinate.row + 1, coordinate.col + 2},
        {coordinate.row - 1, coordinate.col + 2}
    };

    for(auto& coord: common_lookup_coordinates) {
        if(coord.is_valid())
            add_common_legal_moves(board, lookup_coordinates, coord.row, coord.col);
    }

    return lookup_coordinates;
}

Piece::TYPE Knight::get_type() const {
    return Piece::TYPE::KNIGHT;
}
