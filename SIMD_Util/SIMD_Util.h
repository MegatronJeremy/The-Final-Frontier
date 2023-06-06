#pragma once

#include <immintrin.h>

static inline __m128i mul_epi8(const __m128i &v0, const __m128i &v1) {
    const __m256i tmp0 = _mm256_cvtepu8_epi16(v0);
    const __m256i tmp1 = _mm256_cvtepu8_epi16(v1);

    const __m256i res = _mm256_mullo_epi16(tmp0, tmp1);

    const __m128i lo_lane = _mm256_castsi256_si128(res);
    const __m128i hi_lane = _mm256_extracti128_si256(res, 1);
    return _mm_packus_epi16(lo_lane, hi_lane);
}

static inline __m128i div_epi8(const __m128i &a, const __m128i &b) {
    //
    // Convert to 16-bit integers
    //
    const __m256i a_epi16 = _mm256_cvtepu8_epi16(a);
    const __m256i b_epi16 = _mm256_cvtepu8_epi16(b);
    //
    // Convert to two 32-bit integers
    //
    const __m256i a_hi_epi32 = _mm256_srai_epi32(a_epi16, 16);
    const __m256i a_lo_epi32_shift = _mm256_slli_epi32(a_epi16, 16);
    const __m256i a_lo_epi32 = _mm256_srai_epi32(a_lo_epi32_shift, 16);
    const __m256i b_hi_epi32 = _mm256_srai_epi32(b_epi16, 16);
    const __m256i b_lo_epi32_shift = _mm256_slli_epi32(b_epi16, 16);
    const __m256i b_lo_epi32 = _mm256_srai_epi32(b_lo_epi32_shift, 16);
    //
    // Convert to 32-bit floats
    //
    const __m256 a_hi = _mm256_cvtepi32_ps(a_hi_epi32);
    const __m256 a_lo = _mm256_cvtepi32_ps(a_lo_epi32);
    const __m256 b_hi = _mm256_cvtepi32_ps(b_hi_epi32);
    const __m256 b_lo = _mm256_cvtepi32_ps(b_lo_epi32);
    //
    // Perform the division
    //
    const __m256 hi = _mm256_div_ps(a_hi, b_hi);
    const __m256 lo = _mm256_div_ps(a_lo, b_lo);
    //
    // Convert back to integers
    //
    const __m256i hi_epi32 = _mm256_cvttps_epi32(hi);
    const __m256i lo_epi32 = _mm256_cvttps_epi32(lo);
    //
    // Blend the low and the high-parts
    //
    const __m256i hi_epi32_shift = _mm256_slli_epi32(hi_epi32, 16);
    //
    // Return as packed 16-bit values
    //
    const __m256i res = _mm256_blend_epi16(lo_epi32, hi_epi32_shift, 0xAA);
    const __m128i lo_lane = _mm256_castsi256_si128(res);
    const __m128i hi_lane = _mm256_extracti128_si256(res, 1);
    return _mm_packus_epi16(lo_lane, hi_lane);
}