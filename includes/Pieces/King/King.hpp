#pragma once

#include "../../Piece/Piece.hpp"
#include <SFML/Graphics/Rect.hpp>

class King : public Piece {
public:
    King(
        sf::RenderWindow&   render_window,
        Coordinate          coordinate,
        sf::FloatRect       board_local_bound,
        Piece::COLOR        color
    );

    TYPE get_type() const override;
    void set_legal_moves(Board& board) override;

private:
    void add_king_side_castling(Board& board);
    void add_queen_side_castling(Board& board);
};
