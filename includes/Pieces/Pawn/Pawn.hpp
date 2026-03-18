#pragma once

#include "../../Piece/Piece.hpp"

class Pawn : public Piece {
public:
    Pawn(
        sf::RenderWindow&   render_window,
        Coordinate          coordinate,
        sf::FloatRect       board_local_bound,
        SIDE                side
    );

    std::vector<Coordinate> get_possible_moves() override;

private:
    bool is_first_move = true;
    int direction;
};
