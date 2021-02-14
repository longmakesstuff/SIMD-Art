#include <immintrin.h>
#include <iostream>

int main() {
    float a[8] {1,2,3,4,1,2,3,4};
    float b[8] {1,2,3,4,4,3,2,1};
    __m256 a_ = _mm256_load_ps(a);
    __m256 b_ = _mm256_load_ps(b);
    __m256 c = _mm256_max_ps(a_, b_);

    auto out = (float *)&c;
    for(u_int32_t i = 0; i < 8; i++) {
        std::cout << (float )out[i] << std::endl;
    }
}