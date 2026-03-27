#include "Board.hpp"
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>
#include <cstddef>
#include <cstdlib>
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
    set_promotion_selection_list();
    populate_look_up_tables();
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

    if(!promotion_coord.is_valid())
        return;

    render_window.draw(dimmer);

    for(auto& sq: promotion_selection_list) {
        sq.draw(render_window);
        sq.piece->draw();
    }
}

void Board::handle_click(sf::Event& event) {
    Coordinate clicked_coordinate = get_clicked_coordinate();

    if(!clicked_coordinate.is_valid()) return;

    Square& new_selected_square = get_square(clicked_coordinate);

    // handle promotion
    if(promotion_coord.is_valid()) {
        for(const auto& sq: promotion_selection_list) {
            if(sq.coordinate == clicked_coordinate) {
                if (sq.piece->is_white()) {
                    w_pieces.insert(w_pieces.end() - 1, sq.piece);
                } else {
                    b_pieces.insert(b_pieces.end() - 1, sq.piece);
                }

                get_square(promotion_coord).piece->is_alive = false;
                get_square(promotion_coord).piece = sq.piece;
                get_square(promotion_coord).piece->set_coordinate(promotion_coord);

                promotion_coord.row = 0;
                promotion_coord.col = 0;
            }
        }

        if(promotion_coord.is_valid())
            return;

        toggle_player();
        update_zobrist_hash();
        reset_variables(); // call this before updating pinned_pieaces and legal_moves
        update_pinned_pieces();
        update_legal_moves();
        check_game_state();
        manage_check_highlights();
        return;
    }

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

    if(promotion_coord.is_valid())
        return;

    toggle_player();
    update_zobrist_hash();
    reset_variables(); // call this before updating pinned_pieaces and legal_moves
    update_pinned_pieces();
    update_legal_moves();
    check_game_state();
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

        bool is_en_pass_sq = (coord == Coordinate(en_passant_row, en_passant_file));
        bool is_oppo_piece = (sq.piece && sq.piece->color != current_turn); 

        if(is_en_pass_sq || is_oppo_piece) {
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

    bool is_en_pass_sq = (new_square.coordinate == Coordinate(en_passant_row, en_passant_file));
    bool is_oppo_piece = (new_square.piece && new_square.piece->color != current_turn); 

    bool is_pawn       = (selected_square->piece->get_type() == Piece::TYPE::PAWN);
    bool is_king       = (selected_square->piece->get_type() == Piece::TYPE::KING);
    bool is_castling   = is_king && ( std::abs(new_square.coordinate.col - selected_square->coordinate.col) == 2 );

    // call this before updating zobrist varaibles
    // otherwise they'll be reset
    if(is_en_pass_sq) {
        auto& oppo_piece_sq = get_square(selected_square->coordinate.row, en_passant_file);
        if(oppo_piece_sq.piece) oppo_piece_sq.piece = nullptr;
    }

    if(is_oppo_piece) {
        new_square.piece->is_alive = false;
    }

    if(is_castling) { // we are only setting the rook here
        int old_col, new_col;

        if(new_square.coordinate.col > 5) { // king side castle
            old_col = 8;
            new_col = 6;
        }else { // queen side
            old_col = 1;
            new_col = 4;
        }

        auto& rook_sq     = get_square(new_square.coordinate.row, old_col);
        auto& new_rook_sq = get_square(new_square.coordinate.row, new_col);

        new_rook_sq.piece = rook_sq.piece;
        new_rook_sq.piece->set_coordinate(new_rook_sq.coordinate);

        rook_sq.piece = nullptr;
    }

    if(is_pawn && (new_square.coordinate.row == 1 || new_square.coordinate.row == 8)) {
        promotion_coord = new_square.coordinate;
        show_promotion_selection_list(new_square.coordinate.row, new_square.coordinate.col);
    }

    update_zobrist_variables(new_square);

    new_square.piece = selected_square->piece;
    new_square.piece->set_coordinate(new_square.coordinate);
    selected_square->piece = nullptr;
}

// in set legal moves, we have to check if the king is in check or not
// this is checked through attacked_by.size()
// so essentially, for the player, whose king may be in check, in order to calculate the players legal moves
// we need to first know how many opponents pieces are attacking/checking players king at that moment
// and for that the oppoents legal moves need to be calculated first
// due to this dependancy, we are first calculating the opponents legal moves
// which populates the players kings attacked_by
// and then with that up-to-date value we can safely use the variable
void Board::update_legal_moves() {
    auto& first_batch  = (current_turn == Piece::COLOR::WHITE) ? b_pieces : w_pieces;
    auto& second_batch = (current_turn == Piece::COLOR::WHITE) ? w_pieces : b_pieces;

    for(auto& piece: first_batch) {
        if(!piece->is_alive) continue;

        piece->legal_moves.clear();
        piece->set_legal_moves(*this);
    }

    for(auto& piece: second_batch) {
        if(!piece->is_alive) continue;

        piece->legal_moves.clear();
        piece->set_legal_moves(*this);
    }
}

// this works as follows,
// - start from the king and traverse through all 8 direcitons
// - in each direction if we encounter a piece,
//   - break (check next dir) if we encounter our own piece twice
//   - break (check next dir) if we encounter oppo piece without encountering our own piece beforehand
//   - 
//   - if we encounter own piece for the first time, store and continue searching for oppo piece in the same dir
//   - if we encounter own piece and then find oppo piece afterwards,
//      - check its type. (only Queen, Bishop & Rook can pin) and the corresponding dir (bishop can only pin diagonally)
//      - if type matches, we have our own_piece pinned by this oppo piece
void Board::update_pinned_pieces() {
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

    const auto& king_coord = get_king(current_turn)->get_coordinate();

    for(const auto& dir: directions) {
        std::shared_ptr<Piece> own_piece = nullptr;
        bool is_diag = (dir.row != 0 && dir.col != 0);

        for(auto coord = king_coord + dir; coord.is_valid(); coord += dir) {
            auto curr_piece = get_square(coord).piece;

            if(!curr_piece) continue;

            if( own_piece && curr_piece->color == current_turn) break;
            if(!own_piece && curr_piece->color != current_turn) break;

            if(!own_piece && curr_piece->color == current_turn) {
                own_piece = curr_piece;
                continue;
            }

            if( own_piece && curr_piece->color != current_turn) {
                auto oppo_piece_type = curr_piece->get_type();
                if(
                    oppo_piece_type == Piece::TYPE::QUEEN               ||
                    (!is_diag && oppo_piece_type == Piece::TYPE::ROOK)  ||
                    ( is_diag && oppo_piece_type == Piece::TYPE::BISHOP)
                )
                {
                    own_piece->is_pinned = true;
                    own_piece->pinned_dir = dir;
                    break;
                }
            }
        }
    }
}

void Board::update_zobrist_hash() {
    hash = 0;

    for(const auto& row: squares) {
        for(const auto& sq: row) {
            if(!sq.piece) continue;

            std::size_t color_index = static_cast<std::size_t>(sq.piece->color);
            std::size_t type_index = static_cast<std::size_t>(sq.piece->get_type());

            hash ^= piece_table[ color_index ][ type_index ][ sq.coordinate.to_index() ];
        }
    }

    if(current_turn == Piece::COLOR::BLACK)
        hash ^= side_to_move;

    if(en_passant_file > 0)
        hash ^= en_passant_file_table[static_cast<std::size_t>(en_passant_file - 1)];

    hash ^= castling_right_table[castling_right];

    repetition_list[hash]++;
}

void Board::update_zobrist_variables(Square& new_square) {
    en_passant_file = -1;
    en_passant_row  = -1;

    if(!selected_square->piece) return;

    const auto& np        = new_square.piece;
    const auto& sp        = selected_square->piece;
    const auto& type      = sp->get_type();
    const bool  has_moved = sp->has_moved;

    const bool is_pawn = type == Piece::TYPE::PAWN;
    const bool is_king = type == Piece::TYPE::KING;
    const bool is_rook = type == Piece::TYPE::ROOK;

    if(is_pawn || np)
        repetition_list.clear();

    // check en passant possibility
    if(is_pawn && !has_moved) {
        if(std::abs( new_square.coordinate.row - selected_square->coordinate.row ) > 1) {
            en_passant_file = new_square.coordinate.col;
            en_passant_row  = new_square.coordinate.row - (sp->is_white() ? -1 : +1);
        }
    }

    // check for king move
    if(is_king && !has_moved) {
        if(sp->is_white())
            castling_right &= ~( static_cast<std::size_t>(CR::WK) | static_cast<std::size_t>(CR::WQ) );
        else
            castling_right &= ~( static_cast<std::size_t>(CR::BK) | static_cast<std::size_t>(CR::BQ) );
    }

    // check for rook move
    if(is_rook && !has_moved) {
        if(selected_square->coordinate.col == 8) // king side rook moved
            castling_right &= ~static_cast<std::size_t>(sp->is_white() ? CR::WK : CR::BK);
        else // queen side rook moved
            castling_right &= ~static_cast<std::size_t>(sp->is_white() ? CR::WQ : CR::BQ);
    }

    // if a rook is captured
    if(np && np->get_type() == Piece::TYPE::ROOK && !np->has_moved) {
        if(new_square.coordinate.col == 8) // king side rook captured
            castling_right &= ~static_cast<std::size_t>(np->is_white() ? CR::WK : CR::BK);
        else // queen side rook captured
            castling_right &= ~static_cast<std::size_t>(np->is_white() ? CR::WQ : CR::BQ);
    }
}

void Board::check_game_state() {
    if(repetition_list[hash] >= 3) {
        // handle draw
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

    for(auto& piece: w_pieces) {
        if(!piece->is_alive) continue;
        piece->attacked_by.clear();
        piece->is_pinned = false;
    }

    for(auto& piece: b_pieces) {
        if(!piece->is_alive) continue;
        piece->attacked_by.clear();
        piece->is_pinned = false;
    }
}

const std::shared_ptr<Piece>& Board::get_king(const Piece::COLOR color) const {
    return color == Piece::COLOR::WHITE ? w_pieces.back() : b_pieces.back();
}
