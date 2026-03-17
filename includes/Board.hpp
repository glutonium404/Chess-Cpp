#pragma once

#include "Pieces/Bishop.hpp"
#include "Pieces/King.hpp"
#include "Pieces/Knight.hpp"
#include "Pieces/Pawn.hpp"
#include "Pieces/Queen.hpp"
#include "Pieces/Rook.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <array>
#include <memory>

class Board {
public:
    float board_width;
    float square_length;
    sf::RenderWindow& render_window;
    sf::Vector2f board_origin;
    std::vector<std::unique_ptr<Piece>> b_pieces;
    std::vector<std::unique_ptr<Piece>> w_pieces;

    Board(float board_width, sf::RenderWindow& render_window)
        : board_width(board_width),
        render_window(render_window)
    {
        w_pieces.reserve(16);
        b_pieces.reserve(16);

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

        for(int i=0; i<b_pieces.size(); i++) {
            if(b_pieces[i]->is_alive) {
                /*highlight_possible_moves(b_pieces[i]);*/
                b_pieces[i]->draw();
            }
        }

        for(int i=0; i<w_pieces.size(); i++) {
            if(w_pieces[i]->is_alive) {
                /*highlight_possible_moves(w_pieces[i]);*/
                w_pieces[i]->draw();
            }
        }
    }

    void highlight_possible_moves(const std::unique_ptr<Piece>& piece) {
        for(auto& coord: piece->get_possible_moves()) {
            squares[coord.row - 1][coord.col - 1].setOutlineThickness(1.f);
            squares[coord.row - 1][coord.col - 1].setOutlineColor(sf::Color::Black);
            squares[coord.row - 1][coord.col - 1].setFillColor(highlight_square_color);
            render_window.draw(squares[coord.row - 1][coord.col - 1]);
        }
    }

private:
    std::array<std::array<sf::RectangleShape, 8>, 8> squares;

    sf::Color light_square_color = sf::Color(251,194,115);
    sf::Color dark_square_color = sf::Color(149,83,59);
    sf::Color highlight_square_color = sf::Color(140, 194, 255);

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

                sf::Color color = flag ? light_square_color : dark_square_color;

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
        b_pieces.push_back( make_rook   (1, 1, SIDE::BLACK) );
        b_pieces.push_back( make_knight (1, 2, SIDE::BLACK) );
        b_pieces.push_back( make_bishop (1, 3, SIDE::BLACK) );
        b_pieces.push_back( make_queen  (1, 4, SIDE::BLACK) );
        b_pieces.push_back( make_king   (1, 5, SIDE::BLACK) );
        b_pieces.push_back( make_bishop (1, 6, SIDE::BLACK) );
        b_pieces.push_back( make_knight (1, 7, SIDE::BLACK) );
        b_pieces.push_back( make_rook   (1, 8, SIDE::BLACK) );

        b_pieces.push_back( make_pawn   (2, 1, SIDE::BLACK) );
        b_pieces.push_back( make_pawn   (2, 2, SIDE::BLACK) );
        b_pieces.push_back( make_pawn   (2, 3, SIDE::BLACK) );
        b_pieces.push_back( make_pawn   (2, 4, SIDE::BLACK) );
        b_pieces.push_back( make_pawn   (2, 5, SIDE::BLACK) );
        b_pieces.push_back( make_pawn   (2, 6, SIDE::BLACK) );
        b_pieces.push_back( make_pawn   (2, 7, SIDE::BLACK) );
        b_pieces.push_back( make_pawn   (2, 8, SIDE::BLACK) );



        w_pieces.push_back( make_rook   (8, 1, SIDE::WHITE) );
        w_pieces.push_back( make_knight (8, 2, SIDE::WHITE) );
        w_pieces.push_back( make_bishop (8, 3, SIDE::WHITE) );
        w_pieces.push_back( make_queen  (8, 4, SIDE::WHITE) );
        w_pieces.push_back( make_king   (8, 5, SIDE::WHITE) );
        w_pieces.push_back( make_bishop (8, 6, SIDE::WHITE) );
        w_pieces.push_back( make_knight (8, 7, SIDE::WHITE) );
        w_pieces.push_back( make_rook   (8, 8, SIDE::WHITE) );

        w_pieces.push_back( make_pawn(7, 1, SIDE::WHITE) );
        w_pieces.push_back( make_pawn(7, 2, SIDE::WHITE) );
        w_pieces.push_back( make_pawn(7, 3, SIDE::WHITE) );
        w_pieces.push_back( make_pawn(7, 4, SIDE::WHITE) );
        w_pieces.push_back( make_pawn(7, 5, SIDE::WHITE) );
        w_pieces.push_back( make_pawn(7, 6, SIDE::WHITE) );
        w_pieces.push_back( make_pawn(7, 7, SIDE::WHITE) );
        w_pieces.push_back( make_pawn(7, 8, SIDE::WHITE) );
    }

    std::unique_ptr<Piece> make_pawn(int row, int col, SIDE side) {
        return std::make_unique<Pawn>(
            render_window, Coordinate(row, col), board_origin, square_length, side
        );
    }

    std::unique_ptr<Piece> make_king(int row, int col, SIDE side) {
        return std::make_unique<King>(
            render_window, Coordinate(row, col), board_origin, square_length, side
        );
    }

    std::unique_ptr<Piece> make_queen(int row, int col, SIDE side) {
        return std::make_unique<Queen>(
            render_window, Coordinate(row, col), board_origin, square_length, side
        );
    }
    std::unique_ptr<Piece> make_rook(int row, int col, SIDE side) {
        return std::make_unique<Rook>(
            render_window, Coordinate(row, col), board_origin, square_length, side
        );
    }
    std::unique_ptr<Piece> make_knight(int row, int col, SIDE side) {
        return std::make_unique<Knight>(
            render_window, Coordinate(row, col), board_origin, square_length, side
        );
    }
    std::unique_ptr<Piece> make_bishop(int row, int col, SIDE side) {
        return std::make_unique<Bishop>(
            render_window, Coordinate(row, col), board_origin, square_length, side
        );
    }
};
