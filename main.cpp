#include <iostream>

#include "Image/Image.hpp"
#include "Utils/_Timer.h"

#include "ImagePredefined/Grayscale.hpp"
#include "ImagePredefined/Inversion.hpp"
#include "Filter/Filter.hpp"


int main() {
    Image img{"sky.jpg"};
    Image newImg{img};

    StartTimer(No SIMD)
        sobel(img, newImg);
    EndTimer

    newImg.save("shadow1.png");

    newImg = Image{img};

    StartTimer(Wt SIMD)
        stencil(img, newImg);
    EndTimer

    newImg.save("shadow2.png");

    return 0;
}
