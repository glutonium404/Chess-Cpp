#include "Queen.hpp"
#include "../../Board/Board.hpp"
#include <my_utils.hpp>
#include <vector>

Queen::Queen(
    sf::RenderWindow&   render_window,
    Coordinate          coordinate,
    sf::FloatRect       board_local_bound,
    Piece::COLOR         color
)
    : Piece(
    render_window,
    color == Piece::COLOR::BLACK ? "assets/images/queen-b.png" : "assets/images/queen-w.png",
    coordinate,
    board_local_bound,
    color
) {}


void Queen::set_legal_moves(Board& board) {
    auto& kings_attackers = board.get_king(color)->attacked_by;

    // in terms of double check, the king must be moved hence no other piece has any valid moves
    if(kings_attackers.size() > 1) return;

    std::vector<Coordinate> directions = {
        { 0, -1}, // left
        { 0,  1}, // right
        {-1,  0}, // top
        { 1,  0}, // bottom
        {-1, -1}, // top-left
        {-1,  1}, // top-right
        { 1,  1}, // bottom-right
        { 1, -1}  // bottom-left
    };

    for(auto& dir: directions) {
        for(Coordinate curr_coord = coordinate + dir; curr_coord.is_valid(); curr_coord += dir) {
            if(add_common_legal_moves(board, curr_coord.row, curr_coord.col))
                break;
        }
    }

    // if there is a check on king that is not a double check,
    // the only valid moves are the ones that block the check or eliminates the attacker
    // through finding the intersection between both the piece and the attackers legal move,
    // we can determine which moves intersect the check. filter_legal_moves() does exactly that
    if(kings_attackers.size() > 0) {
        // if not double check then there is only one attacker
        auto& attacker = kings_attackers[0];
        legal_moves = get_check_elimination_moves(attacker, board.get_king(color));
    }
}

Piece::TYPE Queen::get_type() const {
    return Piece::TYPE::QUEEN;
}
