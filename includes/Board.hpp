#pragma once

#include "Piece.hpp"
#include "Pieces/Bishop.hpp"
#include "Pieces/King.hpp"
#include "Pieces/Knight.hpp"
#include "Pieces/Pawn.hpp"
#include "Pieces/Queen.hpp"
#include "Pieces/Rook.hpp"
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/System/Vector2.hpp>
#include <array>
#include <iostream>
#include <memory>
#include <tuple>

struct Square {
public:
    enum class TYPE { LIGHT, DARK };

    inline static const sf::Color light_color     = sf::Color(251,194,115);
    inline static const sf::Color dark_color      = sf::Color(149,83,59);
    inline static const sf::Color highlight_color = sf::Color(140, 194, 255);

    TYPE type;
    sf::RectangleShape shape;


    void highlight() {
        shape.setOutlineThickness(1.f);
        shape.setOutlineColor(sf::Color::Black);
        shape.setFillColor(Square::highlight_color);
    }

    void unhighlight() {
        shape.setOutlineThickness(0.f);
        shape.setOutlineColor(sf::Color::Transparent);
        shape.setFillColor(
            type == TYPE::LIGHT ? Square::light_color : Square::dark_color
        );
    }

private:
};

class Board {
public:
    float                               board_width;
    float                               square_length;
    sf::RenderWindow&                   render_window;
    sf::FloatRect                       board_local_bound;
    std::vector<std::unique_ptr<Piece>> b_pieces;
    std::vector<std::unique_ptr<Piece>> w_pieces;

    Board(float board_width, sf::RenderWindow& render_window)
        : board_width(board_width),
        render_window(render_window)
    {
        w_pieces.reserve(16);
        b_pieces.reserve(16);

        square_length = board_width / 8.0;

        set_board_local_bound();
        set_squares_shapes();
        set_pieces();
    }

    void draw() {
        handle_click();

        for(auto& square_array: squares) {
            for(auto& square: square_array) {
                render_window.draw(square.shape);
            }
        }

        for(int i=0; i<b_pieces.size(); i++) {
            if(b_pieces[i]->is_alive) {
                b_pieces[i]->draw();
            }
        }

        for(int i=0; i<w_pieces.size(); i++) {
            if(w_pieces[i]->is_alive) {
                w_pieces[i]->draw();
            }
        }
    }

    void highlight_possible_moves(const std::unique_ptr<Piece>& piece) {
        remove_existing_highilights();

        highlighted_coord = piece->get_possible_moves();

        for(auto& coord: highlighted_coord) {
            auto& highlighted_square = squares[coord.row - 1][coord.col - 1];
            highlighted_square.highlight();
            render_window.draw(highlighted_square.shape);
        }
    }

    void remove_existing_highilights() {
        for(auto& coord: highlighted_coord) {
            auto& highlighted_square = squares[coord.row - 1][coord.col - 1];
            highlighted_square.unhighlight();
            render_window.draw(highlighted_square.shape);
        }
    }

private:
    std::array<std::array<Square, 8>, 8>    squares;
    std::vector<Coordinate>                 highlighted_coord;

    sf::Mouse mouse;

    // the index (int) being negative = no piece selected
    std::tuple<SIDE, int> selected_piece = std::make_tuple(SIDE::BLACK, -1);

    void handle_click() {
        if(is_mouse_clicked()) {
            set_selected_piece();

            int&  selected_index  = std::get<1>(selected_piece);
            SIDE& selected_side   = std::get<0>(selected_piece);
            auto& selected_vector = (selected_side == SIDE::WHITE) ? w_pieces : b_pieces;

            if(selected_index != -1) {
                highlight_possible_moves(selected_vector[selected_index]);
            }else {
                remove_existing_highilights();
            }
        }
    }

    bool is_mouse_clicked() {
        auto pos = mouse.getPosition(render_window);
        return mouse.isButtonPressed(mouse.Left) && board_local_bound.contains(pos.x, pos.y);
    }

    void set_selected_piece() {
        auto clicked_coordinate = get_clicked_coordinate();

        for(int i=0; i<w_pieces.size(); i++) {
            if(!w_pieces[i]->is_alive) continue;

            if(w_pieces[i]->coordinate == clicked_coordinate) {
                selected_piece = std::make_tuple(SIDE::WHITE, i);
                return;
            }

            std::get<1>(selected_piece) = -1;
        }

        for(int i=0; i<b_pieces.size(); i++) {
            if(!b_pieces[i]->is_alive) continue;

            if(b_pieces[i]->coordinate == clicked_coordinate) {
                selected_piece = std::make_tuple(SIDE::BLACK, i);
                return;
            }
        }

        selected_piece = std::make_tuple(SIDE::BLACK, -1);
    }

    Coordinate get_clicked_coordinate() {
        // get mouse position relative to the window
        sf::Vector2i mouse_pixel_pos = sf::Mouse::getPosition(render_window);

        // map pixels to world coordinates (handles scaling/views)
        sf::Vector2f mouse_pos = render_window.mapPixelToCoords(mouse_pixel_pos);

        // calculate 1 based coordinates
        int row = static_cast<int>((mouse_pos.y - board_local_bound.top) / square_length) + 1;
        int col = static_cast<int>((mouse_pos.x - board_local_bound.left) / square_length) + 1;

        return Coordinate(row, col);
    }

    void set_board_local_bound() {
        sf::Vector2u window_dim = render_window.getSize();
        float offset_x = (window_dim.x - board_width) / 2.0;
        float offset_y = (window_dim.y - board_width) / 2.0;


        board_local_bound.left      = offset_x;
        board_local_bound.top       = offset_y;
        board_local_bound.width     = square_length * 8.f;
        board_local_bound.height    = square_length * 8.f;
    }

    void set_squares_shapes() {
        bool flag = true;

        for(int i=0; i<8; i++) {
            for(int j=0; j<8; j++) {

                sf::Color color = flag ? Square::light_color : Square::dark_color;

                sf::Vector2f position = {
                    j * square_length + board_local_bound.left,
                    i * square_length + board_local_bound.top
                };

                squares[i][j].shape = sf::RectangleShape({square_length, square_length});

                squares[i][j].shape.setFillColor(color);
                squares[i][j].shape.setPosition(position);

                squares[i][j].type = flag ? Square::TYPE::LIGHT : Square::TYPE::DARK;

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
            render_window, Coordinate(row, col), board_local_bound, square_length, side
        );
    }

    std::unique_ptr<Piece> make_king(int row, int col, SIDE side) {
        return std::make_unique<King>(
            render_window, Coordinate(row, col), board_local_bound, square_length, side
        );
    }

    std::unique_ptr<Piece> make_queen(int row, int col, SIDE side) {
        return std::make_unique<Queen>(
            render_window, Coordinate(row, col), board_local_bound, square_length, side
        );
    }
    std::unique_ptr<Piece> make_rook(int row, int col, SIDE side) {
        return std::make_unique<Rook>(
            render_window, Coordinate(row, col), board_local_bound, square_length, side
        );
    }
    std::unique_ptr<Piece> make_knight(int row, int col, SIDE side) {
        return std::make_unique<Knight>(
            render_window, Coordinate(row, col), board_local_bound, square_length, side
        );
    }
    std::unique_ptr<Piece> make_bishop(int row, int col, SIDE side) {
        return std::make_unique<Bishop>(
            render_window, Coordinate(row, col), board_local_bound, square_length, side
        );
    }
};
