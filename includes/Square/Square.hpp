#pragma once

#include "../Coordinate/Coordinate.hpp"
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <memory>

class Piece;

class Square {
public:
    enum class COLOR { LIGHT, DARK, RED };

    inline static const sf::Color light_color     = sf::Color(235, 236, 208);
    inline static const sf::Color dark_color      = sf::Color(115, 149, 82);

    inline static const sf::Color light_highlight = sf::Color(245, 246, 130);
    inline static const sf::Color dark_highlight  = sf::Color(185, 202, 67 );
    inline static const sf::Color red_highlight   = sf::Color(149, 82 , 82 );
    inline static const sf::Color check_highlight = sf::Color(235, 61 , 61 );

    COLOR                   type;
    Coordinate              coordinate = Coordinate(0, 0);
    sf::RectangleShape      shape;
    std::shared_ptr<Piece>  piece = nullptr;

    bool is_controlled_by_white = false;
    bool is_controlled_by_black = false;

    void highlight();
    void highlight(const sf::Color& color);
    void unhighlight();
    void draw(sf::RenderWindow& render_window);

    bool is_legal_move() const;
};
