#pragma once

#include "../Piece.hpp"

class Pawn : public Piece {
public:
    Pawn(
        sf::RenderWindow& render_window,
        Coordinate coordinate,
        sf::Vector2f board_origin,
        float square_length,
        SIDE side
    )
        : Piece(
        render_window,
        side == SIDE::BLACK ? "assets/images/pawn-b.png" : "assets/images/pawn-w.png",
        coordinate,
        board_origin,
        square_length
    )
    {
        // direction dictates the forward direction of the merching pawn
        // when pawn moves forward, either it's row value increases or decreases based on it's forward direction
        // -ve: forward direction = bottom -> top
        // +ve: forward direction = top -> bottom
        direction = coordinate.row > 4 ? -1 : 1;
    }

    std::vector<Coordinate> get_possible_moves() override {
        std::vector<Coordinate> possible_moves;

        possible_moves.push_back( Coordinate( coordinate.row + (1 * direction), coordinate.col ) );

        if(is_first_move)
            possible_moves.push_back( Coordinate( coordinate.row + (2 * direction), coordinate.col ) );

        return possible_moves;
    }

private:
    bool is_first_move = true;
    int direction;
};
