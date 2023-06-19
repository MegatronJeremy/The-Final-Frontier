#include "Filter.hpp"

void filter_simd(Image &src, Image &dst, const double *kernel, int N) {
    int block_size = 64 / sizeof(uint8_t);
    for (int block_row = 1; block_row < (src.height - 1); block_row += block_size) {
        for (int block_col = 1; block_col < (src.width - 1); block_col += block_size) {
            // process each block independently
            for (int row = block_row; row < std::min(src.height - 1, block_row + block_size); ++row) {
                for (int column = block_col; column < std::min(src.width - 1, block_col + block_size); ++column) {
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
    }
}

void box_blur_simd(Image &src, Image &dst) {
    double GX[] = {1. / 9, 1. / 9, 1. / 9,
                   1. / 9, 1. / 9, 1. / 9,
                   1. / 9, 1. / 9, 1. / 9};
    filter_simd(src, dst, GX, 3);
}

