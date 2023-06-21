#include "ImageProcessor.hpp"
#include "../Arithmetic/Arithmetic.hpp"
#include "../Filter/Filter.hpp"
#include "../Functions/Functions.hpp"
#include "../Utils/TimeDecorator.hpp"
#include "../Predefined/Grayscale.hpp"
#include "../Predefined/Inversion.hpp"

std::map<ImageProcessor::OpEnum, ImageProcessor::OpType> ImageProcessor::opTypeMap = {
        {ADD,    UCHAR},
        {SUB,    UCHAR},
        {ISUB,   UCHAR},
        {MUL,    UCHAR},
        {DIV,    UCHAR},
        {IDIV,   UCHAR},
        {POW,    DOUBLE},
        {LOG,    NONE},
        {ABS,    NONE},
        {MIN,    UCHAR},
        {MAX,    UCHAR},
        {INV,    NONE},
        {GRAY,   NONE},
        {FILTER, STRING},
};

std::unordered_map<ImageProcessor::OpEnum, std::string> ImageProcessor::opNameMap = {
        {ADD,    "ADD"},
        {SUB,    "SUB"},
        {ISUB,   "INVERSE SUB"},
        {MUL,    "MUL"},
        {DIV,    "DIV"},
        {IDIV,   "INVERSE DIV"},
        {POW,    "POW"},
        {LOG,    "LOG"},
        {ABS,    "ABS"},
        {MIN,    "MIN"},
        {MAX,    "MAX"},
        {INV,    "INVERSION"},
        {GRAY,   "GRAYSCALE"},
        {FILTER, "FILTER"},
};

std::unordered_map<ImageProcessor::OpEnum, std::function<void(Image &, Image &,
                                                              char)>> ImageProcessor::ucharOpRefFnMap{
        {ADD,  add_ref},
        {SUB,  sub_ref},
        {ISUB, inv_sub_ref},
        {MUL,  mul_ref},
        {DIV,  div_ref},
        {IDIV, inv_div_ref},
        {MIN,  min_ref},
        {MAX,  max_ref}
};

std::unordered_map<ImageProcessor::OpEnum, std::function<void(Image &, Image &,
                                                              char)>> ImageProcessor::ucharOpOptFnMap{
        {ADD,  add_simd},
        {SUB,  sub_simd},
        {ISUB, inv_sub_simd},
        {MUL,  mul_simd},
        {DIV,  div_simd},
        {IDIV, inv_div_simd},
        {MIN,  min_simd},
        {MAX,  max_simd}
};

std::unordered_map<ImageProcessor::OpEnum, std::function<void(Image &, Image &,
                                                              double)>> ImageProcessor::doubleOpFnMap{
        {POW, pow_ref}
};

std::unordered_map<ImageProcessor::OpEnum, std::function<void(Image &, Image &,
                                                              double)>> ImageProcessor::doubleOpOptFnMap{
        {POW, pow_simd}
};

std::unordered_map<ImageProcessor::OpEnum, std::function<void(Image &, Image &)>> ImageProcessor::predefOpFnMap{
        {LOG,  log_ref},
        {ABS,  abs_ref},
        {INV,  inversion_ref},
        {GRAY, grayscale_ref}
};

std::unordered_map<ImageProcessor::OpEnum, std::function<void(Image &, Image &)>> ImageProcessor::predefOpOptFnMap{
        {LOG,  log_simd},
        {ABS,  abs_simd},
        {INV,  inversion_simd},
        {GRAY, grayscale_simd}
};

