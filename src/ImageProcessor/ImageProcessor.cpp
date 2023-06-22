#include "../../inc/ImageProcessor/ImageProcessor.hpp"
#include "../../inc/Arithmetic/Arithmetic.hpp"
#include "../../inc/Filter/Filter.hpp"
#include "../../inc/Functions/Functions.hpp"
#include "../../inc/Utils/TimeDecorator.hpp"
#include "../../inc/Predefined/Grayscale.hpp"
#include "../../inc/Predefined/Inversion.hpp"

#include <filesystem>
#include <fstream>

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
        {SOBEL,  NONE},
        {FILTER, MATRIX},
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
        {SOBEL,  "SOBEL FILTER"},
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
        {LOG,   log_ref},
        {ABS,   abs_ref},
        {INV,   inversion_ref},
        {GRAY,  grayscale_ref},
        {SOBEL, sobel_ref},
};

std::unordered_map<ImageProcessor::OpEnum, std::function<void(Image &, Image &)>> ImageProcessor::predefOpOptFnMap{
        {LOG,   log_simd},
        {ABS,   abs_simd},
        {INV,   inversion_simd},
        {GRAY,  grayscale_simd},
        {SOBEL, sobel_mt_blocking},
};

std::unordered_map<ImageProcessor::OpEnum, std::function<void(Image &, Image &,
                                                              const double *, int)>> ImageProcessor::matrixOpFnMap{
        {FILTER, filter_ref}
};

std::unordered_map<ImageProcessor::OpEnum, std::function<void(Image &, Image &,
                                                              const double *, int)>> ImageProcessor::matrixOpOptFnMap{
        {FILTER, filter_mt_blocking}
};


