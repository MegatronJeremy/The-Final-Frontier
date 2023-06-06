//
// Created by xparh on 6/6/2023.
//

#include <algorithm>
#include "Arithmetic.hpp"

void add_ref(Image &src, Image &dst, uint8_t c) {
    for (size_t i = 0; i < src.rgbSize; i++) {
        dst.R[i] = src.R[i] + c;
        dst.G[i] = src.G[i] + c;
        dst.B[i] = src.B[i] + c;
        if (src.channels == 4) {
            dst.A[i] = src.A[i];
        }
    }
}

void sub_ref(Image &src, Image &dst, uint8_t c) {
    for (size_t i = 0; i < src.rgbSize; i++) {
        dst.R[i] = src.R[i] - c;
        dst.G[i] = src.G[i] - c;
        dst.B[i] = src.B[i] - c;
        if (src.channels == 4) {
            dst.A[i] = src.A[i];
        }
    }
}

void inv_sub_ref(Image &src, Image &dst, uint8_t c) {
    for (size_t i = 0; i < src.rgbSize; i++) {
        dst.R[i] = c - src.R[i];
        dst.G[i] = c - src.G[i];
        dst.B[i] = c - src.B[i];
        if (src.channels == 4) {
            dst.A[i] = src.A[i];
        }
    }
}

void mul_ref(Image &src, Image &dst, uint8_t c) {
    for (size_t i = 0; i < src.rgbSize; i++) {
        // multiply with saturation
        dst.R[i] = std::clamp(src.R[i] * c, 0, 255);
        dst.G[i] = std::clamp(src.G[i] * c, 0, 255);
        dst.B[i] = std::clamp(src.B[i] * c, 0, 255);
        if (src.channels == 4) {
            dst.A[i] = src.A[i];
        }
    }
}

void div_ref(Image &src, Image &dst, uint8_t c) {
    for (size_t i = 0; i < src.rgbSize; i++) {
        dst.R[i] = src.R[i] / c;
        dst.G[i] = src.G[i] / c;
        dst.B[i] = src.B[i] / c;
        if (src.channels == 4) {
            dst.A[i] = src.A[i];
        }
    }
}

void inv_div_ref(Image &src, Image &dst, uint8_t c) {
    for (size_t i = 0; i < src.rgbSize; i++) {
        dst.R[i] = src.R[i] != 0 ? c / src.R[i] : 255;
        dst.G[i] = src.G[i] != 0 ? c / src.G[i] : 255;
        dst.B[i] = src.B[i] != 0 ? c / src.B[i] : 255;
        if (src.channels == 4) {
            dst.A[i] = src.A[i];
        }
    }
}

