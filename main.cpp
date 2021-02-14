#pragma GCC optimize("O3","unroll-loops","omit-frame-pointer","inline")
#pragma GCC option("arch=native","tune=native","no-zeroupper")
#pragma GCC target("avx")

#include <iostream>
#include <SFML/Graphics.hpp>
#include "Utils.h"
#include "Field.h"
#include "Memory.h"

int main() {
    initialize_memory();

    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Particle system", sf::Style::Default);
    window.setFramerateLimit(60);

    sf::Font font;
    if (!font.loadFromFile("DejaVuSans.ttf")) {
        LOG_ERROR("Can not load DejaVuSans.ttf. Exit");
        std::exit(1);
    }

    Field universe{&window, &font};
    universe.run();

    return 0;
}
