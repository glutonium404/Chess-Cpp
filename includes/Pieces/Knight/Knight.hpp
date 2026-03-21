#pragma once

#include "../../Piece/Piece.hpp"

class Knight : public Piece {
public:
    Knight(
        sf::RenderWindow&   render_window,
        Coordinate          coordinate,
        sf::FloatRect       board_local_bound,
        Piece::COLOR        color
    );

    TYPE                    get_type() const override;
    std::vector<Coordinate> get_legal_moves(Board& board) override;

private:
    const std::vector<Coordinate> get_lookup_coordinates(Board& board) const;
};
