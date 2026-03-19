#pragma once

#include "../../Piece/Piece.hpp"

class Rook : public Piece {
public:
    Piece::COLOR side;

    Rook(
        sf::RenderWindow&   render_window,
        Coordinate          coordinate,
        sf::FloatRect       board_local_bound,
        Piece::COLOR        side
    );

    TYPE                    get_type() const override;
    std::vector<Coordinate> get_possible_moves() override;

private:
    const std::vector<Coordinate> get_lookup_coordinates() const;
};
