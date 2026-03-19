#include "Board.hpp"
#include "../Pieces/King/King.hpp"
#include "../Pieces/Queen/Queen.hpp"
#include "../Pieces/Rook/Rook.hpp"
#include "../Pieces/Bishop/Bishop.hpp"
#include "../Pieces/Knight/Knight.hpp"
#include "../Pieces/Pawn/Pawn.hpp"
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>
#include <memory>

Board::Board(float board_width, sf::RenderWindow& render_window) : render_window(render_window) {
    w_pieces.reserve(16);
    b_pieces.reserve(16);

    square_length = board_width / 8.0;

    set_board_local_bound(board_width);
    set_squares_shapes();
    set_pieces();
}

void Board::handle_event(sf::Event& event) {
    handle_click(event);
}

void Board::draw() {
    for(auto& square_array: squares) {
        for(auto& square: square_array) {
            render_window.draw(square.shape);
        }
    }

    for(int i=0; i<b_pieces.size(); i++) {
        if(b_pieces[i]->is_alive) {
            b_pieces[i]->draw();
        }
    }

    for(int i=0; i<w_pieces.size(); i++) {
        if(w_pieces[i]->is_alive) {
            w_pieces[i]->draw();
        }
    }
}

void Board::highlight_possible_moves(const std::unique_ptr<Piece>& piece) {
    remove_existing_highilights();
    get_square(selected_piece.piece(*this)->coordinate).highlight();

    highlighted_coord = piece->get_possible_moves();

    for(auto& coord: highlighted_coord) {
        auto& highlighted_square = get_square(coord);
        highlighted_square.highlight();
        render_window.draw(highlighted_square.shape);
    }
}

void Board::remove_existing_highilights() {
    get_square(selected_piece.piece(*this)->coordinate).unhighlight();

    for(auto& coord: highlighted_coord) {
        auto& highlighted_square = get_square(coord);
        highlighted_square.unhighlight();
        render_window.draw(highlighted_square.shape);
    }
}

void Board::handle_click(sf::Event& event) {
    if(!is_mouse_clicked(event)) return;

    // remove highilight from any previously selected piece square
    // without this when we select a piece and then select another piece
    // the square of the previously selected piece stays highlighted
    if(selected_piece.is_any_selected) {
        get_square(selected_piece.piece(*this)->coordinate).unhighlight();
    }

    if(piece_clicked()) {
        highlight_possible_moves(selected_piece.piece(*this));
        return;
    }

    // empty square was clicked

    if(selected_piece.is_any_selected) {
        highlighted_square_clicked();
    }

    selected_piece.is_any_selected = false;
    remove_existing_highilights();
}

bool Board::piece_clicked() {
    auto clicked_coordinate = get_clicked_coordinate();

    if(current_turn == Piece::COLOR::WHITE) {
        for(int i=0; i<w_pieces.size(); i++) {
            if(!w_pieces[i]->is_alive) continue;

            if(w_pieces[i]->coordinate == clicked_coordinate) {
                selected_piece.is_any_selected = true;
                selected_piece.side  = Piece::COLOR::WHITE;
                selected_piece.index = i;

                return true;
            }
        }
    } else {
        for(int i=0; i<b_pieces.size(); i++) {
            if(!b_pieces[i]->is_alive) continue;

            if(b_pieces[i]->coordinate == clicked_coordinate) {
                selected_piece.is_any_selected = true;
                selected_piece.side  = Piece::COLOR::BLACK;
                selected_piece.index = i;

                return true;
            }
        }
    }

    // we are setting is_any_selected to false in handle_click
    // because we need this variable to check if any highlighted square is clicked
    return false;
}

void Board::highlighted_square_clicked() {
    auto clicked_coordinate = get_clicked_coordinate();

    for(auto& coord: highlighted_coord) {
        if(coord != clicked_coordinate) continue;
        make_move(coord);
        return;
    }
}

bool Board::is_mouse_clicked(sf::Event& event) const {
    if(event.type != sf::Event::MouseButtonPressed || event.mouseButton.button != sf::Mouse::Left) return false;

    return board_local_bound.contains(
        static_cast<float>(event.mouseButton.x),
        static_cast<float>(event.mouseButton.y)
    );
}

