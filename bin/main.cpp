#include <iostream>
#include <SFML/Graphics.hpp>
#include <TGUI/TGUI.hpp>
#include "simdart/Utils.h"
#include "simdart/GUI.h"

int main() {
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH + 400, WINDOW_HEIGHT), "Particle system", sf::Style::Default);
    window.setFramerateLimit(60);

    tgui::GuiSFML widgets(window);

    sf::Font font;
    if (!font.loadFromFile("DejaVuSans.ttf")) {
        std::exit(1);
    }

    GUI gui{&window, &font, &widgets};
    gui.main_loop();

    return 0;
}
