#include "Memory.h"

fpt *pos_x = new fpt[n + 1];
fpt *pos_y = new fpt[n + 1];
fpt *v_x = new fpt[n + 1];
fpt *v_y = new fpt[n + 1];
fpt *g_force_x = new fpt[n + 1];
fpt *g_force_y = new fpt[n + 1];
fpt *m = new fpt[n + 1];
sf::Color *colors = new sf::Color[n];
sf::Color *texture = new sf::Color[WINDOW_HEIGHT * WINDOW_WIDTH];
extern sf::VertexArray pixels(sf::Points, n);

void initialize_memory() {
    if (!pos_x || !pos_y || !v_x || !v_y || !g_force_x || !g_force_y || !m || !colors || !texture) {
        LOG_ERROR("Cannot allocate enough memory. Exit now!")
        std::exit(1);
    }
    // Loading texture
    sf::Image image;
    if (!image.loadFromFile("texture.jpg")) {
        LOG_ERROR("Can not load texture.jpg. Exit now")
        std::exit(1);
    }

    for (int32_t y = 0; y < WINDOW_HEIGHT; y++) {
        for (int32_t x = 0; x < WINDOW_WIDTH; x++) {
            texture[y * WINDOW_HEIGHT + x].r = image.getPixel(x, y).r;
            texture[y * WINDOW_HEIGHT + x].g = image.getPixel(x, y).g;
            texture[y * WINDOW_HEIGHT + x].b = image.getPixel(x, y).b;
        }
    }


    std::mt19937 rng;
    std::uniform_real_distribution<fpt> pos(100.0, 200.0);
    // Initialize random position
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

    // Position for mouse
    // We want mouse to dominates
    m[n] = 10000000;
}
