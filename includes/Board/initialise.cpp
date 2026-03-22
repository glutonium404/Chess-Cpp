#include "Board.hpp"
#include "../Pieces/King/King.hpp"
#include "../Pieces/Queen/Queen.hpp"
#include "../Pieces/Rook/Rook.hpp"
#include "../Pieces/Bishop/Bishop.hpp"
#include "../Pieces/Knight/Knight.hpp"
#include "../Pieces/Pawn/Pawn.hpp"

void Board::set_board_local_bound(float& board_width) {
    sf::Vector2u window_dim = render_window.getSize();

    float offset_x = (window_dim.x - board_width) / 2.0;
    float offset_y = (window_dim.y - board_width) / 2.0;

    board_local_bound.left      = offset_x;
    board_local_bound.top       = offset_y;
    board_local_bound.width     = square_length * 8.f;
    board_local_bound.height    = square_length * 8.f;
}

void Board::set_squares() {
    bool flag = true;

    for(int i=0; i<8; i++) {
        for(int j=0; j<8; j++) {

            sf::Color color = flag ? Square::light_color : Square::dark_color;

            sf::Vector2f position = {
                j * square_length + board_local_bound.left,
                i * square_length + board_local_bound.top
            };

            squares[i][j].shape = sf::RectangleShape({square_length, square_length});

            squares[i][j].shape.setFillColor(color);
            squares[i][j].shape.setPosition(position);

            squares[i][j].coordinate = Coordinate(i+1, j+1);
            squares[i][j].type       = flag ? Square::COLOR::LIGHT : Square::COLOR::DARK;

            if(j != 7) { flag = !flag; }
        }
    }
}

void Board::set_pieces() {
    b_pieces.push_back( make_rook   (1, 1, Piece::COLOR::BLACK) );
    b_pieces.push_back( make_knight (1, 2, Piece::COLOR::BLACK) );
    b_pieces.push_back( make_bishop (1, 3, Piece::COLOR::BLACK) );
    b_pieces.push_back( make_queen  (1, 4, Piece::COLOR::BLACK) );
    b_pieces.push_back( make_bishop (1, 6, Piece::COLOR::BLACK) );
    b_pieces.push_back( make_knight (1, 7, Piece::COLOR::BLACK) );
    b_pieces.push_back( make_rook   (1, 8, Piece::COLOR::BLACK) );

    b_pieces.push_back( make_pawn   (2, 1, Piece::COLOR::BLACK) );
    b_pieces.push_back( make_pawn   (2, 2, Piece::COLOR::BLACK) );
    b_pieces.push_back( make_pawn   (2, 3, Piece::COLOR::BLACK) );
    b_pieces.push_back( make_pawn   (2, 4, Piece::COLOR::BLACK) );
    b_pieces.push_back( make_pawn   (2, 5, Piece::COLOR::BLACK) );
    b_pieces.push_back( make_pawn   (2, 6, Piece::COLOR::BLACK) );
    b_pieces.push_back( make_pawn   (2, 7, Piece::COLOR::BLACK) );
    b_pieces.push_back( make_pawn   (2, 8, Piece::COLOR::BLACK) );



    w_pieces.push_back( make_rook   (8, 1, Piece::COLOR::WHITE) );
    w_pieces.push_back( make_knight (8, 2, Piece::COLOR::WHITE) );
    w_pieces.push_back( make_bishop (8, 3, Piece::COLOR::WHITE) );
    w_pieces.push_back( make_queen  (8, 4, Piece::COLOR::WHITE) );
    w_pieces.push_back( make_bishop (8, 6, Piece::COLOR::WHITE) );
    w_pieces.push_back( make_knight (8, 7, Piece::COLOR::WHITE) );
    w_pieces.push_back( make_rook   (8, 8, Piece::COLOR::WHITE) );

    w_pieces.push_back( make_pawn(7, 1, Piece::COLOR::WHITE) );
    w_pieces.push_back( make_pawn(7, 2, Piece::COLOR::WHITE) );
    w_pieces.push_back( make_pawn(7, 3, Piece::COLOR::WHITE) );
    w_pieces.push_back( make_pawn(7, 4, Piece::COLOR::WHITE) );
    w_pieces.push_back( make_pawn(7, 5, Piece::COLOR::WHITE) );
    w_pieces.push_back( make_pawn(7, 6, Piece::COLOR::WHITE) );
    w_pieces.push_back( make_pawn(7, 7, Piece::COLOR::WHITE) );
    w_pieces.push_back( make_pawn(7, 8, Piece::COLOR::WHITE) );


    // when updating legal moves
    // we need the kigns to be updated at the very end
    // this is necessary for knowing if a square is safe or not for a king to be considered a legal move
    // which is why we are adding both kings at the very end
    w_pieces.push_back( make_king   (8, 5, Piece::COLOR::WHITE) );
    b_pieces.push_back( make_king   (1, 5, Piece::COLOR::BLACK) );
}

void Board::add_pieces_to_board() {
    for(auto& piece: w_pieces) {
        auto coord = piece->get_coordinate();
        squares[coord.row - 1][coord.col - 1].piece = piece;
    }
    for(auto& piece: b_pieces) {
        auto coord = piece->get_coordinate();
        squares[coord.row - 1][coord.col - 1].piece = piece;
    }
}

std::shared_ptr<Piece> Board::make_pawn(int row, int col, Piece::COLOR color) {
    return std::make_shared<Pawn>(
        render_window, Coordinate(row, col), board_local_bound, color
    );
}

std::shared_ptr<Piece> Board::make_king(int row, int col, Piece::COLOR color) {
    return std::make_shared<King>(
        render_window, Coordinate(row, col), board_local_bound, color
    );
}

std::shared_ptr<Piece> Board::make_queen(int row, int col, Piece::COLOR color) {
    return std::make_shared<Queen>(
        render_window, Coordinate(row, col), board_local_bound, color
    );
}
std::shared_ptr<Piece> Board::make_rook(int row, int col, Piece::COLOR color) {
    return std::make_shared<Rook>(
        render_window, Coordinate(row, col), board_local_bound, color
    );
}
std::shared_ptr<Piece> Board::make_knight(int row, int col, Piece::COLOR color) {
    return std::make_shared<Knight>(
        render_window, Coordinate(row, col), board_local_bound, color
    );
}
std::shared_ptr<Piece> Board::make_bishop(int row, int col, Piece::COLOR color) {
    return std::make_shared<Bishop>(
        render_window, Coordinate(row, col), board_local_bound, color
    );
}
