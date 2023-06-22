#include "../../inc/Arithmetic/Arithmetic.hpp"
#include "../../inc/SIMD_Util/SIMD_Util.h"

#include <immintrin.h>
#include <algorithm>


void add_simd(Image &src, Image &dst, uint8_t c) {
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

        r = _mm256_adds_epu8(r, coeff);
        g = _mm256_adds_epu8(g, coeff);
        b = _mm256_adds_epu8(b, coeff);

        _mm256_store_si256(nRP + i, r);
        _mm256_store_si256(nGP + i, g);
        _mm256_store_si256(nBP + i, b);

        if (src.channels == 4) {
            __m256i a = _mm256_load_si256(AP + i);
            _mm256_store_si256(nAP + i, a);
        }
    }

    for (size_t i = rounded_down * 32; i < src.rgbSize; i++) {
        dst.R[i] = std::clamp(src.R[i] + c, 0, UCHAR_MAX);
        dst.G[i] = std::clamp(src.G[i] + c, 0, UCHAR_MAX);
        dst.B[i] = std::clamp(src.B[i] + c, 0, UCHAR_MAX);
        if (src.channels == 4) {
            dst.A[i] = src.A[i];
        }
    }
}

void sub_simd(Image &src, Image &dst, uint8_t c) {
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

        r = _mm256_subs_epu8(r, coeff);
        g = _mm256_subs_epu8(g, coeff);
        b = _mm256_subs_epu8(b, coeff);

        _mm256_store_si256(nRP + i, r);
        _mm256_store_si256(nGP + i, g);
        _mm256_store_si256(nBP + i, b);

        if (src.channels == 4) {
            __m256i a = _mm256_load_si256(AP + i);
            _mm256_store_si256(nAP + i, a);
        }
    }

    for (size_t i = rounded_down * 32; i < src.rgbSize; i++) {
        dst.R[i] = std::clamp(src.R[i] - c, 0, UCHAR_MAX);
        dst.G[i] = std::clamp(src.G[i] - c, 0, UCHAR_MAX);
        dst.B[i] = std::clamp(src.B[i] - c, 0, UCHAR_MAX);
        if (src.channels == 4) {
            dst.A[i] = src.A[i];
        }
    }
}


void inv_sub_simd(Image &src, Image &dst, uint8_t c) {
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

        r = _mm256_subs_epu8(coeff, r);
        g = _mm256_subs_epu8(coeff, g);
        b = _mm256_subs_epu8(coeff, b);

        _mm256_store_si256(nRP + i, r);
        _mm256_store_si256(nGP + i, g);
        _mm256_store_si256(nBP + i, b);

        if (src.channels == 4) {
            __m256i a = _mm256_load_si256(AP + i);
            _mm256_store_si256(nAP + i, a);
        }
    }

    for (size_t i = rounded_down * 32; i < src.rgbSize; i++) {
        dst.R[i] = std::clamp(c - src.R[i], 0, UCHAR_MAX);
        dst.G[i] = std::clamp(c - src.G[i], 0, UCHAR_MAX);
        dst.B[i] = std::clamp(c - src.B[i], 0, UCHAR_MAX);
        if (src.channels == 4) {
            dst.A[i] = src.A[i];
        }
    }
}

void mul_simd(Image &src, Image &dst, uint8_t c) {
    __m128i coeff = _mm_set1_epi8(c);

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

        r = mul_epi8(r, coeff);
        g = mul_epi8(g, coeff);
        b = mul_epi8(b, coeff);

        _mm_store_si128(nRP + i, r);
        _mm_store_si128(nGP + i, g);
        _mm_store_si128(nBP + i, b);

        if (src.channels == 4) {
            __m128i a = _mm_load_si128(AP + i);
            _mm_store_si128(nAP + i, a);
        }
    }

    for (size_t i = rounded_down * 16; i < src.rgbSize; i++) {
        dst.R[i] = std::clamp(src.R[i] * c, 0, UCHAR_MAX);
        dst.G[i] = std::clamp(src.G[i] * c, 0, UCHAR_MAX);
        dst.B[i] = std::clamp(src.B[i] * c, 0, UCHAR_MAX);
        if (src.channels == 4) {
            dst.A[i] = src.A[i];
        }
    }
}

void div_simd(Image &src, Image &dst, uint8_t c) {
    __m128i coeff = _mm_set1_epi8(c);

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

        __m128i rd = div_epi8(r, coeff);
        __m128i gd = div_epi8(g, coeff);
        __m128i bd = div_epi8(b, coeff);

        _mm_store_si128(nRP + i, rd);
        _mm_store_si128(nGP + i, gd);
        _mm_store_si128(nBP + i, bd);

        if (src.channels == 4) {
            __m128i a = _mm_load_si128(AP + i);
            _mm_store_si128(nAP + i, a);
        }
    }

    for (size_t i = rounded_down * 16; i < src.rgbSize; i++) {
        dst.R[i] = src.R[i] / c;
        dst.G[i] = src.G[i] / c;
        dst.B[i] = src.B[i] / c;
        if (src.channels == 4) {
            dst.A[i] = src.A[i];
        }
    }
}

void inv_div_simd(Image &src, Image &dst, uint8_t c) {
    __m128i coeff = _mm_set1_epi8(c);
    __m128i zero = _mm_set1_epi8(0);
    __m128i ones = _mm_set1_epi8(-1);

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

        __m128i mask;

        // mask out the zeroes and replace them with the performMax value
        __m128i maskR = _mm_cmpeq_epi8(r, zero);
        __m128i rr = _mm_blendv_epi8(r, ones, mask);

        __m128i maskG = _mm_cmpeq_epi8(g, zero);
        __m128i gg = _mm_blendv_epi8(g, ones, mask);

        __m128i maskB = _mm_cmpeq_epi8(b, zero);
        __m128i bb = _mm_blendv_epi8(b, ones, mask);

        // blend with the performMax value where the original value was zero
        __m128i rd = div_epi8(coeff, rr);
        rd = _mm_blendv_epi8(rd, ones, maskR);

        __m128i gd = div_epi8(coeff, gg);
        gd = _mm_blendv_epi8(gd, ones, maskG);

        __m128i bd = div_epi8(coeff, bb);
        bd = _mm_blendv_epi8(bd, ones, maskB);

        _mm_store_si128(nRP + i, rd);
        _mm_store_si128(nGP + i, gd);
        _mm_store_si128(nBP + i, bd);

        if (src.channels == 4) {
            __m128i a = _mm_load_si128(AP + i);
            _mm_store_si128(nAP + i, a);
        }
    }

    for (size_t i = rounded_down * 16; i < src.rgbSize; i++) {
        dst.R[i] = src.R[i] != 0 ? c / src.R[i] : UCHAR_MAX;
        dst.G[i] = src.G[i] != 0 ? c / src.G[i] : UCHAR_MAX;
        dst.B[i] = src.B[i] != 0 ? c / src.B[i] : UCHAR_MAX;
        if (src.channels == 4) {
            dst.A[i] = src.A[i];
        }
    }
}
