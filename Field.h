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

constexpr auto simd_set = _mm256_set1_ps;
constexpr auto simd_load = _mm256_load;
using simd_fpt = __m256;
constexpr auto simd_sub = _mm256_sub_ps;
constexpr auto simd_add = _mm256_add_ps;
constexpr auto simd_mul = _mm256_mul_ps;
constexpr auto simd_div = _mm256_div_ps;
constexpr auto simd_sqrt = _mm256_sqrt_ps;
constexpr auto simd_max = _mm256_max_ps;


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

    static constexpr uint32_t blocks = 200000;

    static constexpr uint32_t n = block_size * blocks;

    alignas(32) fpt *pos_x;
    alignas(32) fpt *pos_y;
    alignas(32) fpt *v_x;
    alignas(32) fpt *v_y;
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