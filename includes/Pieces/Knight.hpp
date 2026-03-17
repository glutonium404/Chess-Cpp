#pragma once

#include "../Piece.hpp"

class Knight : public Piece {
public:
    Knight(
        sf::RenderWindow& render_window,
        Coordinate coordinate,
        sf::Vector2f board_origin,
        float square_length,
        SIDE side
    )
        : Piece(
        render_window,
        side == SIDE::BLACK ? "assets/images/knight-b.png" : "assets/images/knight-w.png",
        coordinate,
        board_origin,
        square_length
    )
    {}

    std::vector<Coordinate> get_possible_moves() override {
        std::vector<Coordinate> possible_moves;
        possible_moves.reserve(8); // 8 = maximum possible moves

        for(auto& coord: get_lookup_coordinates()) {
            possible_moves.push_back(coord);
        }

        return possible_moves;
    }

private:
    const std::vector<Coordinate> get_lookup_coordinates() const {
        std::vector<Coordinate> lookup_coordinates;
        lookup_coordinates.reserve(8);

        lookup_coordinates.push_back( Coordinate( coordinate.row - 2, coordinate.col - 1 ) );
        lookup_coordinates.push_back( Coordinate( coordinate.row - 2, coordinate.col + 1 ) );

        lookup_coordinates.push_back( Coordinate( coordinate.row + 2, coordinate.col - 1 ) );
        lookup_coordinates.push_back( Coordinate( coordinate.row + 2, coordinate.col + 1 ) );

        lookup_coordinates.push_back( Coordinate( coordinate.row + 1, coordinate.col - 2 ) );
        lookup_coordinates.push_back( Coordinate( coordinate.row - 1, coordinate.col - 2 ) );

        lookup_coordinates.push_back( Coordinate( coordinate.row + 1, coordinate.col + 2 ) );
        lookup_coordinates.push_back( Coordinate( coordinate.row - 1, coordinate.col + 2 ) );

        return lookup_coordinates;
    }
};
