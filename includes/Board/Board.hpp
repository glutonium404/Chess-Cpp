#pragma once

#include "../Piece/Piece.hpp"
#include "../Square/Square.hpp"
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Mouse.hpp>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <array>
#include <unordered_map>
#include <vector>

class Board {
public:
    float             square_length;
    sf::RenderWindow& render_window;
    sf::FloatRect     board_local_bound;
    Piece::COLOR      current_turn = Piece::COLOR::WHITE;

    std::size_t       castling_right = 15; // using bitset. 15 = 1111
    int               en_passant_file = -1; // negative = no en passsant available yet

    enum class CR { WK = 1, WQ = 2, BK = 4, BQ = 8 }; // Castling Rights. Corresponds to bitset

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

    // Zobrist hasing constants
    uint64_t piece_table[2][6][64];
    uint64_t castling_right_table[16];
    uint64_t en_passant_file_table[8];
    uint64_t side_to_move;
    uint64_t hash = 0;

    std::unordered_map<uint64_t, unsigned int> repetition_list;

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
    void    update_pinned_pieces();
    void    reset_variables();
    void    empty_square_clicked();
    void    highlighted_square_clicked(Square& new_selected_square);
    void    own_piece_clicked(Square& new_selected_square);
    void    manage_check_highlights();

    void    populate_look_up_tables();
    void    update_zobrist_hash();
    void    update_zobrist_variables(Square& new_square);
    void    check_game_state();

    Coordinate  get_clicked_coordinate();

    std::shared_ptr<Piece> make_pawn  (int row, int col, Piece::COLOR color);
    std::shared_ptr<Piece> make_king  (int row, int col, Piece::COLOR color);
    std::shared_ptr<Piece> make_queen (int row, int col, Piece::COLOR color);
    std::shared_ptr<Piece> make_rook  (int row, int col, Piece::COLOR color);
    std::shared_ptr<Piece> make_knight(int row, int col, Piece::COLOR color);
    std::shared_ptr<Piece> make_bishop(int row, int col, Piece::COLOR color);
};
