#include "Grayscale.hpp"

#include "../SIMD_Util/SIMD_Util.h"


void grayscale_ref(Image &src, Image &dst) {
    for (size_t i = 0; i < src.rgbSize; i++) {
        uint8_t average = (src.R[i] + src.G[i] + src.B[i]) / 3;
        dst.R[i] = dst.G[i] = dst.B[i] = average;
        if (src.channels == 4) {
            dst.A[i] = src.A[i];
        }
    }
}

void grayscale_simd(Image &src, Image &dst) {
    __m256i coeff = _mm256_set1_epi16(43);
    __m128i count = _mm_set1_epi64x(7); // 43 / 128 == 0.3359375

    size_t rounded_down = src.rgbSize / 16;

    const auto *RP = reinterpret_cast<const __m128i *>(src.R.data());
    const auto *GP = reinterpret_cast<const __m128i *>(src.G.data());
    const auto *BP = reinterpret_cast<const __m128i *>(src.B.data());
    const auto *AP = reinterpret_cast<const __m128i *>(src.A.data());

    auto *nRP = reinterpret_cast<__m128i *>(dst.R.data());
    auto *nGP = reinterpret_cast<__m128i *>(dst.G.data());
    auto *nBP = reinterpret_cast<__m128i *>(dst.B.data());
    auto *nAP = reinterpret_cast<__m128i *>(dst.A.data());

    for (size_t i = 0; i < rounded_down; i++) {
        __m128i r = _mm_load_si128(RP + i);
        __m128i g = _mm_load_si128(GP + i);
        __m128i b = _mm_load_si128(BP + i);

        __m256i r16 = _mm256_cvtepu8_epi16(r);
        __m256i g16 = _mm256_cvtepu8_epi16(g);
        __m256i b16 = _mm256_cvtepu8_epi16(b);

        __m256i res = _mm256_adds_epu16(r16, g16);
        res = _mm256_adds_epu16(res, b16);

        res = _mm256_mullo_epi16(res, coeff);
        res = _mm256_srl_epi16(res, count);

        __m128i res128 = cvtepu16_epu8(res);

        _mm_store_si128(nRP + i, res128);
        _mm_store_si128(nGP + i, res128);
        _mm_store_si128(nBP + i, res128);

        if (src.channels == 4) {
            __m128i a = _mm_load_si128(AP + i);
            _mm_store_si128(nAP + i, a);
        }
    }

    for (size_t i = rounded_down * 16; i < src.rgbSize; i++) {
        uint8_t average = (src.R[i] + src.G[i] + src.B[i]) / 3;
        dst.R[i] = dst.G[i] = dst.B[i] = average;
        if (src.channels == 4) {
            dst.A[i] = src.A[i];
        }
    }
}

