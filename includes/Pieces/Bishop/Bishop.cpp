#include "Bishop.hpp"

Bishop::Bishop(
    sf::RenderWindow&   render_window,
    Coordinate          coordinate,
    sf::FloatRect       board_local_bound,
    Piece::COLOR        color
)
    : Piece(
    render_window,
    color == Piece::COLOR::BLACK ? "assets/images/bishop-b.png" : "assets/images/bishop-w.png",
    coordinate,
    board_local_bound,
    color
)
{
    directions = {
        {-1, -1}, // top-left
        {-1,  1}, // top-right
        { 1,  1}, // bottom-right
        { 1, -1}  // bottom-left
    };
}

Piece::TYPE Bishop::get_type() const {
    return Piece::TYPE::BISHOP;
}
