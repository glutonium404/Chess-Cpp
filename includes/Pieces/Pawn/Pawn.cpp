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

void Pawn::set_legal_moves(Board& board) {
    int forward_row = coordinate.row + direction;

    if(Coordinate::is_valid(forward_row, coordinate.col)) {
        auto& sq = board.get_square(forward_row, coordinate.col);

        if(!sq.piece) {
            legal_moves.push_back({ forward_row, coordinate.col });

            if(!has_moved) {
                int double_forward = coordinate.row + (2 * direction);

                if(!board.get_square(double_forward, coordinate.col).piece) {
                    legal_moves.push_back({ double_forward, coordinate.col });
                }
            }
        }
    }

    int diag_cols[] = { coordinate.col - 1, coordinate.col + 1 };

    for(int diag_col: diag_cols) {
        if(Coordinate::is_valid(forward_row, diag_col)) {
            auto& sq = board.get_square(forward_row, diag_col);

            if(sq.piece && sq.piece->color != color) {
                legal_moves.push_back({ forward_row, diag_col });
                sq.piece->attacked_by.push_back(this);

                if(color == Piece::COLOR::WHITE)
                    sq.is_controlled_by_white = true;
                else
                    sq.is_controlled_by_black = true;
            }
        }
    }
}

Piece::TYPE Pawn::get_type() const {
    return Piece::TYPE::PAWN;
}
