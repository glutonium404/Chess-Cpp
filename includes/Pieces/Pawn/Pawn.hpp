#pragma once

#include "../../Piece/Piece.hpp"

class Pawn : public Piece {
public:
    Pawn(
        sf::RenderWindow&   render_window,
        Coordinate          coordinate,
        sf::FloatRect       board_local_bound,
        Piece::COLOR        color
    );

    TYPE get_type() const override;
    void set_legal_moves(Board& board) override;

private:
    int direction;

    void add_forward_moves(Board& board);
    void add_diagonal_moves(Board& board);
};
