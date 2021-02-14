#include <iostream>
#include <SFML/Graphics.hpp>
#include "Utils.h"
#include "GUI.h"

int main() {
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Particle system", sf::Style::Default);
    window.setFramerateLimit(60);

    sf::Font font;
    if (!font.loadFromFile("DejaVuSans.ttf")) {
        LOG_ERROR("Can not load DejaVuSans.ttf. Exit");
        std::exit(1);
    }

    GUI gui{&window, &font};
    gui.main_loop();

    return 0;
}
