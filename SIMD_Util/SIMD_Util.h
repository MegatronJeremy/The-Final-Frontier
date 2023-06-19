#pragma once

#include <immintrin.h>

extern __m256 log256_ps(__m256 x);

extern __m256 exp256_ps(__m256 x);

inline __m128i cvtepu16_epu8(const __m256i &val) {
    const __m128i lo_lane = _mm256_castsi256_si128(val);
    const __m128i hi_lane = _mm256_extracti128_si256(val, 1);
    return _mm_packus_epi16(lo_lane, hi_lane);
}

static inline __m128i mul_epi8(const __m128i &v0, const __m128i &v1) {
    const __m256i tmp0 = _mm256_cvtepu8_epi16(v0);
    const __m256i tmp1 = _mm256_cvtepu8_epi16(v1);

    const __m256i res = _mm256_mullo_epi16(tmp0, tmp1);

    return cvtepu16_epu8(res);
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
    const __m256i res = _mm256_blend_epi16(lo_epi32, hi_epi32_shift, 0xAA);
    //
    // Return as packed 16-bit values
    //
    return cvtepu16_epu8(res);
}

static inline __m128i log_epi8(const __m128i &a, const __m256 &c) {
    //
    // Convert to 16-bit integers
    //
    const __m256i a_epi16 = _mm256_cvtepu8_epi16(a);
    //
    // Convert to two 32-bit integers
    //
    const __m256i a_hi_epi32 = _mm256_srai_epi32(a_epi16, 16);
    const __m256i a_lo_epi32_shift = _mm256_slli_epi32(a_epi16, 16);
    const __m256i a_lo_epi32 = _mm256_srai_epi32(a_lo_epi32_shift, 16);
    //
    // Convert to 32-bit floats
    //
    const __m256 a_hi = _mm256_cvtepi32_ps(a_hi_epi32);
    const __m256 a_lo = _mm256_cvtepi32_ps(a_lo_epi32);
    //
    // Perform the operation
    //
    const __m256 hi = _mm256_mul_ps(log256_ps(a_hi), c);
    const __m256 lo = _mm256_mul_ps(log256_ps(a_lo), c);
    //
    // Convert back to integers
    //
    const __m256i hi_epi32 = _mm256_cvttps_epi32(hi);
    const __m256i lo_epi32 = _mm256_cvttps_epi32(lo);
    //
    // Blend the low and the high-parts
    //
    const __m256i hi_epi32_shift = _mm256_slli_epi32(hi_epi32, 16);
    const __m256i res = _mm256_blend_epi16(lo_epi32, hi_epi32_shift, 0xAA);
    //
    // Return as packed 16-bit values
    //
    return cvtepu16_epu8(res);
}

static inline __m128i pow_epi8(const __m128i &a, const __m256 &b, const __m256 &clamp, const __m256 &add) {
    // Constants
    //
    // Convert to 16-bit integers
    //
    const __m256i a_epi16 = _mm256_cvtepi8_epi16(a);
    //
    // Convert to two 32-bit integers
    //
    const __m256i a_hi_epi32 = _mm256_srai_epi32(a_epi16, 16);
    const __m256i a_lo_epi32_shift = _mm256_slli_epi32(a_epi16, 16);
    const __m256i a_lo_epi32 = _mm256_srai_epi32(a_lo_epi32_shift, 16);
    //
    // Convert to 32-bit floats
    //
    const __m256 a_hi = _mm256_cvtepi32_ps(a_hi_epi32);
    const __m256 a_lo = _mm256_cvtepi32_ps(a_lo_epi32);
    //
    // Perform the operation
    //
    // pow(x, y) = exp(log(x)*y) with saturation
    __m256 hi = log256_ps(_mm256_add_ps(a_hi, add));
    __m256 lo = log256_ps(_mm256_add_ps(a_lo, add));
    hi = _mm256_mul_ps(hi, b);
    lo = _mm256_mul_ps(lo, b);
    hi = exp256_ps(hi);
    lo = exp256_ps(lo);
    hi = _mm256_min_ps(clamp, hi);
    lo = _mm256_min_ps(clamp, lo);
    //
    // Convert back to integers
    //
    const __m256i hi_epi32 = _mm256_cvttps_epi32(hi);
    const __m256i lo_epi32 = _mm256_cvttps_epi32(lo);
    //
    // Blend the low and the high-parts
    //
    const __m256i hi_epi32_shift = _mm256_slli_epi32(hi_epi32, 16);
    const __m256i res = _mm256_blend_epi16(lo_epi32, hi_epi32_shift, 0xAA);
    //
    // Return as packed 16-bit values
    //
    return cvtepu16_epu8(res);
}
