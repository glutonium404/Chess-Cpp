#pragma once

#include "../Coordinate/Coordinate.hpp"
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Vector2.hpp>
#include <memory>
#include <string>
#include <SFML/Graphics.hpp>

class Board;

class Piece {
public:
    enum class COLOR { BLACK, WHITE };
    enum class TYPE  { KING, QUEEN, ROOK, BISHOP, KNIGHT, PAWN };

    sf::RenderWindow&       render_window;
    std::string             texture_path;
    std::vector<Coordinate> legal_moves;
    sf::FloatRect           board_local_bound;
    Piece::COLOR            color;
    std::vector<Piece*>     attacked_by;

    bool                    is_alive = true;
    float                   square_length;

    bool                    is_pinned = false;
    Coordinate              pinned_dir = Coordinate(9, 9);

    Piece(
        sf::RenderWindow& render_window,
        std::string texture_path,
        Coordinate coordinate,
        sf::FloatRect board_local_bound,
        COLOR color
    );

    virtual ~Piece();

    virtual TYPE get_type() const = 0;
    virtual void set_legal_moves(Board& board) = 0;

    void draw();
    void set_coordinate(int row, int col);
    void set_coordinate(const Coordinate& coord);
    void set_piece_scale(float scale);
    void set_controlled_squares(Board& board, Coordinate& coord) const;

    bool is_white() const;
    bool is_black() const;

    Coordinate get_coordinate() const;

private:
    std::shared_ptr<sf::Texture> texture;
    sf::Sprite                   sprite;
    bool                         initial_setup = true;

    void setup_sprite();

protected:
    Coordinate coordinate;
    bool       has_moved = false;

    bool add_common_legal_moves(Board& board, const int row, const int col);
    std::vector<Coordinate> get_check_elimination_moves(const Piece* const attacker, const std::shared_ptr<Piece>& king);
};
