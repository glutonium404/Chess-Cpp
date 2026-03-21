#include "Pawn.hpp"
#include "../../Board/Board.hpp"

Pawn::Pawn(
    sf::RenderWindow&   render_window,
    Coordinate          coordinate,
    sf::FloatRect       board_local_bound,
    Piece::COLOR        color
)
    : Piece(
    render_window,
    color == Piece::COLOR::BLACK ? "assets/images/pawn-b.png" : "assets/images/pawn-w.png",
    coordinate,
    board_local_bound,
    color
)
{
    // direction dictates the forward direction of the merching pawn
    // when pawn moves forward, either it's row value increases or decreases based on it's forward direction
    // -ve: forward direction = bottom -> top
    // +ve: forward direction = top -> bottom
    direction = coordinate.row > 4 ? -1 : 1;
}

std::vector<Coordinate> Pawn::get_legal_moves(Board& board) {
    std::vector<Coordinate> legal_moves;

    add_common_legal_moves(board, legal_moves, coordinate.row + (1 * direction), coordinate.col);

    if(!has_moved)
        add_common_legal_moves(board, legal_moves, coordinate.row + (2 * direction), coordinate.col);

    Coordinate diagonal_1 = Coordinate(coordinate.row + (1 * direction), coordinate.col + 1);
    Coordinate diagonal_2 = Coordinate(coordinate.row + (1 * direction), coordinate.col - 1);

    auto& diagonal_1_piece = board.get_square(diagonal_1).piece;
    auto& diagonal_2_piece = board.get_square(diagonal_2).piece;

    if(diagonal_1_piece && diagonal_1_piece->color != board.current_turn)
        legal_moves.push_back(diagonal_1);

    if(diagonal_2_piece && diagonal_2_piece->color != board.current_turn)
        legal_moves.push_back(diagonal_2);

    return legal_moves;
}

Piece::TYPE Pawn::get_type() const {
    return Piece::TYPE::PAWN;
}
