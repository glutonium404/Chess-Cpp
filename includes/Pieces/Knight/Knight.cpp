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

void Knight::set_legal_moves(Board& board) {
    std::vector<Coordinate> common_legal_moves = {
        {coordinate.row - 2, coordinate.col - 1},
        {coordinate.row - 2, coordinate.col + 1},

        {coordinate.row + 2, coordinate.col - 1},
        {coordinate.row + 2, coordinate.col + 1},

        {coordinate.row + 1, coordinate.col - 2},
        {coordinate.row - 1, coordinate.col - 2},

        {coordinate.row + 1, coordinate.col + 2},
        {coordinate.row - 1, coordinate.col + 2}
    };

    for(auto& coord: common_legal_moves) {
        if(coord.is_valid())
            add_common_legal_moves(board, legal_moves, coord.row, coord.col);
    }
}

Piece::TYPE Knight::get_type() const {
    return Piece::TYPE::KNIGHT;
}
