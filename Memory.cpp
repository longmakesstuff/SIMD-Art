#include "Memory.h"

fpt * pos_x = new fpt[n + 1];
fpt * pos_y = new fpt[n + 1];
fpt * v_x = new fpt[n + 1];
fpt * v_y = new fpt[n + 1];
fpt * g_force_x = new fpt[n + 1];
fpt * g_force_y = new fpt[n + 1];
fpt * m = new fpt[n + 1];
sf::Color * colors = new sf::Color[n];
sf::Color * texture = new sf::Color[WINDOW_HEIGHT * WINDOW_WIDTH];
extern sf::VertexArray pixels(sf::Points, n);

void initialize_memory() {
    std::mt19937 rng;
    std::uniform_real_distribution<fpt> pos(100.0, 900.0);

    for (uint32_t i = 0; i < n; i++) {
        pos_x[i] = pos(rng);
        pos_y[i] = pos(rng);
        v_x[i] = 0.0;
        v_y[i] = 0.0;
        g_force_x[i] = 0.0;
        g_force_y[i] = 0.0;
        m[i] = 10000;
        colors[i].g = 255;
        colors[i].b = 255;
    }

    m[n] = 10000000000000;
}