Coordinate Board::get_clicked_coordinate() {
    // get mouse position relative to the window
    sf::Vector2i mouse_pixel_pos = sf::Mouse::getPosition(render_window);

    // map pixels to world coordinates (handles scaling/views)
    sf::Vector2f mouse_pos = render_window.mapPixelToCoords(mouse_pixel_pos);

    // calculate 1 based coordinates
    int row = static_cast<int>((mouse_pos.y - board_local_bound.top) / square_length) + 1;
    int col = static_cast<int>((mouse_pos.x - board_local_bound.left) / square_length) + 1;

    return Coordinate(row, col);
}

void Board::make_move(Coordinate& new_coordinate) {
    // toggle player
    current_turn = (current_turn == Piece::COLOR::WHITE) ? Piece::COLOR::BLACK : Piece::COLOR::WHITE;

    selected_piece.piece(*this)->set_coordinate(new_coordinate);
}

Board::Square& Board::get_square(int row, int col) {
    return squares[row - 1][col - 1];
}

Board::Square& Board::get_square(const Coordinate& coord) {
    return get_square(coord.row, coord.col);
}

void Board::set_board_local_bound(float& board_width) {
    sf::Vector2u window_dim = render_window.getSize();

    float offset_x = (window_dim.x - board_width) / 2.0;
    float offset_y = (window_dim.y - board_width) / 2.0;

    board_local_bound.left      = offset_x;
    board_local_bound.top       = offset_y;
    board_local_bound.width     = square_length * 8.f;
    board_local_bound.height    = square_length * 8.f;
}

void Board::set_squares_shapes() {
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

            squares[i][j].type = flag ? Square::COLOR::LIGHT : Square::COLOR::DARK;

            if(j != 7) { flag = !flag; }
        }
    }
}

void Board::set_pieces() {
    b_pieces.push_back( make_rook   (1, 1, Piece::COLOR::BLACK) );
    b_pieces.push_back( make_knight (1, 2, Piece::COLOR::BLACK) );
    b_pieces.push_back( make_bishop (1, 3, Piece::COLOR::BLACK) );
    b_pieces.push_back( make_queen  (1, 4, Piece::COLOR::BLACK) );
    b_pieces.push_back( make_king   (1, 5, Piece::COLOR::BLACK) );
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
    w_pieces.push_back( make_king   (8, 5, Piece::COLOR::WHITE) );
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
}

std::unique_ptr<Piece> Board::make_pawn(int row, int col, Piece::COLOR side) {
    return std::make_unique<Pawn>(
        render_window, Coordinate(row, col), board_local_bound, side
    );
}

std::unique_ptr<Piece> Board::make_king(int row, int col, Piece::COLOR side) {
    return std::make_unique<King>(
        render_window, Coordinate(row, col), board_local_bound, side
    );
}

std::unique_ptr<Piece> Board::make_queen(int row, int col, Piece::COLOR side) {
    return std::make_unique<Queen>(
        render_window, Coordinate(row, col), board_local_bound, side
    );
}
std::unique_ptr<Piece> Board::make_rook(int row, int col, Piece::COLOR side) {
    return std::make_unique<Rook>(
        render_window, Coordinate(row, col), board_local_bound, side
    );
}
std::unique_ptr<Piece> Board::make_knight(int row, int col, Piece::COLOR side) {
    return std::make_unique<Knight>(
        render_window, Coordinate(row, col), board_local_bound, side
    );
}
std::unique_ptr<Piece> Board::make_bishop(int row, int col, Piece::COLOR side) {
    return std::make_unique<Bishop>(
        render_window, Coordinate(row, col), board_local_bound, side
    );
}

void Board::Square::highlight() {
    switch (type) {
        case COLOR::LIGHT:
            shape.setFillColor(Square::light_highlight);
            break;
        case COLOR::DARK:
            shape.setFillColor(Square::dark_highlight);
            break;
        case COLOR::RED:
            shape.setFillColor(Square::red_highlight);
            break;
    }
}

void Board::Square::unhighlight() {
    shape.setFillColor(
        type == COLOR::LIGHT ? Square::light_color : Square::dark_color
    );
}

std::unique_ptr<Piece>& Board::SelectedPiece::piece(Board& board) const {
    return (side == Piece::COLOR::WHITE) ? board.w_pieces[index] : board.b_pieces[index];
}
