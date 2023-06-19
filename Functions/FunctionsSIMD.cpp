#include "../Image/Image.hpp"
#include "Functions.hpp"
#include "../SIMD_Util/SIMD_Util.h"
#include "../SIMD_Util/avx_mathfun.h"

#include <cmath>
#include <algorithm>

#include <immintrin.h>

void pow_simd(Image &src, Image &dst, double c) {
    const __m256 y = _mm256_set1_ps(c);
    const __m256 uchar_max = _mm256_set1_ps(UCHAR_MAX);
    const __m256 one = _mm256_set1_ps(1);

    const auto *RP = reinterpret_cast<const __m128i *>(src.R.data());
    const auto *GP = reinterpret_cast<const __m128i *>(src.G.data());
    const auto *BP = reinterpret_cast<const __m128i *>(src.B.data());
    const auto *AP = reinterpret_cast<const __m128i *>(src.A.data());

    auto *nRP = reinterpret_cast<__m128i *>(dst.R.data());
    auto *nGP = reinterpret_cast<__m128i *>(dst.G.data());
    auto *nBP = reinterpret_cast<__m128i *>(dst.B.data());
    auto *nAP = reinterpret_cast<__m128i *>(dst.A.data());

    size_t rounded_down = src.rgbSize / 16;
    for (size_t i = 0; i < rounded_down; i++) {
        __m128i r = _mm_load_si128(RP + i);
        __m128i g = _mm_load_si128(GP + i);
        __m128i b = _mm_load_si128(BP + i);

        r = pow_epi8(r, y, uchar_max, one);
        g = pow_epi8(g, y, uchar_max, one);
        b = pow_epi8(b, y, uchar_max, one);

        _mm_store_si128(nRP + i, r);
        _mm_store_si128(nGP + i, g);
        _mm_store_si128(nBP + i, b);

        if (src.channels == 4) {
            __m128i a = _mm_load_si128(AP + i);
            _mm_store_si128(nAP + i, a);
        }
    }

    double MIN = 0;
    double MAX = 255;
    for (size_t i = rounded_down * 16; i < src.rgbSize; i++) {
        dst.R[i] = static_cast<uint8_t>(std::clamp(pow(src.R[i], c), MIN, MAX));
        dst.G[i] = static_cast<uint8_t>(std::clamp(pow(src.G[i], c), MIN, MAX));
        dst.B[i] = static_cast<uint8_t>(std::clamp(pow(src.B[i], c), MIN, MAX));
        if (src.channels == 4) {
            dst.A[i] = src.A[i];
        }
    }
}

void log_simd(Image &src, Image &dst) {
    double c = 255.0 / log(1 + 255); // max value is 255
    const __m256 coeff = _mm256_set1_ps(c);

    const auto *RP = reinterpret_cast<const __m128i *>(src.R.data());
    const auto *GP = reinterpret_cast<const __m128i *>(src.G.data());
    const auto *BP = reinterpret_cast<const __m128i *>(src.B.data());
    const auto *AP = reinterpret_cast<const __m128i *>(src.A.data());

    auto *nRP = reinterpret_cast<__m128i *>(dst.R.data());
    auto *nGP = reinterpret_cast<__m128i *>(dst.G.data());
    auto *nBP = reinterpret_cast<__m128i *>(dst.B.data());
    auto *nAP = reinterpret_cast<__m128i *>(dst.A.data());

    size_t rounded_down = src.rgbSize / 16;

    for (size_t i = 0; i < rounded_down; i++) {
        __m128i r = _mm_load_si128(RP + i);
        __m128i g = _mm_load_si128(GP + i);
        __m128i b = _mm_load_si128(BP + i);

        r = log_epi8(r, coeff);
        g = log_epi8(g, coeff);
        b = log_epi8(b, coeff);

        _mm_store_si128(nRP + i, r);
        _mm_store_si128(nGP + i, g);
        _mm_store_si128(nBP + i, b);

        if (src.channels == 4) {
            __m128i a = _mm_load_si128(AP + i);
            _mm_store_si128(nAP + i, a);
        }
    }

    for (size_t i = rounded_down * 16; i < src.rgbSize; i++) {
        dst.R[i] = static_cast<uint8_t>(c * log(src.R[i] + 1));
        dst.G[i] = static_cast<uint8_t>(c * log(src.G[i] + 1));
        dst.B[i] = static_cast<uint8_t>(c * log(src.B[i] + 1));
        if (src.channels == 4) {
            dst.A[i] = src.A[i];
        }
    }
}

