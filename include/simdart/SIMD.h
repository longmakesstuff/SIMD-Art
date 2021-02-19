#ifndef GRAVITATION_SIMD_H
#define GRAVITATION_SIMD_H

#include <immintrin.h>

using simd_fpt = __m256;

/**
 * My CPU does not support the native loading instruction so I have load the data by myself
 * @param ptr float pointer
 * @return 256bit vector
 */
inline simd_fpt _mm256_load(const float *ptr) {
    return _mm256_set_ps(ptr[7], ptr[6], ptr[5], ptr[4], ptr[3], ptr[2], ptr[1], ptr[0]);
}

inline float reduce_add_ps(simd_fpt x) {
    /* ( x3+x7, x2+x6, x1+x5, x0+x4 ) */
    const __m128 x128 = _mm_add_ps(_mm256_extractf128_ps(x, 1), _mm256_castps256_ps128(x));
    /* ( -, -, x1+x3+x5+x7, x0+x2+x4+x6 ) */
    const __m128 x64 = _mm_add_ps(x128, _mm_movehl_ps(x128, x128));
    /* ( -, -, -, x0+x1+x2+x3+x4+x5+x6+x7 ) */
    const __m128 x32 = _mm_add_ss(x64, _mm_shuffle_ps(x64, x64, 0x55));
    /* Conversion to float is a no-op on x86-64 */
    return _mm_cvtss_f32(x32);
}

constexpr auto simd_set = _mm256_set1_ps;
constexpr auto simd_load = _mm256_load;
constexpr auto simd_sub = _mm256_sub_ps;
constexpr auto simd_add = _mm256_add_ps;
constexpr auto simd_mul = _mm256_mul_ps;
constexpr auto simd_div = _mm256_div_ps;
constexpr auto simd_sqrt = _mm256_sqrt_ps;
constexpr auto simd_max = _mm256_max_ps;
constexpr auto simd_min = _mm256_min_ps;

#endif //GRAVITATION_SIMD_H
