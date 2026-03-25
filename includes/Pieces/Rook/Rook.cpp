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
)
{
    directions = {
        { 0, -1}, // left
        { 0,  1}, // right
        {-1,  0}, // top
        { 1,  0}, // bottom
    };
}

Piece::TYPE Rook::get_type() const {
    return Piece::TYPE::ROOK;
}
