#pragma once

#include <iostream>
#include <string>
#include <SFML/Graphics.hpp>

enum SIDE {
    BLACK,
    WHITE
};

class Piece {
public:
    sf::RenderWindow& render_window;
    std::string texture_path;
    sf::Vector2u coordinate;
    sf::Vector2f board_origin;
    bool is_alive = true;
    float square_length;

    Piece(
        sf::RenderWindow& render_window,
        std::string texture_path,
        sf::Vector2u coordinate,
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
        set_piece_scale((square_length - 5.0) / texture.getSize().x);
        set_coordinate(coordinate.x, coordinate.y);
    }

    void draw() {
        render_window.draw(sprite);
    }

    void set_coordinate(int row, int col) {
        if(row <= 0 || col <= 0) {
            std::cerr << "Error: Invalid coordinate {" << row << ", " << col << "}" << std::endl;
        }

        sf::Vector2f pos = {
            (col - 1) * square_length + board_origin.x,
            (row - 1) * square_length + board_origin.y
        };

        sprite.setPosition(pos);
    }

    void set_piece_scale(float scale) {
        sprite.setScale({scale, scale});
    }

private:
    sf::Texture texture;
    sf::Sprite sprite;

    void setup_sprite() {
        if(!texture.loadFromFile(texture_path)) {
            std::cerr << "Error: Failed to load image from file" << std::endl;
        }
        sprite.setTexture(texture);
    }
};
