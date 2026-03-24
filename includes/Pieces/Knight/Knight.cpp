#include "Knight.hpp"
#include "../../Board/Board.hpp"

Knight::Knight(
    sf::RenderWindow&   render_window,
    Coordinate          coordinate,
    sf::FloatRect       board_local_bound,
    Piece::COLOR        color
)
    : Piece(
    render_window,
    color == Piece::COLOR::BLACK ? "assets/images/knight-b.png" : "assets/images/knight-w.png",
    coordinate,
    board_local_bound,
    color
) {}

void Knight::set_legal_moves(Board& board) {
    const auto& kings_attackers = board.get_king(color)->attacked_by;

    // in terms of double check, the king must be moved hence no other piece has any valid moves
    if(kings_attackers.size() > 1) return;

    std::vector<Coordinate> common_legal_moves = {
        {coordinate.row - 2, coordinate.col - 1},
        {coordinate.row - 2, coordinate.col + 1},

        {coordinate.row + 2, coordinate.col - 1},
        {coordinate.row + 2, coordinate.col + 1},

        {coordinate.row + 1, coordinate.col - 2},
        {coordinate.row - 1, coordinate.col - 2},

        {coordinate.row + 1, coordinate.col + 2},
        {coordinate.row - 1, coordinate.col + 2}
    };

    for(auto& coord: common_legal_moves) {
        if(coord.is_valid())
            add_common_legal_moves(board, coord.row, coord.col);
    }

    if(kings_attackers.size() > 0) {
        const auto& attacker = kings_attackers[0];
        legal_moves = get_check_elimination_moves(attacker, board.get_king(color));
    }
}

Piece::TYPE Knight::get_type() const {
    return Piece::TYPE::KNIGHT;
}