void ImageProcessor::performOperations(const std::string &imgPath) {
    imgRefSrc = std::make_unique<Image>(imgPath);
    imgOptSrc = std::make_unique<Image>(imgPath);
    imgRef = std::make_unique<Image>(Image::createCanvas(*imgRefSrc));
    imgOpt = std::make_unique<Image>(Image::createCanvas(*imgOptSrc));

    while (!opsQueue.empty()) {
        OpEnum fnType = opsQueue.front();
        opsQueue.pop();

        if (fnType == BENCH) {
            performBenchmark();
            continue;
        }

        switch (opTypeMap[fnType]) {
            case UCHAR: {
                uint8_t c = ucharOpQueue.front();
                ucharOpQueue.pop();

                performOperation(ucharOpRefFnMap[fnType], ucharOpOptFnMap[fnType], opNameMap[fnType], c);
            }
                break;
            case DOUBLE: {
                double c = doubleOpQueue.front();
                doubleOpQueue.pop();

                performOperation(ucharOpRefFnMap[fnType], ucharOpOptFnMap[fnType], opNameMap[fnType], c);
            }
                break;
            case STRING: {
            }
                break;
            case NONE: {
                performOperation(predefOpFnMap[fnType], predefOpOptFnMap[fnType], opNameMap[fnType]);
            }
                break;
            default:
                throw std::runtime_error("Operation not recognized");
        }
    }

    printResults();

    imgRefSrc->save("ai1.jpg");
    imgOptSrc->save("ai2.jpg");
}

void ImageProcessor::performBenchmark() {
    std::cout << "-------------------------------------------------------" << std::endl;
    std::cout << "\t\tPerforming benchmark" << std::endl;
    std::cout << "-------------------------------------------------------" << std::endl;

    for (const auto &[fnType, opType]: opTypeMap) {
        switch (opType) {
            case UCHAR:
                performOperation(ucharOpRefFnMap[fnType], ucharOpOptFnMap[fnType], opNameMap[fnType], 100);
                break;
            case DOUBLE:
                performOperation(doubleOpFnMap[fnType], doubleOpOptFnMap[fnType], opNameMap[fnType], 1.1);
                break;
            case STRING:
                performOperation(gaussian_blur_ref, gaussian_blur_mt_blocking, opNameMap[fnType]);
                break;
            case NONE:
                performOperation(predefOpFnMap[fnType], predefOpOptFnMap[fnType], opNameMap[fnType]);
                break;
            default:
                throw std::runtime_error("Operation not recognized");
        }
    }
    std::cout << "\t\tBenchmark finished" << std::endl;
    std::cout << "-------------------------------------------------------" << std::endl;
}

void ImageProcessor::printResults() const {
    std::chrono::nanoseconds totalRefTimeNs(totalRefTime);
    std::chrono::nanoseconds totalOptTimeNs(totalOptTime);

    auto totalRefSec = std::chrono::duration_cast<std::chrono::seconds>(totalRefTimeNs);
    auto totalRefMs = std::chrono::duration_cast<std::chrono::milliseconds>(totalRefTimeNs - totalRefSec);
    auto totalRefNs = std::chrono::duration_cast<std::chrono::nanoseconds>(totalRefTimeNs - totalRefSec - totalRefMs);

    auto totalOptSec = std::chrono::duration_cast<std::chrono::seconds>(totalOptTimeNs);
    auto totalOptMs = std::chrono::duration_cast<std::chrono::milliseconds>(totalOptTimeNs - totalOptSec);
    auto totalOptNs = std::chrono::duration_cast<std::chrono::nanoseconds>(totalOptTimeNs - totalOptSec - totalOptMs);

    std::cout << std::endl;
    std::cout << "-------------------------------------------------------" << std::endl;
    std::cout << "\t\tResults" << std::endl;
    std::cout << "-------------------------------------------------------" << std::endl;
    std::cout << "Unoptimized\t";
    std::cout << totalRefSec.count() << " s ";
    std::cout << totalRefMs.count() << " ms ";
    std::cout << totalRefNs.count() << " ns" << std::endl;
    std::cout << "Optimized\t";
    std::cout << totalOptSec.count() << " s ";
    std::cout << totalOptMs.count() << " ms ";
    std::cout << totalOptNs.count() << " ns" << std::endl;
    std::cout << "Total time shortened " << static_cast<double>(totalRefTime) / static_cast<double>(totalOptTime)
              << " times" << std::endl;
    std::cout << "-------------------------------------------------------" << std::endl;
}
