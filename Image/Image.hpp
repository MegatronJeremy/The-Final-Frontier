#pragma once


#include <cstdint>
#include <string>
#include <vector>
#include "../Utils/AlignedAllocator.h"

template<typename T, std::size_t ALIGNMENT_IN_BYTES = 64>
using AlignedVector = std::vector<T, AlignedAllocator<T, ALIGNMENT_IN_BYTES>>;

struct Image {
    Image() = default;

    explicit Image(const std::string &fname);

    void save(const std::string &fname);

    void saveRGB(const std::string &fname);

    int width{};
    int height{};
    int channels{};
    size_t size{};
    size_t rgbSize{};

    AlignedVector<uint8_t> data;

    AlignedVector<uint8_t> R;
    AlignedVector<uint8_t> G;
    AlignedVector<uint8_t> B;
    AlignedVector<uint8_t> A;
};
