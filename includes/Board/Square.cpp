#include "Board.hpp"

void Board::Square::highlight(const sf::Color& color) {
    shape.setFillColor(color);
}

void Board::Square::highlight() {
    shape.setFillColor(
        type == COLOR::LIGHT ? Square::light_highlight : Square::dark_highlight
    );
}

void Board::Square::unhighlight() {
    shape.setFillColor(
        type == COLOR::LIGHT ? Square::light_color : Square::dark_color
    );
}

void Board::Square::draw(const Board& board) {
    board.render_window.draw(shape);
    if(piece && piece->is_alive) piece->draw();
}

bool Board::Square::is_a_legal_moves(const Board& board) const {
    auto it = std::find(board.highlighted_coord.begin(), board.highlighted_coord.end(), coordinate);
    return it != board.highlighted_coord.end();
}
