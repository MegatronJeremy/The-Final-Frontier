#pragma once

#include "../Image/Image.hpp"
#include <iostream>

void add_ref(Image &src, Image &dst, uint8_t c);

void sub_ref(Image &src, Image &dst, uint8_t c);

void inv_sub_ref(Image &src, Image &dst, uint8_t c);

void mul_ref(Image &src, Image &dst, uint8_t c);

void div_ref(Image &src, Image &dst, uint8_t c);

void inv_div_ref(Image &src, Image &dst, uint8_t c);

void add_simd(Image &src, Image &dst, uint8_t c);

void sub_simd(Image &src, Image &dst, uint8_t c);

void inv_sub_simd(Image &src, Image &dst, uint8_t c);

void mul_simd(Image &src, Image &dst, uint8_t c);

void div_simd(Image &src, Image &dst, uint8_t c);

void inv_div_simd(Image &src, Image &dst, uint8_t c);
