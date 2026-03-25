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
    auto& kings_attackers = board.get_king(color)->attacked_by;

    // in terms of double check, the king must be moved hence no other piece has any valid moves
    if(kings_attackers.size() > 1) return;

    add_forward_moves(board);
    add_diagonal_moves(board);

    if(kings_attackers.size() > 0) {
        const auto& attacker = kings_attackers[0];
        legal_moves = get_check_elimination_moves(attacker, board.get_king(color));
    }
}

void Pawn::add_forward_moves(Board& board) {
    bool is_hori_pinned = is_pinned && pinned_dir.row == 0;
    bool is_diag_pinned = is_pinned && pinned_dir.row != 0 && pinned_dir.col != 0;

    // hizontally or diagonally pinned pawns can't move forward
    if(is_hori_pinned || is_diag_pinned) return;

    int forward_row = coordinate.row + direction;

    if(!Coordinate::is_valid(forward_row, coordinate.col)) return;

    auto& sq = board.get_square(forward_row, coordinate.col);

    if(sq.piece) return;

    legal_moves.push_back({ forward_row, coordinate.col });

    if(has_moved) return;

    int double_forward = coordinate.row + (2 * direction);

    if(!board.get_square(double_forward, coordinate.col).piece) {
        legal_moves.push_back({ double_forward, coordinate.col });
    }
}

void Pawn::add_diagonal_moves(Board& board) {
    bool is_vert_pinned = is_pinned && pinned_dir.col == 0;
    bool is_diag_pinned = is_pinned && pinned_dir.row != 0 && pinned_dir.col != 0;

    int forward_row = coordinate.row + direction;
    int diag_cols[] = { coordinate.col - 1, coordinate.col + 1 };

    for(int diag_col: diag_cols) {
        Coordinate diag_coord = Coordinate(forward_row, diag_col);
        Square&    diag_sq    = board.get_square(diag_coord);

        if(!diag_coord.is_valid())
            continue;

        set_controlled_squares(board, diag_coord);

        // a vertically pinned pawn CANNOT capture diagonally.
        // this condition needs to be after setting control squares
        // because, even if a pawn is pinned it can still control the diagonals
        if(is_vert_pinned)
            continue;

        // when diagonally pinned, the pawns capture dir and pinned dir must match
        if(is_diag_pinned && (diag_coord - coordinate).getStepValues() != pinned_dir)
            continue;

        if(diag_sq.piece && diag_sq.piece->color != color) {
            legal_moves.push_back({ forward_row, diag_col });
            diag_sq.piece->attacked_by.push_back(this);
        }
    }
}

Piece::TYPE Pawn::get_type() const {
    return Piece::TYPE::PAWN;
}
