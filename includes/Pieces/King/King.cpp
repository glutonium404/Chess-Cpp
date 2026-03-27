#include "King.hpp"
#include "../../Board/Board.hpp"
#include <cstddef>

King::King(
    sf::RenderWindow&   render_window,
    Coordinate          coordinate,
    sf::FloatRect       board_local_bound,
    Piece::COLOR        color
)
    : Piece(
    render_window,
    color == Piece::COLOR::BLACK ? "assets/images/king-b.png" : "assets/images/king-w.png",
    coordinate,
    board_local_bound,
    color
) {}

void King::set_legal_moves(Board& board) {
    std::vector<Coordinate> common_lookup_coordinates = {
        {coordinate.row - 1, coordinate.col - 1}, // top left
        {coordinate.row - 1, coordinate.col    }, // top middle
        {coordinate.row - 1, coordinate.col + 1}, // top right

        {coordinate.row + 1, coordinate.col - 1}, // bottom left
        {coordinate.row + 1, coordinate.col    }, // bottom middle
        {coordinate.row + 1, coordinate.col + 1}, // bottom right

        {coordinate.row    , coordinate.col - 1}, // center left
        {coordinate.row    , coordinate.col + 1}  // center right
    };

    for(auto& coord: common_lookup_coordinates) {
        if(coord.is_valid()) {
            auto& sq = board.get_square(coord);

            set_controlled_squares(board, coord);

            if(( is_white() && !sq.is_controlled_by_black ) || ( is_black() && !sq.is_controlled_by_white ))
                add_common_legal_moves(board, coord.row, coord.col);
        }
    }

    if(has_moved) return;

    std::size_t k = is_white() ? static_cast<std::size_t>(Board::CR::WK) : static_cast<std::size_t>(Board::CR::BK);
    std::size_t q = is_white() ? static_cast<std::size_t>(Board::CR::WQ) : static_cast<std::size_t>(Board::CR::BQ);

    bool king_side  = (board.castling_right & k) == k;
    bool queen_side = (board.castling_right & q) == q;

    if(!king_side && !queen_side) return;

    add_king_side_castling(board);
    add_queen_side_castling(board);
}

void King::add_king_side_castling(Board& board) {
    for(int col = coordinate.col + 1; col < 8; col++) {
        const auto& sq = board.get_square(coordinate.row, col);
        if(sq.piece) return;
        if(is_white() && sq.is_controlled_by_black) return;
        if(is_black() && sq.is_controlled_by_white) return;
    }

    const auto& sq = board.get_square(coordinate.row, 8);

    if(!sq.piece || sq.piece->get_type() != Piece::TYPE::ROOK || sq.piece->color != color || sq.piece->has_moved)
        return;

    legal_moves.push_back({ coordinate.row, coordinate.col + 2 });
}

void King::add_queen_side_castling(Board& board) {
    for(int col = coordinate.col - 1; col > 1; col--) {
        const auto& sq = board.get_square(coordinate.row, col);
        if(sq.piece) return;

        if(col < 3) continue; // it does not matter if 1, 2 col is controlled by the oppo or not

        if(is_white() && sq.is_controlled_by_black) return;
        if(is_black() && sq.is_controlled_by_white) return;
    }

    const auto& sq = board.get_square(coordinate.row, 1);

    if(!sq.piece || sq.piece->get_type() != Piece::TYPE::ROOK || sq.piece->color != color || sq.piece->has_moved)
        return;

    legal_moves.push_back({ coordinate.row, coordinate.col - 2 });
}

Piece::TYPE King::get_type() const {
    return Piece::TYPE::KING;
}
