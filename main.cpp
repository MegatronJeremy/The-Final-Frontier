#include <iostream>

#include "Image/Image.hpp"
#include "Utils/_Timer.h"

#include "Predefined/Inversion.hpp"
#include "Predefined/Grayscale.hpp"
#include "Filter/Filter.hpp"
#include "Arithmetic/Arithmetic.hpp"
#include "Functions/Functions.hpp"


int main() {
    Image img{"ai.jpg"};
    Image newImg1 = Image::createEmpty(img);
    Image newImg2 = Image::createEmpty(img);

    StartTimer(No SIMD)
        box_blur_ref(img, newImg1);
    EndTimer

    StartTimer(Wt SIMD)
        box_blur_simd(img, newImg2);
    EndTimer

    newImg1.save("ai1.jpg");
    newImg2.save("ai2.jpg");

    return 0;
}
