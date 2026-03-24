#pragma once

#include "../Piece/Piece.hpp"
#include "../Square/Square.hpp"
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Mouse.hpp>
#include <memory>
#include <array>
#include <vector>

class Board {
public:
    float             square_length;
    sf::RenderWindow& render_window;
    sf::FloatRect     board_local_bound;
    Piece::COLOR      current_turn = Piece::COLOR::WHITE;

    Board(float board_width, sf::RenderWindow& render_window);

    void handle_event(sf::Event& event);
    void draw();

    Square& get_square(const Coordinate& coordinate);
    Square& get_square(int row, int col);

    const std::shared_ptr<Piece>& get_king(const Piece::COLOR color) const;

private:
    sf::Mouse                               mouse;
    std::vector<Coordinate>                 highlighted_coord;
    std::array<std::array<Square, 8>, 8>    squares;

    std::vector<std::shared_ptr<Piece>>     b_pieces;
    std::vector<std::shared_ptr<Piece>>     w_pieces;

    Square* selected_square = nullptr;

    bool    is_mouse_clicked(sf::Event& event) const;

    void    handle_click(sf::Event& event);
    void    set_board_local_bound(float& board_width);
    void    set_squares();
    void    set_pieces();
    void    add_pieces_to_board();
    void    highlight_legal_moves(const std::shared_ptr<Piece>& piece);
    void    remove_existing_highilights();
    void    make_move(Square& new_square);
    void    toggle_player();
    void    update_legal_moves();
    void    reset_variables();
    void    empty_square_clicked();
    void    highlighted_square_clicked(Square& new_selected_square);
    void    own_piece_clicked(Square& new_selected_square);
    void    manage_check_highlights();

    Coordinate  get_clicked_coordinate();

    std::shared_ptr<Piece> make_pawn  (int row, int col, Piece::COLOR color);
    std::shared_ptr<Piece> make_king  (int row, int col, Piece::COLOR color);
    std::shared_ptr<Piece> make_queen (int row, int col, Piece::COLOR color);
    std::shared_ptr<Piece> make_rook  (int row, int col, Piece::COLOR color);
    std::shared_ptr<Piece> make_knight(int row, int col, Piece::COLOR color);
    std::shared_ptr<Piece> make_bishop(int row, int col, Piece::COLOR color);
};
