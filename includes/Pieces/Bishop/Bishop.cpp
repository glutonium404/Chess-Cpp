#include "Bishop.hpp"
#include "../../Board/Board.hpp"

Bishop::Bishop(
    sf::RenderWindow&   render_window,
    Coordinate          coordinate,
    sf::FloatRect       board_local_bound,
    Piece::COLOR        color
)
    : Piece(
    render_window,
    color == Piece::COLOR::BLACK ? "assets/images/bishop-b.png" : "assets/images/bishop-w.png",
    coordinate,
    board_local_bound,
    color
) {}

void Bishop::set_legal_moves(Board& board) {
    // in terms of double check, the king must be moved hence no other piece has any valid moves
    if(board.get_king(color)->attacked_by.size() > 1) return;

    std::vector<Coordinate> directions = {
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
}

Piece::TYPE Bishop::get_type() const {
    return Piece::TYPE::BISHOP;
}
