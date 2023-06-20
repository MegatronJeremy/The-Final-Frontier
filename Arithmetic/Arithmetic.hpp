#pragma once

#include "../Image/Image.hpp"
#include <iostream>

#include <immintrin.h>
#include <algorithm>

void add_ref(Image &src, Image &dst, uint8_t c);

void sub_ref(Image &src, Image &dst, uint8_t c);

void inv_sub_ref(Image &src, Image &dst, uint8_t c);

void mul_ref(Image &src, Image &dst, uint8_t c);

void div_ref(Image &src, Image &dst, uint8_t c);

void inv_div_ref(Image &src, Image &dst, uint8_t c);

static inline void add_simd(Image &src, Image &dst, uint8_t c) {
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

static inline void sub_simd(Image &src, Image &dst, uint8_t c) {
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

void inv_sub_simd(Image &src, Image &dst, uint8_t c);

void mul_simd(Image &src, Image &dst, uint8_t c);

void div_simd(Image &src, Image &dst, uint8_t c);

void inv_div_simd(Image &src, Image &dst, uint8_t c);
