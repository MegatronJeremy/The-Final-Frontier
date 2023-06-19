#pragma once

#include <cstdint>

#include <immintrin.h>
#include <emmintrin.h>

#include "../Image/Image.hpp"


void filter_ref(Image &src, Image &dst, const double *kernel, int N);

void filter_simd(Image &src, Image &dst, const double *kernel, int N);

void sobel(Image &src, Image &dst);

void box_blur_ref(Image &src, Image &dx);

void box_blur_simd(Image &src, Image &dx);

void stencil(Image &src, Image &dx);

void sharp(Image &src, Image &dx);



