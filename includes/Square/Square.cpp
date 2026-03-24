#include "Square.hpp"
#include "../Piece/Piece.hpp"

void Square::highlight(const sf::Color& color) {
    shape.setFillColor(color);
}

void Square::highlight() {
    shape.setFillColor(
        type == COLOR::LIGHT ? Square::light_highlight : Square::dark_highlight
    );
}

void Square::unhighlight() {
    shape.setFillColor(
        type == COLOR::LIGHT ? Square::light_color : Square::dark_color
    );
}

void Square::draw(sf::RenderWindow& render_window) {
    render_window.draw(shape);
    if(piece && piece->is_alive) piece->draw();
}

bool Square::is_legal_move() const {
    const sf::Color& color = shape.getFillColor();
    return (color == light_highlight) || (color == dark_highlight) || (color == red_highlight);
}
