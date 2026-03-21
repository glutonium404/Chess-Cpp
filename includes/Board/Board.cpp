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
#include <my_utils.hpp>

Board::Board(float board_width, sf::RenderWindow& render_window) : render_window(render_window) {
    w_pieces.reserve(16);
    b_pieces.reserve(16);

    square_length = board_width / 8.0;

    set_board_local_bound(board_width);
    set_squares_shapes();
    set_pieces();
    add_pieces_to_board();
}

void Board::handle_event(sf::Event& event) {
    if(is_mouse_clicked(event))
        handle_click(event);
}

void Board::draw() {
    for(auto& square_array: squares) {
        for(auto& square: square_array) {
            square.draw(*this);
        }
    }
}

void Board::handle_click(sf::Event& event) {
    Coordinate clicked_coordinate = get_clicked_coordinate();

    if(!clicked_coordinate.is_valid()) return;

    Square& new_selected_square = get_square(clicked_coordinate);

    // ======= EMPTY SQUARE (square without peice or highlight) ======

    if(!new_selected_square.piece && !new_selected_square.is_a_possible_move(*this)) {
        remove_existing_highilights();

        if(selected_square) {
            selected_square->unhighlight();
            selected_square = nullptr;
        }

        return;
    }

    // ====== EIHTER A PIECE OR A HIGHLIGHT SQUARE IS CLICKED =====

    // if a highlighted square is clicked
    if(!new_selected_square.piece) {
        selected_square->unhighlight();
        remove_existing_highilights();
        make_move(new_selected_square);
        current_turn = (current_turn == Piece::COLOR::WHITE) ? Piece::COLOR::BLACK : Piece::COLOR::WHITE;
        return;
    }

    // if a player piece was clicked
    if(new_selected_square.piece->color == current_turn) {

        // return is the same piece is clicked again
        if(selected_square == &new_selected_square) {
            return;
        }

        // remove highlights if any previous player piece was selected
        if(selected_square) {
            selected_square->unhighlight();
            remove_existing_highilights();
        }

        // highlight newly selected square piece and its possible moves
        new_selected_square.highlight();
        highlight_possible_moves(new_selected_square.piece);
        selected_square = &new_selected_square;
        return;
    }

    // opposition piece was clicked

    // remove all highlights except checks (TODO: check logic) and reset selected_square
    remove_existing_highilights();

    if(selected_square) {
        selected_square->unhighlight();
        selected_square = nullptr;
    }
}

void Board::highlight_possible_moves(const std::shared_ptr<Piece>& piece) {
    highlighted_coord = piece->get_possible_moves();

    for(auto& coord: highlighted_coord) {
        get_square(coord).highlight();
    }
}

void Board::remove_existing_highilights() {
    for(auto& coord: highlighted_coord) {
        get_square(coord).unhighlight();
    }

    highlighted_coord.clear();
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

void Board::make_move(Square& new_square) {
    if(!selected_square || !selected_square->piece) return;

    new_square.piece = selected_square->piece;
    new_square.piece->set_coordinate(new_square.coordinate);
    selected_square->piece = nullptr;
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
