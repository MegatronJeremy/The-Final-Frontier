#pragma once

#include <cstdint>

#include <immintrin.h>
#include <emmintrin.h>

#include "../Image/Image.hpp"


void filter_ref(Image &src, Image &dst, const double *kernel, int N);

void filter_simd(Image &src, Image &dst, const double *kernel, int N);

void sobel(Image &src, Image &dst);

void gaussian_blur_ref(Image &src, Image &dst);

void gaussian_blur_simd(Image &src, Image &dx);

void unsharp_mask_ref(Image &src, Image &dst);

void unsharp_mask_simd(Image &src, Image &dx);



