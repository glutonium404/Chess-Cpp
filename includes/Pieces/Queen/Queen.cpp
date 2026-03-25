#include "Queen.hpp"
#include <my_utils.hpp>

Queen::Queen(
    sf::RenderWindow&   render_window,
    Coordinate          coordinate,
    sf::FloatRect       board_local_bound,
    Piece::COLOR         color
)
    : Piece(
    render_window,
    color == Piece::COLOR::BLACK ? "assets/images/queen-b.png" : "assets/images/queen-w.png",
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
        {-1, -1}, // top-left
        {-1,  1}, // top-right
        { 1,  1}, // bottom-right
        { 1, -1}  // bottom-left
    };
}

Piece::TYPE Queen::get_type() const {
    return Piece::TYPE::QUEEN;
}
