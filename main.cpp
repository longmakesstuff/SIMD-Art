#include <iostream>
#include <SFML/Graphics.hpp>
#include <TGUI/TGUI.hpp>
#include "Utils.h"
#include "GUI.h"

int main() {
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH + 400, WINDOW_HEIGHT), "Particle system", sf::Style::Default);
    window.setFramerateLimit(60);

    tgui::GuiSFML widgets(window);

    sf::Font font;
    if (!font.loadFromFile("DejaVuSans.ttf")) {
        LOG_ERROR("Can not load DejaVuSans.ttf. Exit");
        std::exit(1);
    }

    GUI gui{&window, &font, &widgets};
    gui.main_loop();

    return 0;
}
