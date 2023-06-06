#include "Image.hpp"
#include "utils.h"

#define STB_IMAGE_IMPLEMENTATION

#include "stb_image/stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "stb_image/stb_image_write.h"

Image::Image(const std::string &fname) {
    uint8_t *image;
    if ((image = stbi_load(fname.c_str(), &width, &height, &channels, 0)) != nullptr) {
        size = width * height * channels;
        rgbSize = width * height;
    } else {
        ON_ERROR_EXIT(true, "Invalid image extension");
    }

    R = AlignedVector<uint8_t>(rgbSize);
    G = AlignedVector<uint8_t>(rgbSize);
    B = AlignedVector<uint8_t>(rgbSize);
    A = AlignedVector<uint8_t>(rgbSize);

    for (size_t i = 0, j = 0; i < size; j++) {
        R[j] = image[i++];
        G[j] = image[i++];
        B[j] = image[i++];
        if (channels == 4) {
            A[j] = image[i++];
        }
    }

    // free image
    stbi_image_free(image);
}

void Image::save(const std::string &fname) const {
    auto *data = new uint8_t[size];

    for (size_t i = 0, j = 0; i < size; j++) {
        data[i++] = R[j];
        data[i++] = G[j];
        data[i++] = B[j];
        if (channels == 4) {
            data[i++] = A[j];
        }
    }
    if (str_ends_in(fname, ".jpg") || str_ends_in(fname, ".JPG") || str_ends_in(fname, ".jpeg") ||
        str_ends_in(fname, ".JPEG")) {
        stbi_write_jpg(fname.c_str(), width, height, channels, data, 100);
    } else if (str_ends_in(fname, ".png") || str_ends_in(fname, ".PNG")) {
        stbi_write_png(fname.c_str(), width, height, channels, data, width * channels);
    } else {
        ON_ERROR_EXIT(true, "Invalid image extension");
    }

    delete[] data;
}