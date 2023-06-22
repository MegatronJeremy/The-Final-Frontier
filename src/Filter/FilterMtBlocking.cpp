#include "../../inc/Filter/Filter.hpp"
#include "../../inc/CPU_Info/CPU_Info.h"

#include <thread>
#include <algorithm>
#include <iostream>
#include <cmath>


void filter_mt_blocking(Image &src, Image &dst, const double *kernel, int N) {
    // number of hardware threads
    uint32_t num_threads = std::thread::hardware_concurrency();
    std::vector<std::thread> threads(num_threads);

    int block_row_size = static_cast<int>((src.height - 1) / num_threads + 1);

    int block_size = static_cast<int>(get_cache_line_size(L1_CACHE_ID) / sizeof(uint8_t));

    const int center = N / 2;
    const double MIN = 0;
    const double MAX = 255;

    for (int thread_id = 0; thread_id < num_threads; ++thread_id) {
        threads[thread_id] = std::thread([&, thread_id]() {
            int block_row_start = std::max(thread_id * block_row_size, center); // to start indexing from center
            int block_row_end = std::min(src.height - center, (thread_id + 1) * block_row_size);

            for (int rowi = block_row_start; rowi < block_row_end; rowi += block_size) {
                for (int coli = center; coli < src.width - center; coli += block_size) {
                    int inner_row_end = std::min(block_row_end, rowi + block_size);
                    int inner_col_end = std::min(src.width - 1, coli + block_size);

                    for (int row = rowi; row < inner_row_end; row++) {
                        for (int col = coli; col < inner_col_end; col++) {
                            double r = 0;
                            double g = 0;
                            double b = 0;

                            for (int i = 0; i < N; ++i) {
                                for (int j = 0; j < N; ++j) {
                                    size_t image_row = (row + i - center);
                                    size_t image_column = (col + j - center);

                                    size_t index = image_row * src.width + image_column;

                                    int kernel_index = i * N + j;

                                    r += src.R[index] * kernel[kernel_index];
                                    g += src.G[index] * kernel[kernel_index];
                                    b += src.B[index] * kernel[kernel_index];
                                }
                            }

                            size_t index = row * src.width + col;

                            dst.R[index] = static_cast<uint8_t>(std::clamp(r, MIN, MAX));
                            dst.G[index] = static_cast<uint8_t>(std::clamp(g, MIN, MAX));
                            dst.B[index] = static_cast<uint8_t>(std::clamp(b, MIN, MAX));
                            if (src.channels == 4) {
                                dst.A[index] = src.A[index];
                            }
                        }
                    }
                }
            }
        });
    }

    std::for_each(threads.begin(), threads.end(), [](std::thread &t) {
        t.join();
    });
}

void sobel_mt_blocking(Image &src, Image &dst) {
    int GX[] = {-1, 0, 1, -2, 0, 2, -1, 0, 1};
    int GY[] = {-1, -2, -1, 0, 0, 0, 1, 2, 1};

    int N = 3;
    const int center = N / 2;

    // number of hardware threads
    uint32_t num_threads = std::thread::hardware_concurrency();
    std::vector<std::thread> threads(num_threads);

    int block_row_size = static_cast<int>((src.height - 1) / num_threads + 1);

    int block_size = static_cast<int>(get_cache_line_size(L1_CACHE_ID) / sizeof(uint8_t));

    for (int thread_id = 0; thread_id < num_threads; ++thread_id) {
        threads[thread_id] = std::thread([&, thread_id]() {
            int block_row_start = std::max(thread_id * block_row_size, center); // to start indexing from center
            int block_row_end = std::min(src.height - center, (thread_id + 1) * block_row_size);

            for (int rowi = block_row_start; rowi < block_row_end; rowi += block_size) {
                for (int coli = center; coli < src.width - center; coli += block_size) {
                    int inner_row_end = std::min(block_row_end, rowi + block_size);
                    int inner_col_end = std::min(src.width - 1, coli + block_size);
                    
                    for (int row = rowi; row < inner_row_end; row++) {
                        for (int col = coli; col < inner_col_end; col++) {
                            double gx = 0;
                            double gy = 0;

                            for (int i = 0; i < N; ++i) {
                                for (int j = 0; j < N; ++j) {
                                    size_t image_row = (row + i - center);
                                    size_t image_column = (col + j - center);

                                    size_t index = image_row * src.width + image_column;

                                    int kernel_index = i * N + j;

                                    double average =
                                            static_cast<double>(src.R[index] + src.G[index] + src.B[index]) / 3;
                                    gx += average * GX[kernel_index];
                                    gy += average * GY[kernel_index];
                                }
                            }

                            size_t index = row * src.width + col;

                            dst.R[index] = dst.G[index] = dst.B[index] = static_cast<uint8_t>(sqrt(gx * gx + gy * gy));
                            if (src.channels == 4) {
                                dst.A[index] = src.A[index];
                            }
                        }
                    }
                }
            }
        });
    }

    std::for_each(threads.begin(), threads.end(), [](std::thread &t) {
        t.join();
    });
}

void gaussian_blur_mt_blocking(Image &src, Image &dx) {
    double kernel[] = {1. / 256, 4. / 256, 6. / 256, 4. / 256, 1. / 256,
                       4. / 256, 16. / 256, 24. / 256, 16. / 256, 4. / 256,
                       6. / 256, 24. / 256, 36. / 256, 24. / 256, 6. / 256,
                       4. / 256, 16. / 256, 24. / 256, 16. / 256, 4. / 256,
                       1. / 256, 4. / 256, 6. / 256, 4. / 256, 1. / 256
    };

    filter_mt_blocking(src, dx, kernel, 5);
}

void unsharp_mask_mt_blocking(Image &src, Image &dx) {
    double kernel[] = {-1. / 256, -4. / 256, -6. / 256, -4. / 256, -1. / 256,
                       -4. / 256, -16. / 256, -24. / 256, -16. / 256, -4. / 256,
                       -6. / 256, -24. / 256, 476. / 256, -24. / 256, -6. / 256,
                       -4. / 256, -16. / 256, -24. / 256, -16. / 256, -4. / 256,
                       -1. / 256, -4. / 256, -6. / 256, -4. / 256, -1. / 256
    };

    filter_mt_blocking(src, dx, kernel, 5);
}
