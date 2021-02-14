#pragma once

#include <SFML/Graphics.hpp>
#include <sstream>
#include <vector>
#include <omp.h>
#include "Utils.h"
#include "Memory.h"
#include "TimeIt.h"

class Field {
    sf::Event event{};
    sf::RenderWindow *window;
    sf::Font *font;
    float dt = 0.f;
    FPS fps;
    bool mouse_pressed = false;
    bool texture_mapping = true;

    sf::Clock clock;

    void simulate();

    void info_text();

public:
    explicit Field(sf::RenderWindow *window, sf::Font *font);

    void run();
};