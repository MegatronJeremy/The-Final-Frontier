#pragma once

#include <cstdint>

#include <immintrin.h>
#include <emmintrin.h>

#include "../Image/Image.hpp"

void inversion(size_t rgb_size, int channels,
               const uint8_t *R, const uint8_t *G, const uint8_t *B, const uint8_t *A,
               uint8_t *nR, uint8_t *nG, uint8_t *nB, uint8_t *nA);

void inversion_simd(size_t rgb_size, int channels,
                    const uint8_t *R, const uint8_t *G, const uint8_t *B, const uint8_t *A,
                    uint8_t *nR, uint8_t *nG, uint8_t *nB, uint8_t *nA);

