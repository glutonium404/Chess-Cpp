#pragma once

#include "Pieces/King.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <array>

class Board {
public:
    float board_width;
    float square_length;
    sf::RenderWindow& render_window;
    sf::Vector2f board_origin;
    std::vector<Piece> b_pieces;
    std::vector<Piece> w_pieces;

    Board(float board_width, sf::RenderWindow& render_window)
        : board_width(board_width),
        render_window(render_window)
    {
        square_length = board_width / 8.0;
        set_board_origin();
        set_squares_shapes();
        set_pieces();
    }

    void draw() {
        for(auto& square_array: squares) {
            for(auto& square: square_array) {
                render_window.draw(square);
            }
        }

        for(int i=0; i<w_pieces.size(); i++) {
            if(b_pieces[i].is_alive) { b_pieces[i].draw(); }
            if(w_pieces[i].is_alive) { w_pieces[i].draw(); }
        }
    }

private:
    std::array<std::array<sf::RectangleShape, 8>, 8> squares;

    sf::Color square_color1 = sf::Color(238, 238, 210);
    sf::Color square_color2 = sf::Color(118, 150, 86);

    void set_board_origin() {
        sf::Vector2u window_dim = render_window.getSize();
        float offset_x = (window_dim.x - board_width) / 2.0;
        float offset_y = (window_dim.y - board_width) / 2.0;
        board_origin = { offset_x, offset_y };
    }

    void set_squares_shapes() {
        bool flag = true;

        for(int i=0; i<8; i++) {
            for(int j=0; j<8; j++) {
                squares[i][j] = sf::RectangleShape({square_length, square_length});

                sf::Color color = flag ? square_color1 : square_color2;

                sf::Vector2f position = {
                    j * square_length + board_origin.x,
                    i * square_length + board_origin.y
                };

                squares[i][j].setFillColor(color);
                squares[i][j].setPosition(position);

                if(j != 7) { flag = !flag; }
            }
        }
    }

    void set_pieces() {
        King w_k = King(
            render_window,
            {8, 5},
            board_origin,
            square_length,
            SIDE::WHITE
        );

        King b_k = King(
            render_window,
            {1, 5},
            board_origin,
            square_length,
            SIDE::BLACK
        );


        
    }
};
