#ifndef GRAVITATION_MEMORY_H
#define GRAVITATION_MEMORY_H

#include <random>
#include "Utils.h"

constexpr uint32_t n = 2;

extern fpt * pos_x;
extern fpt * pos_y;
extern fpt * v_x;
extern fpt * v_y;
extern fpt * g_force_x;
extern fpt * g_force_y;
extern fpt * m;

void initialize_memory();

#endif //GRAVITATION_MEMORY_H
