#pragma once

#include <SFML/Graphics.hpp>
#include "Utils.h"
#include "Memory.h"

class Particle {
public:
    const uint32_t index;

    explicit Particle(uint32_t index);

    void draw(sf::RenderWindow &window) const;

    void summarize_g_force(const std::vector<Particle> &particles, bool mouse_pressed) const;

    bool operator==(const Particle &rhs) const;

    bool operator!=(const Particle &rhs) const;
};