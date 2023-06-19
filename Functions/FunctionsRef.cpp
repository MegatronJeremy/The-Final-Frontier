#include "../Image/Image.hpp"
#include "Functions.hpp"


#include <cmath>
#include <algorithm>

void pow_ref(Image &src, Image &dst, double c) {
    double MIN = 0;
    double MAX = 255;
    for (size_t i = 0; i < src.rgbSize; i++) {
        dst.R[i] = static_cast<uint8_t>(std::clamp(pow(src.R[i], c), MIN, MAX));
        dst.G[i] = static_cast<uint8_t>(std::clamp(pow(src.G[i], c), MIN, MAX));
        dst.B[i] = static_cast<uint8_t>(std::clamp(pow(src.B[i], c), MIN, MAX));
        if (src.channels == 4) {
            dst.A[i] = src.A[i];
        }
    }
}

void log_ref(Image &src, Image &dst) {
    // c - scaling constant
    double c = 255.0 / log(1 + 255); // max value is 255
    for (size_t i = 0; i < src.rgbSize; i++) {
        dst.R[i] = static_cast<uint8_t>(c * log(src.R[i] + 1));
        dst.G[i] = static_cast<uint8_t>(c * log(src.G[i] + 1));
        dst.B[i] = static_cast<uint8_t>(c * log(src.B[i] + 1));
        if (src.channels == 4) {
            dst.A[i] = src.A[i];
        }
    }
}

void abs_ref(Image &src, Image &dst) {
    for (size_t i = 0; i < src.rgbSize; i++) {
        dst.R[i] = std::abs(static_cast<int8_t>(src.R[i]));
        dst.G[i] = std::abs(static_cast<int8_t>(src.G[i]));
        dst.B[i] = std::abs(static_cast<int8_t>(src.B[i]));
        if (src.channels == 4) {
            dst.A[i] = src.A[i];
        }
    }
}

void min_ref(Image &src, Image &dst, uint8_t c) {
    for (size_t i = 0; i < src.rgbSize; i++) {
        dst.R[i] = std::min(src.R[i], c);
        dst.G[i] = std::min(src.G[i], c);
        dst.B[i] = std::min(src.B[i], c);
        if (src.channels == 4) {
            dst.A[i] = src.A[i];
        }
    }
}

void max_ref(Image &src, Image &dst, uint8_t c) {
    for (size_t i = 0; i < src.rgbSize; i++) {
        dst.R[i] = std::max(src.R[i], c);
        dst.G[i] = std::max(src.G[i], c);
        dst.B[i] = std::max(src.B[i], c);
        if (src.channels == 4) {
            dst.A[i] = src.A[i];
        }
    }
}
