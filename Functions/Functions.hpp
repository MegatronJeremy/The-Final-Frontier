#pragma once

void pow_ref(Image &src, Image &dst, double c);

void log_ref(Image &src, Image &dst);

void abs_ref(Image &src, Image &dst);

void min_ref(Image &src, Image &dst, uint8_t c);

void max_ref(Image &src, Image &dst, uint8_t c);

void pow_simd(Image &src, Image &dst, double c);

void log_simd(Image &src, Image &dst);

void abs_simd(Image &src, Image &dst);

void min_simd(Image &src, Image &dst, uint8_t c);

void max_simd(Image &src, Image &dst, uint8_t c);

