#pragma once

#include <SFML/Graphics.hpp>
#include <sstream>
#include <vector>
#include <omp.h>
#include <cstring>
#include <array>
#include "Utils.h"
#include "TimeIt.h"
#include "Simd.h"


class Field {
    // SFML specific stuff
    sf::Event event{};
    sf::RenderWindow *window;
    sf::Font *font;
    FPS fps;
    sf::Clock clock;

    // Delta time between two simulations
    fpt dt = 0.f;

    // Is the mouse currently pressed?
    bool mouse_pressed = false;

    // Do we want to use texture mapping?
    constexpr static bool texture_mapping = true;

    // SIMD instruction size
    static constexpr uint32_t block_size = 8;

    // How many blocks do we want to simulate
    static constexpr uint32_t blocks = 10000;

    // How many particles do we want to simulate
    static constexpr uint32_t n = block_size * blocks;

    // Mass of mouse point, the heavier, the faster the particles
    static constexpr uint32_t mouse_mass = 25000000;

    // Mass of a particle, the heavier, the slower the particles
    static constexpr uint32_t particle_mass = 10000;

    // Minimal distance for better effect, the smaller, the more unstable will the numerical results get
    static constexpr fpt minimal_distance = 75;

    // Slowing particles over time
    static constexpr fpt drag_coef = 0.025;

    // Link to the texture
    const std::string texture_file = "grumpy-cat.jpg";

    // Buffer for SIMD parallelization
    alignas(32) fpt *pos_x;
    alignas(32) fpt *pos_y;
    alignas(32) fpt *v_x;
    alignas(32) fpt *v_y;
    alignas(32) fpt *masses;

    // Particle's graphical information
    sf::Color *colors;
    sf::Color *texture;
    sf::VertexBuffer vertex_buffer{sf::Points, sf::VertexBuffer::Static};
    sf::Vertex * vertices;

    /**
     * Simulate naively with only thread level parallelization
     */
    void naive_simulate();

    /**
     * Accelerated simulation with SIMD
     */
    void simd_simulate();

    /**
     * Information of FPS
     */
    void info_text();

public:
    explicit Field(sf::RenderWindow *window, sf::Font *font);

    virtual ~Field();

    void run();
};