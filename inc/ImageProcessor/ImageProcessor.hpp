#pragma once

#include "../Image/Image.hpp"

#include <vector>
#include <queue>
#include <functional>
#include <memory>
#include <iostream>
#include <iomanip>
#include <unordered_map>
#include <map>


class ImageProcessor {
public:
    enum OpEnum {
        ADD = 0,
        SUB,
        ISUB,
        MUL,
        DIV,
        IDIV,
        POW,
        LOG,
        ABS,
        MIN,
        MAX,
        INV,
        GRAY,
        SOBEL,
        FILTER,
        BENCH,
    };

    enum OpType {
        NONE,
        UCHAR,
        DOUBLE,
        MATRIX,
    };

    static std::map<OpEnum, OpType> opTypeMap;

    static std::unordered_map<OpEnum, std::string> opNameMap;

    static std::unordered_map<OpEnum, std::function<void(Image &, Image &, char)>> ucharOpRefFnMap;

    static std::unordered_map<OpEnum, std::function<void(Image &, Image &, char)>> ucharOpOptFnMap;

    static std::unordered_map<OpEnum, std::function<void(Image &, Image &, double)>> doubleOpFnMap;

    static std::unordered_map<OpEnum, std::function<void(Image &, Image &, double)>> doubleOpOptFnMap;

    static std::unordered_map<OpEnum, std::function<void(Image &, Image &)>> predefOpFnMap;

    static std::unordered_map<OpEnum, std::function<void(Image &, Image &)>> predefOpOptFnMap;

    static std::unordered_map<OpEnum, std::function<void(Image &, Image &, const double *, int)>> matrixOpFnMap;

    static std::unordered_map<OpEnum, std::function<void(Image &, Image &, const double *, int)>> matrixOpOptFnMap;

    void addOperation(OpEnum o) {
        opsQueue.push(o);
    }

    void addOperand(long op) {
        ucharOpQueue.push(op);
    }

    void addOperand(double op) {
        doubleOpQueue.push(op);
    }

    void addMatrixPath(const std::string &path) {
        pathQueue.push(path);
    }

    void performOperations();

    void printResults() const;

    void setOutputName(const std::string &fileName);

    void setInputFile(const std::string &path);

private:
    template<typename RefOp, typename OptOp, typename... Args>
    void performOperation(const RefOp &refOp, const OptOp &optOp, const std::string &opName,
                          Args &&...args) {
        time_t timeRef = make_time_decorator(refOp)(*imgRefSrc, *imgRef, std::forward<Args>(args)...);
        time_t timeOpt = make_time_decorator(optOp)(*imgOptSrc, *imgOpt, std::forward<Args>(args)...);

        std::cout << "Unoptimized\t" <<
                  std::right << std::setw(36) << timeRef << " ns" << std::endl;
        std::cout << "Optimized\t"
                  << std::right << std::setw(36) << timeOpt << " ns" << std::endl;

        // total width is 49
        std::cout << std::left << std::setw(34) << opName + " time shortened"
                  << std::right << std::setw(15) << std::fixed << std::setprecision(2)
                  << static_cast<double>(timeRef) / static_cast<double>(timeOpt)
                  << " times" << std::endl;
        std::cout << "-------------------------------------------------------" << std::endl;

        totalRefTime += timeRef;
        totalOptTime += timeOpt;

        std::swap(imgRefSrc, imgRef);
        std::swap(imgOptSrc, imgOpt);
    }

    template<typename RefOp, typename OptOp, typename... Args>
    void performNormalizedOperation(const RefOp &refOp, const OptOp &optOp, const std::string &opName,
                                    Args &&...args) {
        const int N = 5;

        time_t timeRefAvg = 0;
        time_t timeOptAvg = 0;

        for (int i = 0; i < N; i++) {
            timeRefAvg += make_time_decorator(refOp)(*imgRefSrc, *imgRef, std::forward<Args>(args)...);
            timeOptAvg += make_time_decorator(optOp)(*imgOptSrc, *imgOpt, std::forward<Args>(args)...);
        }

        timeRefAvg /= N;
        timeOptAvg /= N;

        std::cout << "Unoptimized\t" << std::right << std::setw(36) << timeRefAvg << " ns" << std::endl;
        std::cout << "Optimized\t" << std::right << std::setw(36) << timeOptAvg << " ns" << std::endl;

        std::cout << std::left << std::setw(34) << opName + " time shortened"
                  << std::right << std::setw(15) << std::fixed << std::setprecision(2)
                  << static_cast<double>(timeRefAvg) / static_cast<double>(timeOptAvg)
                  << " times" << std::endl;
        std::cout << "-------------------------------------------------------" << std::endl;

        totalRefTime += timeRefAvg;
        totalOptTime += timeOptAvg;

        std::swap(imgRefSrc, imgRef);
        std::swap(imgOptSrc, imgOpt);
    }

    void performBenchmark();

    void saveImage();

    [[nodiscard]] std::pair<size_t, std::vector<double>> loadMatrix(const std::string &fileName) const;

    static void printTitle(const std::string &s);

    const std::string inputImgFolder = "InputImg";
    const std::string outputImgFolder = "OutputImg";
    const std::string kernelFolder = "Kernel";

    std::string imageName;

    std::string imgPath;

    time_t totalRefTime = 0;
    time_t totalOptTime = 0;

    std::unique_ptr<Image> imgRefSrc;
    std::unique_ptr<Image> imgOptSrc;
    std::unique_ptr<Image> imgRef;
    std::unique_ptr<Image> imgOpt;

    std::queue<OpEnum> opsQueue;
    std::queue<uint8_t> ucharOpQueue;
    std::queue<double> doubleOpQueue;
    std::queue<std::string> pathQueue;

};
