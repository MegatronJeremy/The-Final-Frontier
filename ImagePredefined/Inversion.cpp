#include <cstdint>

#define RGB_MAX (-1)

#include <immintrin.h>

void inversion(size_t rgb_size, int channels,
               const uint8_t *R, const uint8_t *G, const uint8_t *B, const uint8_t *A,
               uint8_t *nR, uint8_t *nG, uint8_t *nB, uint8_t *nA) {
    for (size_t i = 0; i < rgb_size; i++) {
        nR[i] = RGB_MAX - R[i];
        nG[i] = RGB_MAX - G[i];
        nB[i] = RGB_MAX - B[i];
        if (channels == 4) {
            nA[i] = A[i];
        }
    }
}

void inversion_simd(size_t rgb_size, int channels,
                    const uint8_t *R, const uint8_t *G, const uint8_t *B, const uint8_t *A,
                    uint8_t *nR, uint8_t *nG, uint8_t *nB, uint8_t *nA) {

    __m256i rgb_max = _mm256_set1_epi8(RGB_MAX);

    size_t rounded_down = rgb_size / 32;

    const auto *RP = reinterpret_cast<const __m256i *>(R);
    const auto *GP = reinterpret_cast<const __m256i *>(G);
    const auto *BP = reinterpret_cast<const __m256i *>(B);
    const auto *AP = reinterpret_cast<const __m256i *>(A);

    auto *nRP = reinterpret_cast<__m256i *>(nR);
    auto *nGP = reinterpret_cast<__m256i *>(nG);
    auto *nBP = reinterpret_cast<__m256i *>(nB);
    auto *nAP = reinterpret_cast<__m256i *>(nA);

    for (size_t i = 0; i < rounded_down; i++) {
        __m256i r = _mm256_load_si256(RP + i);
        __m256i g = _mm256_load_si256(GP + i);
        __m256i b = _mm256_load_si256(BP + i);

        r = _mm256_sub_epi8(rgb_max, r);
        g = _mm256_sub_epi8(rgb_max, g);
        b = _mm256_sub_epi8(rgb_max, b);

        _mm256_store_si256(nRP + i, r);
        _mm256_store_si256(nGP + i, g);
        _mm256_store_si256(nBP + i, b);

        if (channels == 4) {
            __m256i a = _mm256_load_si256(AP + i);
            _mm256_store_si256(nAP + i, a);
        }
    }

    for (size_t i = rounded_down * 32; i < rgb_size; i++) {
        nR[i] = RGB_MAX - R[i];
        nG[i] = RGB_MAX - G[i];
        nB[i] = RGB_MAX - B[i];
        if (channels == 4) {
            nA[i] = A[i];
        }
    }

}

