#include "Piece.hpp"
#include "../Board/Board.hpp"
#include <iostream>
#include <memory>
#include <my_utils.hpp>

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
    legal_moves.reserve(28); // max legal moves a piece can have is 27 (queen)

    setup_sprite();
    set_piece_scale(square_length / texture->getSize().x);
    set_coordinate(coordinate.row, coordinate.col);
}

void Piece::draw() {
    render_window.draw(sprite);
}

Coordinate Piece::get_coordinate() const { return coordinate; }

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

    // has_moved must only be set to true once a move has been made with that piece
    // set_coordinate() is being used in board set up to set up the pieces
    // which would set has_moved to true before any actual move is made
    //
    // by using another bool initial_setup, we can check if it is the initial set up or not
    // and making sure has_moved doesn't change white initial set up
    if(initial_setup) {
        initial_setup = false;
        return;
    }
    has_moved = true;
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

bool Piece::add_common_legal_moves(Board& board, const int row, const int col) {
    auto& sq = board.get_square(row, col);

    if(sq.piece && sq.piece->color == color) return true;

    legal_moves.push_back({ row, col });

    if(color == Piece::COLOR::WHITE)
        sq.is_controlled_by_white = true;
    else
        sq.is_controlled_by_black = true;

    if(sq.piece && sq.piece->color != color) {
        sq.piece->attacked_by.push_back(this);
        return true;
    }

    return false;
}

std::vector<Coordinate> Piece::get_check_elimination_moves(
    const Piece* const attacker,
    const std::shared_ptr<Piece>& king
) {
    if(!attacker || !king) return {};

    // knights and pawns check can't be blocked
    // so the only way to eliminate those checks without moving the king
    // is to capture those pieces
    if(attacker->get_type() == Piece::TYPE::KNIGHT || attacker->get_type() == Piece::TYPE::PAWN) {
        for(const auto& move : legal_moves) {
            if(move == attacker->get_coordinate()) {
                return {move};
            }
        }
        return {};
    }

    // if attacker is not a knight or a pawn, then there are two ways to eliminate checks
    // 1. block the check
    // 2. capture the attacking piece
    //
    // to do this, we traverse from the attacker towards the king or the line of attack
    // and check which of the moves intersect
    // intersecitng / overlapping moves are the ones that can block the check
    const Coordinate& attacker_coord = attacker->get_coordinate();
    const Coordinate& kings_coord    = king->get_coordinate();
    const Coordinate& attacking_dir  = (kings_coord - attacker_coord).getStepValues();

    std::vector<Coordinate> filtered;

    for(auto coord = attacker_coord; coord != kings_coord; coord += attacking_dir) {
        for(const auto& move : legal_moves) {
            if(move == coord)
                filtered.push_back(coord);
        }
    }

    return filtered;
}
