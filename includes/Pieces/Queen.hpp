/*#pragma once*/
/**/
/*#include "../Piece.hpp"*/
/**/
/*class Queen : public Piece {*/
/*public:*/
/*    Queen(*/
/*        sf::RenderWindow& render_window,*/
/*        sf::Vector2u coordinate,*/
/*        sf::Vector2f board_origin,*/
/*        float square_length,*/
/*        SIDE side*/
/*    )*/
/*        : Piece(*/
/*        render_window,*/
/*        side == SIDE::BLACK ? "assets/images/b_queen.png" : "assets/images/w_queen.png",*/
/*        coordinate,*/
/*        board_origin,*/
/*        square_length*/
/*    )*/
/*    {}*/
/**/
/*    std::vector<sf::Vector2u> get_possible_moves() override {*/
/*        std::vector<sf::Vector2u> possible_moves;*/
/*        possible_moves.reserve(27); // 27 = maximum possible moves*/
/**/
/*        for(auto& coord: get_lookup_coordinates()) {*/
/*            possible_moves.push_back(coord);*/
/*        }*/
/**/
/*        return possible_moves;*/
/*    }*/
/**/
/*private:*/
/*    const std::vector<sf::Vector2u> get_lookup_coordinates() const {*/
/*        std::vector<sf::Vector2u> lookup_coordinates(27);*/
/**/
/*        int col, row;*/
/**/
/*        // all left squares*/
/*        for(col = coordinate.x - 1; col > 0; col--) {*/
/*            lookup_coordinates.push_back({ col, coordinate.y });*/
/*        }*/
/**/
/*        // all right squares*/
/*        for(col = coordinate.x + 1; col < 9; col++) {*/
/*            lookup_coordinates.push_back({ col, coordinate.y });*/
/*        }*/
/**/
/*        // all top squares*/
/*        for(row = coordinate.y - 1; row > 0; row--) {*/
/*            lookup_coordinates.push_back({ coordinate.x, row });*/
/*        }*/
/**/
/*        // all bottom squares*/
/*        for(row = coordinate.x + 1; row < 9; row++) {*/
/*            lookup_coordinates.push_back({ coordinate.x, row });*/
/*        }*/
/**/
/*        // all top-left squares*/
/*        for(col = coordinate.x - 1, row = coordinate.y - 1; is_coordinate_in_bound(row, col); col--, row--) {*/
/*            lookup_coordinates.push_back({ col, row });*/
/*        }*/
/**/
/*        // all top-right squares*/
/*        for(col = coordinate.x + 1, row = coordinate.y - 1; is_coordinate_in_bound(row, col); col++, row--) {*/
/*            lookup_coordinates.push_back({ col, row });*/
/*        }*/
/**/
/*        // all bottom-right squares*/
/*        for(col = coordinate.x + 1, row = coordinate.y + 1; is_coordinate_in_bound(row, col); col++, row++) {*/
/*            lookup_coordinates.push_back({ col, row });*/
/*        }*/
/**/
/*        // all bottom-left squares*/
/*        for(col = coordinate.x - 1, row = coordinate.y + 1; is_coordinate_in_bound(row, col); col--, row++) {*/
/*            lookup_coordinates.push_back({ col, row });*/
/*        }*/
/**/
/*        return lookup_coordinates;*/
/*    }*/
/*};*/
