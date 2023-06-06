#include <iostream>

#include "Image/Image.hpp"
#include "Utils/_Timer.h"

#include "Predefined/Inversion.hpp"
#include "Predefined/Grayscale.hpp"
#include "Filter/Filter.hpp"
#include "Arithmetic/Arithmetic.hpp"


int main() {
    Image img{"sky.jpg"};
    Image newImg{img};

    StartTimer(No SIMD)
        div_ref(img, newImg, 5);
    EndTimer

    newImg.save("shadow1.jpg");

    newImg = Image{img};

    StartTimer(Wt SIMD)
        div_simd(img, newImg, 5);
    EndTimer

    newImg.save("shadow2.jpg");

    return 0;
}