void ImageProcessor::performOperations() {
    std::cout << "-------------------------------------------------------" << std::endl;
    printTitle("Performing operations");
    std::cout << "-------------------------------------------------------" << std::endl;

    while (!opsQueue.empty()) {
        OpEnum fnType = opsQueue.front();
        opsQueue.pop();

        if (fnType == BENCH) {
            performBenchmark();
            return;
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

                performOperation(doubleOpFnMap[fnType], doubleOpOptFnMap[fnType], opNameMap[fnType], c);
            }
                break;
            case MATRIX: {
                std::string path = pathQueue.front();
                pathQueue.pop();

                auto [N, matrix] = loadMatrix(path);

                performOperation(matrixOpFnMap[fnType], matrixOpOptFnMap[fnType], opNameMap[fnType], matrix.data(), N);
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
    saveImage();
}

void ImageProcessor::performBenchmark() {
    std::cout << std::endl;
    std::cout << "-------------------------------------------------------" << std::endl;
    printTitle("Performing benchmark");
    std::cout << "-------------------------------------------------------" << std::endl;

    for (const auto &[fnType, opType]: opTypeMap) {
        switch (opType) {
            case UCHAR:
                performNormalizedOperation(ucharOpRefFnMap[fnType], ucharOpOptFnMap[fnType], opNameMap[fnType], 100);
                break;
            case DOUBLE:
                performNormalizedOperation(doubleOpFnMap[fnType], doubleOpOptFnMap[fnType], opNameMap[fnType], 1.1);
                break;
            case MATRIX:
                performNormalizedOperation(gaussian_blur_ref, gaussian_blur_mt_blocking, opNameMap[fnType]);
                break;
            case NONE:
                performNormalizedOperation(predefOpFnMap[fnType], predefOpOptFnMap[fnType], opNameMap[fnType]);
                break;
            default:
                throw std::runtime_error("Operation not recognized");
        }
    }
    printTitle("Benchmark finished");
    std::cout << "-------------------------------------------------------" << std::endl;

    printResults();
}

void ImageProcessor::printResults() const {
    std::chrono::nanoseconds totalRefTimeNs(totalRefTime);
    std::chrono::nanoseconds totalOptTimeNs(totalOptTime);

    auto totalRefSec = std::chrono::duration_cast<std::chrono::seconds>(totalRefTimeNs);
    auto totalRefMs = std::chrono::duration_cast<std::chrono::milliseconds>(totalRefTimeNs - totalRefSec);
    auto totalRefUs = std::chrono::duration_cast<std::chrono::microseconds>(totalRefTimeNs - totalRefSec - totalRefMs);

    auto totalOptSec = std::chrono::duration_cast<std::chrono::seconds>(totalOptTimeNs);
    auto totalOptMs = std::chrono::duration_cast<std::chrono::milliseconds>(totalOptTimeNs - totalOptSec);
    auto totalOptUs = std::chrono::duration_cast<std::chrono::microseconds>(totalOptTimeNs - totalOptSec - totalOptMs);

    std::cout << std::endl;
    std::cout << "-------------------------------------------------------" << std::endl;
    printTitle("Results");
    std::cout << "-------------------------------------------------------" << std::endl;

    std::cout << std::left << std::setw(30) << "Unoptimized";
    std::cout << std::right << std::setw(5) << totalRefSec.count() << " s ";
    std::cout << std::right << std::setw(5) << totalRefMs.count() << " ms ";
    std::cout << std::right << std::setw(5) << totalRefUs.count() << " us" << std::endl;

    std::cout << std::left << std::setw(30) << "Optimized";
    std::cout << std::right << std::setw(5) << totalOptSec.count() << " s ";
    std::cout << std::right << std::setw(5) << totalOptMs.count() << " ms ";
    std::cout << std::right << std::setw(5) << totalOptUs.count() << " us" << std::endl;

    std::cout << std::left << std::setw(34) << "Total time shortened"
              << std::right << std::setw(15) << std::fixed << std::setprecision(2)
              << static_cast<double>(totalRefTime) / static_cast<double>(totalOptTime)
              << " times" << std::endl;

    std::cout << "-------------------------------------------------------" << std::endl;
}

void ImageProcessor::setOutputName(const std::string &fileName) {
    imageName = fileName;
}

void ImageProcessor::saveImage() {
    if (imageName.empty()) {
        imageName = std::filesystem::path(
                imgPath).stem().string(); // Extract the image name without extension
    }

    std::filesystem::path outputFolderPath = outputImgFolder;
    std::filesystem::create_directory(outputFolderPath); // Create the folder if it doesn't exist

    std::string imageExtension = std::filesystem::path(imgPath).extension().string(); // Extract the image extension
    imgRefSrc->save(outputImgFolder + "/" + imageName + "_ref" + imageExtension);
    imgOptSrc->save(outputImgFolder + "/" + imageName + "_opt" + imageExtension);
}

void ImageProcessor::setInputFile(const std::string &path) {
    imgPath = inputImgFolder + "/" + path;

    imgRefSrc = std::make_unique<Image>(imgPath);
    imgOptSrc = std::make_unique<Image>(imgPath);
    imgRef = std::make_unique<Image>(Image::createCanvas(*imgRefSrc));
    imgOpt = std::make_unique<Image>(Image::createCanvas(*imgOptSrc));
}

std::pair<size_t, std::vector<double>> ImageProcessor::loadMatrix(const std::string &fileName) const {
    std::ifstream file(kernelFolder + "/" + fileName);
    std::string line;

    std::vector<double> matrix;
    size_t N;

    if (!file) {
        throw std::runtime_error("Failed to open file: ");
    }

    if (std::getline(file, line)) {
        std::istringstream iss(line);
        iss >> N;
    } else {
        throw std::runtime_error("Invalid N in input matrix");
    }

    size_t i = N * N;
    while (i != 0) {
        if (!std::getline(file, line)) {
            throw std::runtime_error("Invalid input matrix size");
        }

        std::istringstream iss(line);
        double value;

        while (i != 0 && iss >> value) {
            matrix.push_back(value);
            i--;
        }
    }

    return {N, matrix};
}

void ImageProcessor::printTitle(const std::string &title) {
    size_t titleWidth = 54;  // Width of the line
    size_t titleLength = title.length();
    int indent = static_cast<int>(titleWidth - titleLength) / 2;

    std::cout << std::setw(indent) << "" << title << std::endl;
}