void abs_simd(Image &src, Image &dst) {
    const auto *RP = reinterpret_cast<const __m256i *>(src.R.data());
    const auto *GP = reinterpret_cast<const __m256i *>(src.G.data());
    const auto *BP = reinterpret_cast<const __m256i *>(src.B.data());
    const auto *AP = reinterpret_cast<const __m256i *>(src.A.data());

    auto *nRP = reinterpret_cast<__m256i *>(dst.R.data());
    auto *nGP = reinterpret_cast<__m256i *>(dst.G.data());
    auto *nBP = reinterpret_cast<__m256i *>(dst.B.data());
    auto *nAP = reinterpret_cast<__m256i *>(dst.A.data());

    size_t rounded_down = src.rgbSize / 32;
    for (size_t i = 0; i < rounded_down; i++) {
        __m256i r = _mm256_load_si256(RP + i);
        __m256i g = _mm256_load_si256(GP + i);
        __m256i b = _mm256_load_si256(BP + i);

        r = _mm256_abs_epi8(r);
        g = _mm256_abs_epi8(g);
        b = _mm256_abs_epi8(b);

        _mm256_store_si256(nRP + i, r);
        _mm256_store_si256(nGP + i, g);
        _mm256_store_si256(nBP + i, b);

        if (src.channels == 4) {
            __m256i a = _mm256_load_si256(AP + i);
            _mm256_store_si256(nAP + i, a);
        }
    }

    for (size_t i = rounded_down * 32; i < src.rgbSize; i++) {
        dst.R[i] = std::abs(static_cast<int8_t>(src.R[i]));
        dst.G[i] = std::abs(static_cast<int8_t>(src.G[i]));
        dst.B[i] = std::abs(static_cast<int8_t>(src.B[i]));
        if (src.channels == 4) {
            dst.A[i] = src.A[i];
        }
    }
}

void min_simd(Image &src, Image &dst, uint8_t c) {
    __m256i coeff = _mm256_set1_epi8(c);

    const auto *RP = reinterpret_cast<const __m256i *>(src.R.data());
    const auto *GP = reinterpret_cast<const __m256i *>(src.G.data());
    const auto *BP = reinterpret_cast<const __m256i *>(src.B.data());
    const auto *AP = reinterpret_cast<const __m256i *>(src.A.data());

    auto *nRP = reinterpret_cast<__m256i *>(dst.R.data());
    auto *nGP = reinterpret_cast<__m256i *>(dst.G.data());
    auto *nBP = reinterpret_cast<__m256i *>(dst.B.data());
    auto *nAP = reinterpret_cast<__m256i *>(dst.A.data());

    size_t rounded_down = src.rgbSize / 32;
    for (size_t i = 0; i < rounded_down; i++) {
        __m256i r = _mm256_load_si256(RP + i);
        __m256i g = _mm256_load_si256(GP + i);
        __m256i b = _mm256_load_si256(BP + i);

        r = _mm256_min_epu8(r, coeff);
        g = _mm256_min_epu8(g, coeff);
        b = _mm256_min_epu8(b, coeff);

        _mm256_store_si256(nRP + i, r);
        _mm256_store_si256(nGP + i, g);
        _mm256_store_si256(nBP + i, b);

        if (src.channels == 4) {
            __m256i a = _mm256_load_si256(AP + i);
            _mm256_store_si256(nAP + i, a);
        }
    }

    for (size_t i = rounded_down * 32; i < src.rgbSize; i++) {
        dst.R[i] = std::min(src.R[i], c);
        dst.G[i] = std::min(src.G[i], c);
        dst.B[i] = std::min(src.B[i], c);
        if (src.channels == 4) {
            dst.A[i] = src.A[i];
        }
    }
}

void max_simd(Image &src, Image &dst, uint8_t c) {
    __m256i coeff = _mm256_set1_epi8(c);

    const auto *RP = reinterpret_cast<const __m256i *>(src.R.data());
    const auto *GP = reinterpret_cast<const __m256i *>(src.G.data());
    const auto *BP = reinterpret_cast<const __m256i *>(src.B.data());
    const auto *AP = reinterpret_cast<const __m256i *>(src.A.data());

    auto *nRP = reinterpret_cast<__m256i *>(dst.R.data());
    auto *nGP = reinterpret_cast<__m256i *>(dst.G.data());
    auto *nBP = reinterpret_cast<__m256i *>(dst.B.data());
    auto *nAP = reinterpret_cast<__m256i *>(dst.A.data());

    size_t rounded_down = src.rgbSize / 32;
    for (size_t i = 0; i < rounded_down; i++) {
        __m256i r = _mm256_load_si256(RP + i);
        __m256i g = _mm256_load_si256(GP + i);
        __m256i b = _mm256_load_si256(BP + i);

        r = _mm256_max_epu8(r, coeff);
        g = _mm256_max_epu8(g, coeff);
        b = _mm256_max_epu8(b, coeff);

        _mm256_store_si256(nRP + i, r);
        _mm256_store_si256(nGP + i, g);
        _mm256_store_si256(nBP + i, b);

        if (src.channels == 4) {
            __m256i a = _mm256_load_si256(AP + i);
            _mm256_store_si256(nAP + i, a);
        }
    }

    for (size_t i = rounded_down * 32; i < src.rgbSize; i++) {
        dst.R[i] = std::max(src.R[i], c);
        dst.G[i] = std::max(src.G[i], c);
        dst.B[i] = std::max(src.B[i], c);
        if (src.channels == 4) {
            dst.A[i] = src.A[i];
        }
    }
}
