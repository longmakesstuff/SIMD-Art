#include "Particle.h"

Particle::Particle(uint32_t index) : index(index) {}

void Particle::draw(sf::RenderWindow &window) const {
    // Particle motion
    sf::CircleShape circle(1);
    circle.setFillColor(sf::Color::White);
    circle.setPosition(pos_x[index], pos_y[index]);
    window.draw(circle);
}

void Particle::summarize_g_force(const std::vector<Particle> &particles, bool mouse_pressed) const {
    sf::Vector2<fpt> g_force(0, 0);
    sf::Vector2<fpt> this_p(pos_x[index], pos_y[index]);
    fpt this_m = m[index];

    uint32_t to_iterate = mouse_pressed ? 1 : 0;
    for (uint32_t i = 0; i < to_iterate; i++) {
        Particle particle = particles[i];
        if (particle != *this) {
            fpt other_m = m[particle.index];
            sf::Vector2<fpt> other_p(pos_x[particle.index], pos_y[particle.index]);
            sf::Vector2<fpt> direction = other_p - this_p;
            fpt distance = MAX(30.0, norm(direction));
            g_force += other_m * (direction / std::pow(distance, 3));
        }
    }
    g_force *= this_m * UGC;
    g_force_x[index] = g_force.x;
    g_force_y[index] = g_force.y;
}

bool Particle::operator==(const Particle &rhs) const {
    return index == rhs.index;
}

bool Particle::operator!=(const Particle &rhs) const {
    return !(rhs == *this);
}
