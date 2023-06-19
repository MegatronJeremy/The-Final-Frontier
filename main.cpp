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
    Image newImg1 = Image::createCanvas(img);
    Image newImg2 = Image::createCanvas(img);

    StartTimer(No SIMD)
        unsharp_mask_ref(img, newImg1);
    EndTimer

    StartTimer(Wt SIMD)
        unsharp_mask_simd(img, newImg2);
    EndTimer

    newImg1.save("ai1.jpg");
    newImg2.save("ai2.jpg");

    return 0;
}
