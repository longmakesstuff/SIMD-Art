#pragma once

#include <SFML/Graphics.hpp>
#include <sstream>
#include <vector>
#include <omp.h>
#include <immintrin.h>
#include <cstring>
#include <array>
#include "Utils.h"
#include "TimeIt.h"

class Field {
    sf::Event event{};
    sf::RenderWindow *window;
    sf::Font *font;
    float dt = 0.f;
    FPS fps;
    bool mouse_pressed = false;
    constexpr static bool texture_mapping = false;
    sf::Clock clock;

    static constexpr uint32_t block_size = 8;

    static constexpr uint32_t blocks = 100000;

    static constexpr uint32_t n = block_size * blocks;

    alignas(32) fpt *pos_x;
    alignas(32) fpt *pos_y;
    alignas(32) fpt *v_x;
    alignas(32) fpt *v_y;
    alignas(32) fpt *g_force_x;
    alignas(32) fpt *g_force_y;
    alignas(32) fpt *masses;
    sf::Color *colors;
    sf::Color *texture;
    sf::VertexBuffer v_buffer{sf::Points, sf::VertexBuffer::Static};
    sf::Vertex * v_arr;

    void simulate();

    void intrinsic_simulate();

    void info_text();

public:
    explicit Field(sf::RenderWindow *window, sf::Font *font);

    void run();
};