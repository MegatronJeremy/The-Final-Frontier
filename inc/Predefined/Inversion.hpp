#pragma once

#include <cstdint>

#include <immintrin.h>
#include <emmintrin.h>

#include "../Image/Image.hpp"

void inversion_ref(Image &src, Image &dst);

void inversion_simd(Image &src, Image &dst);

