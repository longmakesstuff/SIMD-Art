#ifndef GRAVITATION_SIMD_H
#define GRAVITATION_SIMD_H

#include <immintrin.h>

/**
 * My CPU does not support the native loading instruction so I have load the data by myself
 * @param ptr float pointer
 * @return 256bit vector
 */
inline __m256 _mm256_load(const float * ptr) {
    return _mm256_set_ps(ptr[7], ptr[6], ptr[5],ptr[4],ptr[3],ptr[2],ptr[1],ptr[0]);
}

using simd_fpt = __m256;
constexpr auto simd_set = _mm256_set1_ps;
constexpr auto simd_load = _mm256_load;
constexpr auto simd_sub = _mm256_sub_ps;
constexpr auto simd_add = _mm256_add_ps;
constexpr auto simd_mul = _mm256_mul_ps;
constexpr auto simd_div = _mm256_div_ps;
constexpr auto simd_sqrt = _mm256_sqrt_ps;
constexpr auto simd_max = _mm256_max_ps;

#endif //GRAVITATION_SIMD_H
