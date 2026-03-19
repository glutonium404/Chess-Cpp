#pragma once

#include "../../Piece/Piece.hpp"
#include <SFML/Graphics/Rect.hpp>

class Bishop : public Piece {
public:
    Bishop(
        sf::RenderWindow&   render_window,
        Coordinate          coordinate,
        sf::FloatRect       board_local_bound,
        Piece::COLOR         side
    );

    std::vector<Coordinate> get_possible_moves() override;

private:
    const std::vector<Coordinate> get_lookup_coordinates() const;
};
