#pragma once

#include <SFML/Graphics.hpp>
#include <sstream>
#include <vector>
#include <omp.h>
#include <cstring>
#include <array>
#include "simdart/Utils.h"
#include "simdart/TimeIt.h"
#include "simdart/SIMD.h"


class Field {
    // SFML specific stuff
    sf::Clock clock;

    // Delta time between two simulations
    fpt dt = 0.f;

    // Is the mouse currently pressed?
    bool mouse_pressed = false;

    // Do we want to use texture mapping?
    bool texture_mapping = true;

    // SIMD instruction size
    static constexpr uint32_t block_size = 8;

    // How many blocks do we want to simulate
    static constexpr uint32_t blocks = 125000;

    // How many particles do we want to simulate
    static constexpr uint32_t n = block_size * blocks;

    // Mass of mouse point, the heavier, the faster the particles
    uint32_t mouse_mass = 45000000;

    // Mass of a particle, the heavier, the slower the particles
    uint32_t particle_mass = 5000;

    // Minimal distance for better effect, the smaller, the more unstable will the numerical results get
    fpt minimal_distance = 80;

    // Slowing particles over time
    fpt drag_coefficient = 0.025;

    // Link to the texture
    std::string texture_file = "grumpy-cat.jpg";

    // Buffer for SIMD parallelization
    fpt *pos_x;
    fpt *pos_y;
    fpt *v_x;
    fpt *v_y;
    fpt *masses;

    // Particle's graphical information
    sf::Color *colors;
    sf::Color *texture;
    sf::VertexBuffer vertex_buffer{sf::Points, sf::VertexBuffer::Static};
    sf::Vertex * vertices;

    // SIMD constants
    simd_fpt SIMD_HALF = simd_set(0.5);
    simd_fpt SIMD_DRAG = simd_set(drag_coefficient);
    simd_fpt SIMD_MIN_DISTANCE = simd_set(minimal_distance);
    simd_fpt SIMD_ZEROS = simd_set(0.0);
    simd_fpt SIMD_SCREEN_HEIGHT = simd_set(WINDOW_HEIGHT);
    simd_fpt SIMD_SCREEN_WIDTH = simd_set(WINDOW_WIDTH);


    /**
     * Simulate naively with only thread level parallelization
     */
    void naive_simulate();

    /**
     * Accelerated simulation with SIMD
     */
    void simd_simulate();

    /**
     * Reloading texture for visual texture mapping
     */
    void load_texture();

public:
    explicit Field();

    virtual ~Field();

    void run(sf::RenderWindow *window);

    [[nodiscard]] bool isMousePressed() const;

    void setMousePressed(bool mousePressed);

    [[nodiscard]] fpt getMinimalDistance() const;

    void setMinimalDistance(fpt minimalDistance);

    [[nodiscard]] bool isTextureMapping() const;

    void setTextureMapping(bool textureMapping);

    [[nodiscard]] fpt getDragCoefficient() const;

    void setDragCoefficient(fpt dragCoefficient);

    [[nodiscard]] uint32_t getMouseMass() const;

    void setMouseMass(uint32_t mouseMass);

    [[nodiscard]] uint32_t getParticleMass() const;

    void setParticleMass(uint32_t particleMass);

    [[nodiscard]] const std::string &getTextureFile() const;

    void setTextureFile(const std::string &textureFile);
};