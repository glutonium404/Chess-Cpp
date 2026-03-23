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

void Rook::set_legal_moves(Board& board) {
    std::vector<Coordinate> directions = {
        { 0, -1}, // left
        { 0,  1}, // right
        {-1,  0}, // top
        { 1,  0}, // bottom
    };

    for(auto& dir: directions) {
        for(Coordinate curr_coord = coordinate + dir; curr_coord.is_valid(); curr_coord += dir) {
            if(add_common_legal_moves(board, curr_coord.row, curr_coord.col))
                break;
        }
    }
}

Piece::TYPE Rook::get_type() const {
    return Piece::TYPE::ROOK;
}
