//
// Created by xparh on 6/5/2023.
//

#include "Filter.hpp"

#include <cmath>

void filter(Image &src, Image &dst, const double *kernel, int N) {
    for (int row = 1; row < (src.height - 1); ++row) {
        for (int column = 1; column < (src.width - 1); ++column) {
            double r = 0;
            double g = 0;
            double b = 0;

            for (int i = 0; i < N; ++i) {
                for (int j = 0; j < N; ++j) {
                    size_t image_row = (row + i - 1);
                    size_t image_column = (column + j - 1);

                    size_t index = image_row * src.width + image_column;

                    int kernel_index = i * N + j;

                    r += src.R[index] * kernel[kernel_index];
                    g += src.G[index] * kernel[kernel_index];
                    b += src.B[index] * kernel[kernel_index];
                }
            }

            size_t index = row * src.width + column;

            dst.R[index] = (uint8_t) r;
            dst.G[index] = (uint8_t) g;
            dst.B[index] = (uint8_t) b;
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

void box_blur(Image &src, Image &dst) {
    double GX[] = {1. / 9, 1. / 9, 1. / 9,
                   1. / 9, 1. / 9, 1. / 9,
                   1. / 9, 1. / 9, 1. / 9};
    filter(src, dst, GX, 3);
}

void stencil(Image &src, Image &dst) {
    double GX[] = {1, -1, 1,
                   -1, 4, -1,
                   1, -1, 1};
    filter(src, dst, GX, 3);
}
