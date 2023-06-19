#include "Filter.hpp"
#include "../CPU_Info/CPU_Info.h"

#include <thread>
#include <algorithm>
#include <iostream>


void filter_simd(Image &src, Image &dst, const double *kernel, int N) {
    // number of hardware threads
    uint32_t num_threads = std::thread::hardware_concurrency();
    std::vector<std::thread> threads(num_threads);

    int block_row_size = static_cast<int>((src.height - 1) / num_threads + 1);

    int block_size = static_cast<int>(get_cache_line_size(L1_CACHE_ID) / sizeof(uint8_t));

    for (int thread_id = 0; thread_id < num_threads; ++thread_id) {
        threads[thread_id] = std::thread([&, thread_id]() {

            int block_row_start = std::max(thread_id * block_row_size, 1); // to start indexing from one
            int block_row_end = std::min(src.height - 1, (thread_id + 1) * block_row_size);
            for (int rowi = block_row_start; rowi < block_row_end; rowi += block_size) {
                for (int coli = 1; coli < src.width - 1; coli += block_size) {

                    int inner_row_end = std::min(block_row_end, rowi + block_size);
                    int inner_col_end = std::min(src.width - 1, coli + block_size);
                    for (int row = rowi; row < inner_row_end; row++) {
                        for (int col = coli; col < inner_col_end; col++) {
                            double r = 0;
                            double g = 0;
                            double b = 0;

                            for (int i = 0; i < N; ++i) {
                                for (int j = 0; j < N; ++j) {
                                    size_t image_row = (row + i - 1);
                                    size_t image_column = (col + j - 1);

                                    size_t index = image_row * src.width + image_column;

                                    int kernel_index = i * N + j;

                                    r += src.R[index] * kernel[kernel_index];
                                    g += src.G[index] * kernel[kernel_index];
                                    b += src.B[index] * kernel[kernel_index];
                                }
                            }

                            size_t index = row * src.width + col;

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
        });
    }

    std::for_each(threads.begin(), threads.end(), [](std::thread &t) {
        t.join();
    });
}

void box_blur_simd(Image &src, Image &dst) {
    double GX[] = {1. / 9, 1. / 9, 1. / 9,
                   1. / 9, 1. / 9, 1. / 9,
                   1. / 9, 1. / 9, 1. / 9};
    filter_simd(src, dst, GX, 3);
}

