#include "ImageProcessor.hpp"
#include "../Arithmetic/Arithmetic.hpp"
#include "../Filter/Filter.hpp"
#include "../Functions/Functions.hpp"
#include "../Utils/TimeDecorator.hpp"
#include "../Predefined/Grayscale.hpp"
#include "../Predefined/Inversion.hpp"

ImageProcessor::ImageProcessor() {
    ops = {
            [this]() { add(); },
            [this]() { sub(); },
            [this]() { isub(); },
            [this]() { mul(); },
            [this]() { div(); },
            [this]() { idiv(); },
            [this]() { pow(); },
            [this]() { log(); },
            [this]() { abs(); },
            [this]() { min(); },
            [this]() { max(); },
            [this]() { inv(); },
            [this]() { grayscale(); },
    };
}

void ImageProcessor::performOperations(const std::string &imgPath) {
    this->imgRefSrc = Image{imgPath};
    this->imgOptSrc = Image{imgPath};
    this->imgRef = Image::createCanvas(imgRefSrc);
    this->imgOpt = Image::createCanvas(imgOptSrc);

    while (!opsQueue.empty()) {
        OpEnum next = opsQueue.front();
        opsQueue.pop();
        ops[next]();
        std::swap(imgRef, imgRefSrc);
        std::swap(imgOpt, imgOptSrc);
    }

    imgRefSrc.save("ai1.jpg");
    imgOptSrc.save("ai2.jpg");
}

void ImageProcessor::add() {
    uint8_t c = charOpQueue.front();
    charOpQueue.pop();

    double timeRef = make_decorator(add_ref)(imgRefSrc, imgRef, c);
    double timeOpt = make_decorator(add_simd)(imgOptSrc, imgOpt, c);

    std::cout << "Add time shortened by " << timeRef / timeOpt << " times" << std::endl;
    std::cout << "-------------------------------------------------------" << std::endl;
}

void ImageProcessor::sub() {
    uint8_t c = charOpQueue.front();
    charOpQueue.pop();

    std::cout << "Unoptimized:\t";
    double timeRef = make_decorator(sub_ref)(imgRefSrc, imgRef, c);
    std::cout << "Optimized:\t";
    double timeOpt = make_decorator(sub_simd)(imgOptSrc, imgOpt, c);

    std::cout << "Sub time shortened by " << timeRef / timeOpt << " times" << std::endl;
    std::cout << "-------------------------------------------------------" << std::endl;
}

void ImageProcessor::isub() {

    uint8_t c = charOpQueue.front();
    charOpQueue.pop();

    double timeRef = make_decorator(sub_ref)(imgRefSrc, imgRef, c);
    double timeOpt = make_decorator(sub_simd)(imgOptSrc, imgOpt, c);

    std::cout << "Inverse sub time shortened by " << timeRef / timeOpt << " times" << std::endl;
    std::cout << "-------------------------------------------------------" << std::endl;
}

void ImageProcessor::mul() {
    uint8_t c = charOpQueue.front();
    charOpQueue.pop();

    double timeRef = make_decorator(mul_ref)(imgRefSrc, imgRef, c);
    double timeOpt = make_decorator(mul_simd)(imgOptSrc, imgOpt, c);

    std::cout << "Mul time shortened by " << timeRef / timeOpt << " times" << std::endl;
    std::cout << "-------------------------------------------------------" << std::endl;

}

void ImageProcessor::div() {

    uint8_t c = charOpQueue.front();
    charOpQueue.pop();

    double timeRef = make_decorator(div_ref)(imgRefSrc, imgRef, c);
    double timeOpt = make_decorator(div_simd)(imgOptSrc, imgOpt, c);

    std::cout << "Div time shortened by " << timeRef / timeOpt << " times" << std::endl;
    std::cout << "-------------------------------------------------------" << std::endl;
}

void ImageProcessor::idiv() {

    uint8_t c = charOpQueue.front();
    charOpQueue.pop();

    double timeRef = make_decorator(inv_div_ref)(imgRefSrc, imgRef, c);
    double timeOpt = make_decorator(inv_div_simd)(imgOptSrc, imgOpt, c);

    std::cout << "Inverse div time shortened by " << timeRef / timeOpt << " times" << std::endl;
    std::cout << "-------------------------------------------------------" << std::endl;
}

void ImageProcessor::pow() {
    double c = doubleOpQueue.front();
    doubleOpQueue.pop();

    double timeRef = make_decorator(pow_ref)(imgRefSrc, imgRef, c);
    double timeOpt = make_decorator(pow_simd)(imgOptSrc, imgOpt, c);

    std::cout << "Pow time shortened by " << timeRef / timeOpt << " times" << std::endl;
    std::cout << "-------------------------------------------------------" << std::endl;
}

void ImageProcessor::log() {
    double timeRef = make_decorator(log_ref)(imgRefSrc, imgRef);
    double timeOpt = make_decorator(log_simd)(imgOptSrc, imgOpt);

    std::cout << "Log time shortened by " << timeRef / timeOpt << " times" << std::endl;
    std::cout << "-------------------------------------------------------" << std::endl;

}

void ImageProcessor::abs() {
    double timeRef = make_decorator(abs_ref)(imgRefSrc, imgRef);
    double timeOpt = make_decorator(abs_simd)(imgOptSrc, imgOpt);

    std::cout << "Abs time shortened by " << timeRef / timeOpt << " times" << std::endl;
    std::cout << "-------------------------------------------------------" << std::endl;
}

void ImageProcessor::min() {
    uint8_t c = charOpQueue.front();
    charOpQueue.pop();

    double timeRef = make_decorator(min_ref)(imgRefSrc, imgRef, c);
    double timeOpt = make_decorator(min_simd)(imgOptSrc, imgOpt, c);

    std::cout << "Min time shortened by " << timeRef / timeOpt << " times" << std::endl;
    std::cout << "-------------------------------------------------------" << std::endl;

}

void ImageProcessor::max() {
    uint8_t c = charOpQueue.front();
    charOpQueue.pop();

    double timeRef = make_decorator(max_ref)(imgRefSrc, imgRef, c);
    double timeOpt = make_decorator(max_simd)(imgOptSrc, imgOpt, c);

    std::cout << "Max time shortened by " << timeRef / timeOpt << " times" << std::endl;
    std::cout << "-------------------------------------------------------" << std::endl;
}

void ImageProcessor::inv() {
    double timeRef = make_decorator(inversion_ref)(imgRefSrc, imgRef);
    double timeOpt = make_decorator(inversion_simd)(imgOptSrc, imgOpt);

    std::cout << "Inversion time shortened by " << timeRef / timeOpt << " times" << std::endl;
    std::cout << "-------------------------------------------------------" << std::endl;
}

void ImageProcessor::grayscale() {
    double timeRef = make_decorator(grayscale_ref)(imgRefSrc, imgRef);
    double timeOpt = make_decorator(grayscale_simd)(imgOptSrc, imgOpt);

    std::cout << "Grayscale time shortened by " << timeRef / timeOpt << " times" << std::endl;
    std::cout << "-------------------------------------------------------" << std::endl;
}

void ImageProcessor::filter() {
    double timeRef = make_decorator(gaussian_blur_ref)(imgRefSrc, imgRef);
    double timeOpt = make_decorator(gaussian_blur_mt_blocking)(imgOptSrc, imgOpt);

    std::cout << "Filter time shortened by " << timeRef / timeOpt << " times" << std::endl;
    std::cout << "-------------------------------------------------------" << std::endl;
}

