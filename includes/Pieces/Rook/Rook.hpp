#pragma once

#include "../../Piece/Piece.hpp"

class Rook : public Piece {
public:
    Rook(
        sf::RenderWindow&   render_window,
        Coordinate          coordinate,
        sf::FloatRect       board_local_bound,
        Piece::COLOR        color
    );

    TYPE get_type() const override;
    void set_legal_moves(Board& board) override;

private:
};
