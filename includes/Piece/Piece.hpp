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

    sf::RenderWindow&   render_window;
    std::string         texture_path;
    sf::FloatRect       board_local_bound;
    Piece::COLOR        color;
    bool                is_alive = true;
    float               square_length;

    Piece(
        sf::RenderWindow& render_window,
        std::string texture_path,
        Coordinate coordinate,
        sf::FloatRect board_local_bound,
        COLOR color
    );

    virtual ~Piece();
    virtual TYPE                    get_type() const = 0;
    virtual std::vector<Coordinate> get_legal_moves(Board& board) = 0;

    void draw();
    void set_coordinate(int row, int col);
    void set_coordinate(const Coordinate& coord);
    void set_piece_scale(float scale);

    Coordinate get_coordinate() const;

private:
    std::shared_ptr<sf::Texture> texture;
    sf::Sprite                   sprite;

    void setup_sprite();

protected:
    Coordinate coordinate;

    bool add_common_legal_moves(Board& board, std::vector<Coordinate>& legal_moves, const int row, const int col) const;
};
