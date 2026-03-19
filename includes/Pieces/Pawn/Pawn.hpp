#pragma once

#include "../../Piece/Piece.hpp"

class Pawn : public Piece {
public:
    Piece::COLOR            side;

    Pawn(
        sf::RenderWindow&   render_window,
        Coordinate          coordinate,
        sf::FloatRect       board_local_bound,
        Piece::COLOR        side
    );

    TYPE                    get_type() const override;
    std::vector<Coordinate> get_possible_moves() override;

private:
    bool is_first_move = true;
    int direction;
};
