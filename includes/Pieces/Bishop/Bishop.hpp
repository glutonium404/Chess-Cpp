#pragma once

#include "../../Piece/Piece.hpp"
#include <SFML/Graphics/Rect.hpp>

class Bishop : public Piece {
public:
    Bishop(
        sf::RenderWindow&   render_window,
        Coordinate          coordinate,
        sf::FloatRect       board_local_bound,
        Piece::COLOR        color
    );

    TYPE get_type() const override;

private:
};
