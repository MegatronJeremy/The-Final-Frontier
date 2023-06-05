#include <iostream>

#include "Image/Image.hpp"
#include "Utils/_Timer.h"

#include "ImagePredefined/Grayscale.hpp"
#include "ImagePredefined/Inversion.hpp"


int main() {
    Image img{"Shadow.png"};
    Image newImg{img};

    StartTimer(No SIMD)
        grayscale_struct_vectors(img.rgbSize, img.channels, img.R.data(), img.G.data(), img.B.data(),
                  img.A.data(), newImg.R.data(), newImg.G.data(), newImg.B.data(), newImg.A.data());
    EndTimer

    newImg.saveRGB("shadow1.png");

    newImg = Image{img};

    StartTimer(SIMD)
        grayscale_simd(img.rgbSize, img.channels, img.R.data(), img.G.data(), img.B.data(),
                       img.A.data(), newImg.R.data(), newImg.G.data(), newImg.B.data(), newImg.A.data());
    EndTimer

    newImg.saveRGB("shadow2.png");

    return 0;
}
