#include "ImageProcessor.hpp"
#include "../Arithmetic/Arithmetic.hpp"
#include "../Filter/Filter.hpp"
#include "../Functions/Functions.hpp"
#include "../Utils/_Timer.h"

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
    this->img = std::make_unique<Image>(imgPath);
    this->imgRef = std::make_unique<Image>(Image::createCanvas(*img));
    this->imgOpt = std::make_unique<Image>(Image::createCanvas(*img));

    while (!opsQueue.empty()) {
        OpEnum next = opsQueue.front();
        opsQueue.pop();
        ops[next]();
    }
}

void ImageProcessor::add() {
}

void ImageProcessor::sub() {

}

void ImageProcessor::isub() {

}

void ImageProcessor::mul() {

}

void ImageProcessor::div() {

}

void ImageProcessor::idiv() {

}

void ImageProcessor::pow() {

}

void ImageProcessor::log() {

}

void ImageProcessor::abs() {

}

void ImageProcessor::min() {

}

void ImageProcessor::max() {

}

void ImageProcessor::inv() {

}

void ImageProcessor::grayscale() {

}

void ImageProcessor::filter() {

}

