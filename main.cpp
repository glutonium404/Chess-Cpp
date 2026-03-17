#include <SFML/Graphics.hpp>
#include "includes/Board.hpp"

int main() {
    sf::RenderWindow window(sf::VideoMode(640, 480), "Hello World");
    Board board(450, window);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        board.draw();
        window.display();
    }
    return 0;
}
