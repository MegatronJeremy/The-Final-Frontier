#pragma once

#include <cstdint>

#include <immintrin.h>
#include <emmintrin.h>

#include "../Image/Image.hpp"

void grayscale_ref(Image &src, Image &dst);

void grayscale_simd(Image &src, Image &dst);

