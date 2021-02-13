#pragma once

#include <SFML/Graphics.hpp>
#include <sstream>
#include <vector>
#include "Utils.h"
#include "Particle.h"

class Field {
    std::vector<Particle> particles;
    sf::Event event{};
    sf::RenderWindow *window;
    sf::Font *font;
    float dt = 0.f;
    FPS fps;
    bool mouse_pressed = false;

    sf::Clock clock;

    void simulate();

    void info_text();

public:
    explicit Field(sf::RenderWindow *window, sf::Font *font);

    void run();
};