#pragma once

#include "../Piece/Piece.hpp"
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Mouse.hpp>
#include <array>

class Board {
public:
    struct Square {
        enum class TYPE { LIGHT, DARK };

        inline static const sf::Color light_color     = sf::Color(251,194,115);
        inline static const sf::Color dark_color      = sf::Color(149,83,59);
        inline static const sf::Color highlight_color = sf::Color(140, 194, 255);

        TYPE type;
        sf::RectangleShape shape;

        void highlight();
        void unhighlight();
    };

    float                               board_width;
    float                               square_length;
    sf::RenderWindow&                   render_window;
    sf::FloatRect                       board_local_bound;
    std::vector<std::unique_ptr<Piece>> b_pieces;
    std::vector<std::unique_ptr<Piece>> w_pieces;

    Board(float board_width, sf::RenderWindow& render_window);

    void draw();

    void highlight_possible_moves(const std::unique_ptr<Piece>& piece);

    void remove_existing_highilights();

private:
    sf::Mouse                               mouse;
    std::vector<Coordinate>                 highlighted_coord;
    std::array<std::array<Square, 8>, 8>    squares;

    struct SelectedPiece {
        bool is_any_selected = false;
        SIDE side;
        int index;
    } selected_piece;

    void handle_click();
    bool is_mouse_clicked() const;
    void set_selected_piece();
    Coordinate get_clicked_coordinate();
    void set_board_local_bound();
    void set_squares_shapes();
    void set_pieces();

    std::unique_ptr<Piece> make_pawn(int row, int col, SIDE side);
    std::unique_ptr<Piece> make_king(int row, int col, SIDE side);
    std::unique_ptr<Piece> make_queen(int row, int col, SIDE side);
    std::unique_ptr<Piece> make_rook(int row, int col, SIDE side);
    std::unique_ptr<Piece> make_knight(int row, int col, SIDE side);
    std::unique_ptr<Piece> make_bishop(int row, int col, SIDE side);
};
