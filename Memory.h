#ifndef GRAVITATION_MEMORY_H
#define GRAVITATION_MEMORY_H

#include <random>
#include <SFML/Graphics.hpp>
#include "Utils.h"

// How many particles
constexpr uint32_t block_size = 8;

constexpr uint32_t blocks = 100000;

// I am lazy so currently I only support perfect blocks
constexpr uint32_t n = block_size * blocks;

alignas(32) extern fpt * pos_x;
alignas(32) extern fpt * pos_y;
alignas(32) extern fpt * v_x;
alignas(32) extern fpt * v_y;
alignas(32) extern fpt * g_force_x;
alignas(32) extern fpt * g_force_y;
alignas(32) extern fpt * masses;
extern sf::Color * colors;
extern sf::Color * texture;
extern sf::VertexArray pixels;
void initialize_memory();

#endif //GRAVITATION_MEMORY_H
