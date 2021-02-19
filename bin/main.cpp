#include <iostream>
#include <SFML/Graphics.hpp>
#include <TGUI/TGUI.hpp>
#include <simdart/Utils.h>
#include <simdart/GUI.h>
#include <SFML/OpenGL.hpp>


int main() {
    sf::ContextSettings settings;
    settings.depthBits = 24;
    settings.stencilBits = 8;
    settings.antialiasingLevel = 4;
    settings.majorVersion = 3;
    settings.minorVersion = 0;
    sf::RenderWindow window(
            sf::VideoMode(WINDOW_WIDTH + 400, WINDOW_HEIGHT),
            "Particle system",
            sf::Style::Default,
            settings);
    window.setFramerateLimit(60);

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_POINT_SMOOTH);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glPointSize(1);

    tgui::GuiSFML widgets(window);

    sf::Font font;
    if (!font.loadFromFile("DejaVuSans.ttf")) {
        std::exit(1);
    }

    GUI gui{&window, &font, &widgets};
    gui.main_loop();

    return 0;
}
