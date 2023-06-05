#include "Grayscale.hpp"


void grayscale_struct_vectors(size_t rgb_size, int channels,
                              const uint8_t *R, const uint8_t *G, const uint8_t *B, const uint8_t *A,
                              uint8_t *nR, uint8_t *nG, uint8_t *nB, uint8_t *nA) {
    for (size_t i = 0; i < rgb_size; i++) {
        uint8_t average = (R[i] + G[i] + B[i]) / 3;
        nR[i] = nG[i] = nB[i] = average;
        if (channels == 4) {
            nA[i] = A[i];
        }
    }
}

void grayscale_simd(size_t rgb_size, int channels,
                    const uint8_t *R, const uint8_t *G, const uint8_t *B, const uint8_t *A,
                    uint8_t *nR, uint8_t *nG, uint8_t *nB, uint8_t *nA) {

    __m256i coeff = _mm256_set1_epi16(43);
    __m128i count = _mm_set1_epi64x(7); // 43 / 128 == 0.3359375

    size_t rounded_down = rgb_size / 16;

    const auto *RP = reinterpret_cast<const __m128i *>(R);
    const auto *GP = reinterpret_cast<const __m128i *>(G);
    const auto *BP = reinterpret_cast<const __m128i *>(B);
    const auto *AP = reinterpret_cast<const __m128i *>(A);

    auto *nRP = reinterpret_cast<__m128i *>(nR);
    auto *nGP = reinterpret_cast<__m128i *>(nG);
    auto *nBP = reinterpret_cast<__m128i *>(nB);
    auto *nAP = reinterpret_cast<__m128i *>(nA);

    for (size_t i = 0; i < rounded_down; i++) {
        __m128i r = _mm_load_si128(RP + i);
        __m128i g = _mm_load_si128(GP + i);
        __m128i b = _mm_load_si128(BP + i);

        __m256i r16 = _mm256_cvtepu8_epi16(r);
        __m256i g16 = _mm256_cvtepu8_epi16(g);
        __m256i b16 = _mm256_cvtepu8_epi16(b);

        __m256i res = _mm256_add_epi16(r16, g16);
        res = _mm256_add_epi16(res, b16);

        res = _mm256_mullo_epi16(res, coeff);
        res = _mm256_srl_epi16(res, count);

        __m128i lo_lane = _mm256_castsi256_si128(res);
        __m128i hi_lane = _mm256_extracti128_si256(res, 1);
        __m128i res128 = _mm_packus_epi16(lo_lane, hi_lane);

        _mm_store_si128(nRP + i, res128);
        _mm_store_si128(nGP + i, res128);
        _mm_store_si128(nBP + i, res128);

        if (channels == 4) {
            __m128i a = _mm_load_si128(AP + i);
            _mm_store_si128(nAP + i, a);
        }
    }

    for (size_t i = rounded_down * 16; i < rgb_size; i++) {
        uint8_t average = (R[i] + G[i] + B[i]) / 3;
        nR[i] = nG[i] = nB[i] = average;
        if (channels == 4) {
            nA[i] = A[i];
        }
    }
}

