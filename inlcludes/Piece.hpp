#pragma once

#include <SFML/System/Vector2.hpp>
#include <iostream>
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

    Coordinate(int row, int col): row(row), col(col) {};
};

class Piece {
public:
    sf::RenderWindow& render_window;
    std::string texture_path;
    Coordinate coordinate;
    sf::Vector2f board_origin;
    bool is_alive = true;
    float square_length;

    Piece(
        sf::RenderWindow& render_window,
        std::string texture_path,
        Coordinate coordinate,
        sf::Vector2f board_origin,
        float square_length
    )
        : render_window(render_window),
        texture_path(texture_path),
        coordinate(coordinate),
        board_origin(board_origin),
        square_length(square_length)
    {
        setup_sprite();
        set_piece_scale((square_length - 10.0) / texture->getSize().x);
        set_coordinate(coordinate.row, coordinate.col);
    }

    virtual ~Piece() {}

    virtual std::vector<Coordinate> get_possible_moves() = 0;

    void draw() {
        render_window.draw(sprite);
    }

    void set_coordinate(int row, int col) {

        coordinate.row = row;
        coordinate.col = col;

        if (!is_coordinate_in_bound(row, col)) {
            std::cerr << "Error: Invalid coordinate {" << row << ", " << col << "}" << std::endl;
            return;
        }

        sf::Vector2f pos = {
            (col - 1) * square_length + board_origin.x + square_length / 2,
            (row - 1) * square_length + board_origin.y + square_length / 2
        };

        sprite.setPosition(pos);
    }

    void set_piece_scale(float scale) {
        sprite.setScale({scale, scale});
    }

private:
    std::shared_ptr<sf::Texture> texture;
    sf::Sprite sprite;

    void setup_sprite() {
        texture = std::make_shared<sf::Texture>();

        if(!texture->loadFromFile(texture_path)) {
            std::cerr << "Error: Failed to load image from file" << std::endl;
        }

        sprite.setTexture(*texture);

        sf::FloatRect local_bound = sprite.getLocalBounds();
        sprite.setOrigin(local_bound.width / 2.f, local_bound.height / 2.f);
    }

protected:
    bool is_coordinate_in_bound(const Coordinate& coordinate) const {
        return !(coordinate.row < 1 || coordinate.row > 8 || coordinate.col < 1 || coordinate.col > 8);
    }

    bool is_coordinate_in_bound(const int row, const int col) const {
        return !(row < 1 || row > 8 || col < 1 || col > 8);
    }
};
