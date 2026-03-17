#pragma once

#include "../Piece.hpp"

class King : public Piece {
public:
    King(
        sf::RenderWindow& render_window,
        sf::Vector2u coordinate,
        sf::Vector2f board_origin,
        float square_length,
        SIDE side
    )
        : Piece(
        render_window,
        side == SIDE::BLACK ? "assets/images/b_king.png" : "assets/images/w_king.png",
        coordinate,
        board_origin,
        square_length
    )
    {}
};
