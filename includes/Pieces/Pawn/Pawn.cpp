#include "Pawn.hpp"

Pawn::Pawn(
    sf::RenderWindow&   render_window,
    Coordinate          coordinate,
    sf::FloatRect       board_local_bound,
    Piece::COLOR         side
)
    : Piece(
    render_window,
    side == Piece::COLOR::BLACK ? "assets/images/pawn-b.png" : "assets/images/pawn-w.png",
    coordinate,
    board_local_bound
)
{
    // direction dictates the forward direction of the merching pawn
    // when pawn moves forward, either it's row value increases or decreases based on it's forward direction
    // -ve: forward direction = bottom -> top
    // +ve: forward direction = top -> bottom
    direction = coordinate.row > 4 ? -1 : 1;
}

std::vector<Coordinate> Pawn::get_possible_moves() {
    std::vector<Coordinate> possible_moves;

    possible_moves.push_back( Coordinate( coordinate.row + (1 * direction), coordinate.col ) );

    if(is_first_move)
        possible_moves.push_back( Coordinate( coordinate.row + (2 * direction), coordinate.col ) );

    return possible_moves;
}

Piece::TYPE Pawn::get_type() const {
    return Piece::TYPE::PAWN;
}
