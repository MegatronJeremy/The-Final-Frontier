#pragma once

#include <cstdint>

#include <immintrin.h>
#include <emmintrin.h>

#include "../Image/Image.hpp"


void filter(Image &src, Image &dx, const double *kernel, int N);

void sobel(Image &src, Image &dst);

void box_blur(Image &src, Image &dx);

void stencil(Image &src, Image &dx);



