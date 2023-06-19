#include <cstdint>

#define RGB_MAX (-1)

#include <immintrin.h>

#include "Inversion.hpp"


void inversion_ref(Image &src, Image &dst) {
    for (size_t i = 0; i < src.rgbSize; i++) {
        dst.R[i] = RGB_MAX - src.R[i];
        dst.G[i] = RGB_MAX - src.G[i];
        dst.B[i] = RGB_MAX - src.B[i];
        if (src.channels == 4) {
            dst.A[i] = src.A[i];
        }
    }
}

void inversion_simd(Image &src, Image &dst) {
    __m256i rgb_max = _mm256_set1_epi8(RGB_MAX);

    size_t rounded_down = src.rgbSize / 32;

    // using registers to store pointers
    const auto *RP = reinterpret_cast<const __m256i *>(src.R.data());
    const auto *GP = reinterpret_cast<const __m256i *>(src.G.data());
    const auto *BP = reinterpret_cast<const __m256i *>(src.B.data());
    const auto *AP = reinterpret_cast<const __m256i *>(src.A.data());

    auto *nRP = reinterpret_cast<__m256i *>(dst.R.data());
    auto *nGP = reinterpret_cast<__m256i *>(dst.G.data());
    auto *nBP = reinterpret_cast<__m256i *>(dst.B.data());
    auto *nAP = reinterpret_cast<__m256i *>(dst.A.data());

    for (size_t i = 0; i < rounded_down; i++) {
        __m256i r = _mm256_load_si256(RP + i);
        __m256i g = _mm256_load_si256(GP + i);
        __m256i b = _mm256_load_si256(BP + i);

        r = _mm256_subs_epu8(rgb_max, r);
        g = _mm256_subs_epu8(rgb_max, g);
        b = _mm256_subs_epu8(rgb_max, b);

        _mm256_store_si256(nRP + i, r);
        _mm256_store_si256(nGP + i, g);
        _mm256_store_si256(nBP + i, b);

        if (src.channels == 4) {
            // loop fusion
            __m256i a = _mm256_load_si256(AP + i);
            _mm256_store_si256(nAP + i, a);
        }
    }

    for (size_t i = rounded_down * 32; i < src.rgbSize; i++) {
        dst.R[i] = RGB_MAX - src.R[i];
        dst.G[i] = RGB_MAX - src.G[i];
        dst.B[i] = RGB_MAX - src.B[i];
        if (src.channels == 4) {
            dst.A[i] = src.A[i];
        }
    }

}

