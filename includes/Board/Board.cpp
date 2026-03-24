#include "Board.hpp"
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>
#include <memory>
#include <my_utils.hpp>

Board::Board(float board_width, sf::RenderWindow& render_window) : render_window(render_window) {
    w_pieces.reserve(16);
    b_pieces.reserve(16);

    square_length = board_width / 8.0;

    set_board_local_bound(board_width);
    set_squares();
    set_pieces();
    add_pieces_to_board();
    update_legal_moves();
}

void Board::handle_event(sf::Event& event) {
    if(is_mouse_clicked(event))
        handle_click(event);
}

void Board::draw() {
    for(auto& square_array: squares) {
        for(auto& square: square_array) {
            square.draw(render_window);
        }
    }
}

void Board::handle_click(sf::Event& event) {
    Coordinate clicked_coordinate = get_clicked_coordinate();

    if(!clicked_coordinate.is_valid()) return;

    Square& new_selected_square = get_square(clicked_coordinate);

    // return is the same piece is clicked again
    if(selected_square == &new_selected_square) return;

    // ======= EMPTY SQUARE (square without peice or highlight) ======

    if(!new_selected_square.piece && !new_selected_square.is_legal_move()) {
        empty_square_clicked();
        manage_check_highlights();
        return;
    }

    // if a highlighted square is clicked
    if(new_selected_square.is_legal_move()) {
        highlighted_square_clicked(new_selected_square);
        manage_check_highlights();
        return;
    }

    // if a player piece was clicked
    if(new_selected_square.piece->color == current_turn) {
        own_piece_clicked(new_selected_square);
        manage_check_highlights();
        return;
    }

    // opposition piece that is not highlighted was clicked
    empty_square_clicked();
    manage_check_highlights();
}

void Board::empty_square_clicked() {
    // remove all highlights except checks (TODO: check logic) and reset selected_square
    remove_existing_highilights();

    if(selected_square) {
        selected_square->unhighlight();
        selected_square = nullptr;
    }
}

void Board::highlighted_square_clicked(Square& new_selected_square) {
    selected_square->unhighlight();
    remove_existing_highilights();
    make_move(new_selected_square);
    toggle_player();
    reset_variables(); // call this before updating legal moves
    update_legal_moves();
}

void Board::own_piece_clicked(Square& new_selected_square) {
    // remove highlights if any previous player piece was selected
    if(selected_square) {
        selected_square->unhighlight();
        remove_existing_highilights();
    }

    // highlight newly selected square piece and its possible moves
    new_selected_square.highlight();
    highlight_legal_moves(new_selected_square.piece);
    selected_square = &new_selected_square;
}

void Board::manage_check_highlights() {
    auto& w_king = get_king(Piece::COLOR::WHITE);
    auto& b_king = get_king(Piece::COLOR::BLACK);

    if(w_king->attacked_by.size() > 0)
        get_square(w_king->get_coordinate()).highlight(Square::check_highlight);
    else
        get_square(w_king->get_coordinate()).unhighlight();

    if(b_king->attacked_by.size() > 0)
        get_square(b_king->get_coordinate()).highlight(Square::check_highlight);
    else
        get_square(b_king->get_coordinate()).unhighlight();
}

void Board::highlight_legal_moves(const std::shared_ptr<Piece>& piece) {
    highlighted_coord = piece->legal_moves;

    for(auto& coord: highlighted_coord) {
        auto& sq = get_square(coord);

        if(sq.piece && sq.piece->color != current_turn) {
            sq.highlight(Square::red_highlight);
        }else {
            sq.highlight();
        }
    }
}

void Board::remove_existing_highilights() {
    for(auto& coord: highlighted_coord) {
        get_square(coord).unhighlight();
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

void Board::make_move(Square& new_square) {
    if(!selected_square || !selected_square->piece) return;

    if(new_square.piece && new_square.piece->color != current_turn) new_square.piece->is_alive = false;

    new_square.piece = selected_square->piece;
    new_square.piece->set_coordinate(new_square.coordinate);
    selected_square->piece = nullptr;
}

void Board::update_legal_moves() {
    for(auto& piece: w_pieces) {
        piece->legal_moves.clear();
        piece->set_legal_moves(*this);
    }

    for(auto& piece: b_pieces) {
        piece->legal_moves.clear();
        piece->set_legal_moves(*this);
    }
}

Square& Board::get_square(int row, int col) {
    return squares[row - 1][col - 1];
}

Square& Board::get_square(const Coordinate& coord) {
    return get_square(coord.row, coord.col);
}

void Board::toggle_player() {
    current_turn = (current_turn == Piece::COLOR::WHITE) ? Piece::COLOR::BLACK : Piece::COLOR::WHITE;
}

void Board::reset_variables() {
    for(auto& row: squares) {
        for(auto& sq: row) {
            sq.is_controlled_by_black = false;
            sq.is_controlled_by_white = false;
        }
    }

    for(auto& piece: w_pieces) piece->attacked_by.clear();
    for(auto& piece: b_pieces) piece->attacked_by.clear();
}

const std::shared_ptr<Piece>& Board::get_king(const Piece::COLOR color) const {
    return color == Piece::COLOR::WHITE ? w_pieces.back() : b_pieces.back();
}
