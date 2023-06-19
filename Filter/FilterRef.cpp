//
// Created by xparh on 6/5/2023.
//

#include "Filter.hpp"

#include <cmath>
#include <algorithm>

void filter_ref(Image &src, Image &dst, const double *kernel, int N) {
    const int center = N / 2;
    const double MIN = 0;
    const double MAX = 255;

    for (int row = center; row < (src.height - center); ++row) {
        for (int column = center; column < (src.width - center); ++column) {
            double r = 0;
            double g = 0;
            double b = 0;

            for (int i = 0; i < N; ++i) {
                for (int j = 0; j < N; ++j) {
                    size_t image_row = (row + i - center);
                    size_t image_column = (column + j - center);

                    size_t index = image_row * src.width + image_column;

                    int kernel_index = i * N + j;

                    r += src.R[index] * kernel[kernel_index];
                    g += src.G[index] * kernel[kernel_index];
                    b += src.B[index] * kernel[kernel_index];
                }
            }

            size_t index = row * src.width + column;

            dst.R[index] = static_cast<uint8_t>(std::clamp(r, MIN, MAX));
            dst.G[index] = static_cast<uint8_t>(std::clamp(g, MIN, MAX));
            dst.B[index] = static_cast<uint8_t>(std::clamp(b, MIN, MAX));
            if (src.channels == 4) {
                dst.A[index] = src.A[index];
            }
        }
    }
}

void sobel(Image &src, Image &dst) {
    int GX[] = {-1, 0, 1, -2, 0, 2, -1, 0, 1};
    int GY[] = {-1, -2, -1, 0, 0, 0, 1, 2, 1};

    int N = 3;

    for (int row = 1; row < (src.height - 1); ++row) {
        for (int column = 1; column < (src.width - 1); ++column) {
            double gx = 0;
            double gy = 0;

            for (int i = 0; i < N; ++i) {
                for (int j = 0; j < N; ++j) {
                    size_t image_row = (row + i - 1);
                    size_t image_column = (column + j - 1);

                    size_t index = image_row * src.width + image_column;

                    int kernel_index = i * N + j;

                    double average = static_cast<double>(src.R[index] + src.G[index] + src.B[index]) / 3;
                    gx += average * GX[kernel_index];
                    gy += average * GY[kernel_index];
                }
            }

            size_t index = row * src.width + column;

            dst.R[index] = dst.G[index] = dst.B[index] = static_cast<uint8_t>(sqrt(gx * gx + gy * gy));
            if (src.channels == 4) {
                dst.A[index] = src.A[index];
            }
        }
    }
}

void gaussian_blur_ref(Image &src, Image &dst) {
    double kernel[] = {1. / 256, 4. / 256, 6. / 256, 4. / 256, 1. / 256,
                       4. / 256, 16. / 256, 24. / 256, 16. / 256, 4. / 256,
                       6. / 256, 24. / 256, 36. / 256, 24. / 256, 6. / 256,
                       4. / 256, 16. / 256, 24. / 256, 16. / 256, 4. / 256,
                       1. / 256, 4. / 256, 6. / 256, 4. / 256, 1. / 256
    };

    filter_ref(src, dst, kernel, 5);
}

void unsharp_mask_ref(Image &src, Image &dst) {
    double kernel[] = {-1. / 256, -4. / 256, -6. / 256, -4. / 256, -1. / 256,
                       -4. / 256, -16. / 256, -24. / 256, -16. / 256, -4. / 256,
                       -6. / 256, -24. / 256, 476. / 256, -24. / 256, -6. / 256,
                       -4. / 256, -16. / 256, -24. / 256, -16. / 256, -4. / 256,
                       -1. / 256, -4. / 256, -6. / 256, -4. / 256, -1. / 256
    };

    filter_ref(src, dst, kernel, 5);
}
