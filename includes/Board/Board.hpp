#pragma once

#include "../Piece/Piece.hpp"
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Mouse.hpp>
#include <array>
#include <memory>

class Board {
public:
    struct Square {
        enum class COLOR { LIGHT, DARK };

        inline static const sf::Color light_color      = sf::Color(235, 236, 208);
        inline static const sf::Color dark_color       = sf::Color(115, 149, 82);

        inline static const sf::Color light_highlight  = sf::Color(245, 246, 130);
        inline static const sf::Color dark_highlight   = sf::Color(185, 202, 67);
        inline static const sf::Color red_highlight    = sf::Color(149, 82, 82);

        COLOR       type;
        Coordinate  coordinate = Coordinate(0, 0);

        sf::RectangleShape shape;

        bool is_red() const;

        void highlight(const Board& board);
        void highlight(const sf::Color& color);

        void unhighlight(const Board& board);
    };

    bool                                is_there_check      = false;
    float                               square_length;
    sf::RenderWindow&                   render_window;
    sf::FloatRect                       board_local_bound;
    std::vector<std::unique_ptr<Piece>> b_pieces;
    std::vector<std::unique_ptr<Piece>> w_pieces;

    Board(float board_width, sf::RenderWindow& render_window);

    void handle_event(sf::Event& event);
    void draw();

private:
    Piece::COLOR                            current_turn = Piece::COLOR::WHITE;

    sf::Mouse                               mouse;
    std::vector<Coordinate>                 highlighted_coord;
    std::array<std::array<Square, 8>, 8>    squares;

    Coordinate current_check_coord = Coordinate(-1, -1);

    struct SelectedPiece {
        bool        is_any_selected = false;
        int         index;
        Piece::COLOR side;

        std::unique_ptr<Piece>& piece(Board& board) const;
    } selected_piece;

    int         is_check() const;
    bool        is_mouse_clicked(sf::Event& event) const;
    bool        piece_clicked();
    void        handle_click(sf::Event& event);
    void        highlighted_square_clicked();
    void        set_board_local_bound(float& board_width);
    void        set_squares();
    void        set_pieces();
    void        highlight_possible_moves(const std::unique_ptr<Piece>& piece);
    void        remove_existing_highilights();
    void        make_move(Coordinate& new_coordinate);
    Square&     get_square(const Coordinate& coordinate);
    Square&     get_square(int row, int col);
    Coordinate  get_clicked_coordinate();

    std::unique_ptr<Piece> make_pawn  (int row, int col, Piece::COLOR side);
    std::unique_ptr<Piece> make_king  (int row, int col, Piece::COLOR side);
    std::unique_ptr<Piece> make_queen (int row, int col, Piece::COLOR side);
    std::unique_ptr<Piece> make_rook  (int row, int col, Piece::COLOR side);
    std::unique_ptr<Piece> make_knight(int row, int col, Piece::COLOR side);
    std::unique_ptr<Piece> make_bishop(int row, int col, Piece::COLOR side);

    const std::unique_ptr<Piece>& get_player_king() const;
};
