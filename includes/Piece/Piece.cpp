#include "Piece.hpp"
#include <iostream>

Piece::~Piece() {}

Piece::Piece(
    sf::RenderWindow& render_window,
    std::string texture_path,
    Coordinate coordinate,
    sf::FloatRect board_local_bound,
    COLOR color
)
    : render_window(render_window),
    texture_path(texture_path),
    coordinate(coordinate),
    board_local_bound(board_local_bound),
    color(color)
{
    square_length = board_local_bound.width / 8.f;

    setup_sprite();
    set_piece_scale(square_length / texture->getSize().x);
    set_coordinate(coordinate.row, coordinate.col);
}

void Piece::draw() {
    render_window.draw(sprite);
}

void Piece::set_coordinate(const Coordinate& coord) { set_coordinate(coord.row, coord.col); }

void Piece::set_coordinate(int row, int col) {
    if (!Coordinate::is_valid(row, col)) {
        std::cerr << "Error: Invalid coordinate {" << row << ", " << col << "}" << std::endl;
        return;
    }

    coordinate.row = row;
    coordinate.col = col;

    sf::Vector2f pos = {
        (col - 1) * square_length + board_local_bound.left + square_length / 2,
        (row - 1) * square_length + board_local_bound.top  + square_length / 2
    };

    sprite.setPosition(pos);
}

void Piece::set_piece_scale(float scale) {
    sprite.setScale({scale, scale});
}

void Piece::setup_sprite() {
    texture = std::make_shared<sf::Texture>();

    if(!texture->loadFromFile(texture_path)) {
        std::cerr << "Error: Failed to load image from file" << std::endl;
    }

    sprite.setTexture(*texture);

    sf::FloatRect local_bound = sprite.getLocalBounds();
    sprite.setOrigin(local_bound.width / 2.f, local_bound.height / 2.f);
}
