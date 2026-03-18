#pragma once

#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Vector2.hpp>
#include <memory>
#include <string>
#include <SFML/Graphics.hpp>

enum SIDE {
    BLACK,
    WHITE
};

struct Coordinate {
    int row = 0;
    int col = 0;

    Coordinate(int row, int col);
    bool operator==(const Coordinate& other) const;
};

class Piece {
public:
    sf::RenderWindow&   render_window;
    std::string         texture_path;
    Coordinate          coordinate;
    sf::FloatRect       board_local_bound;
    bool                is_alive = true;
    float               square_length;

    Piece(
        sf::RenderWindow& render_window,
        std::string texture_path,
        Coordinate coordinate,
        sf::FloatRect board_local_bound
    );

    virtual ~Piece();
    virtual std::vector<Coordinate> get_possible_moves() = 0;

    void draw();
    void set_coordinate(int row, int col);
    void set_piece_scale(float scale);

private:
    std::shared_ptr<sf::Texture> texture;
    sf::Sprite                   sprite;

    void setup_sprite();

protected:
    bool is_coordinate_in_bound(const Coordinate& coordinate) const;
    bool is_coordinate_in_bound(const int row, const int col) const;
};
