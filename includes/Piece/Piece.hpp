#pragma once

#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Vector2.hpp>
#include <memory>
#include <string>
#include <SFML/Graphics.hpp>

struct Coordinate {
    int row = 0;
    int col = 0;

    Coordinate(int row, int col);
    bool operator==(const Coordinate& other) const;
    bool operator!=(const Coordinate& other) const;
    std::string log() {
        std::string msg = "{";
        msg += std::to_string(row);
        msg += ", ";
        msg += std::to_string(col);;
        msg += "}";
        return msg;
    }
};

class Piece {
public:
    enum class COLOR { BLACK, WHITE };
    enum class TYPE  { KING, QUEEN, ROOK, BISHOP, KNIGHT, PAWN };

    sf::RenderWindow&   render_window;
    std::string         texture_path;
    Coordinate          coordinate;
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
    virtual std::vector<Coordinate> get_possible_moves() = 0;

    void draw();
    void set_coordinate(int row, int col);
    void set_coordinate(const Coordinate& coord);
    void set_piece_scale(float scale);

private:
    std::shared_ptr<sf::Texture> texture;
    sf::Sprite                   sprite;

    void setup_sprite();

protected:
    bool is_coordinate_in_bound(const Coordinate& coordinate) const;
    bool is_coordinate_in_bound(const int row, const int col) const;
};
