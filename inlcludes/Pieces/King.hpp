#pragma once

#include "../Piece.hpp"

class King : public Piece {
public:
    King(
        sf::RenderWindow& render_window,
        sf::Vector2u coordinate,
        sf::Vector2f board_origin,
        float square_length,
        SIDE side
    )
        : Piece(
        render_window,
        side == SIDE::BLACK ? "assets/images/b_king.png" : "assets/images/w_king.png",
        coordinate,
        board_origin,
        square_length
    )
    {}

    std::vector<sf::Vector2u> get_possible_moves() override {
        std::vector<sf::Vector2u> possible_moves;
        possible_moves.reserve(8);

        for(auto& coord: get_lookup_coordinates()) {
            if(!is_coordinate_in_bound(coord)) continue;

            possible_moves.push_back(coord);
        }

        return possible_moves;
    }

private:
    const std::vector<sf::Vector2u> get_lookup_coordinates() const {
        std::vector<sf::Vector2u> lookup_coordinates(8);

        lookup_coordinates[0] = { coordinate.x - 1, coordinate.y - 1 }; // top left
        lookup_coordinates[1] = { coordinate.x    , coordinate.y - 1 }; // top middle
        lookup_coordinates[2] = { coordinate.x + 1, coordinate.y - 1 }; // top right

        lookup_coordinates[3] = { coordinate.x - 1, coordinate.y + 1 }; // bottom left
        lookup_coordinates[4] = { coordinate.x    , coordinate.y + 1 }; // bottom middle
        lookup_coordinates[5] = { coordinate.x + 1, coordinate.y + 1 }; // bottom right

        lookup_coordinates[6] = { coordinate.x - 1, coordinate.y     }; // center left
        lookup_coordinates[7] = { coordinate.x + 1, coordinate.y     }; // center right

        return lookup_coordinates;
    }
};
