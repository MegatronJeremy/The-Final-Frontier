#pragma once

#include "../Image/Image.hpp"

#include <vector>
#include <queue>
#include <functional>
#include <memory>
#include <iostream>
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
        FILTER,
        BENCH,
    };

    enum OpType {
        NONE,
        UCHAR,
        DOUBLE,
        STRING,
    };

    static std::map<OpEnum, OpType> opTypeMap;

    static std::unordered_map<OpEnum, std::string> opNameMap;

    static std::unordered_map<OpEnum, std::function<void(Image &, Image &, char)>> ucharOpRefFnMap;

    static std::unordered_map<OpEnum, std::function<void(Image &, Image &, char)>> ucharOpOptFnMap;

    static std::unordered_map<OpEnum, std::function<void(Image &, Image &, double)>> doubleOpFnMap;

    static std::unordered_map<OpEnum, std::function<void(Image &, Image &, double)>> doubleOpOptFnMap;

    static std::unordered_map<OpEnum, std::function<void(Image &, Image &)>> predefOpFnMap;

    static std::unordered_map<OpEnum, std::function<void(Image &, Image &)>> predefOpOptFnMap;

    static std::unordered_map<OpEnum, std::function<void(Image &, Image &, std::string)>> strOpFnMap;

    static std::unordered_map<OpEnum, std::function<void(Image &, Image &, std::string)>> strOpOptFnMap;

    typedef std::function<void()> Operation;

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
        std::cout << "Unoptimized\t";
        time_t timeRef = make_time_decorator(refOp)(*imgRefSrc, *imgRef, std::forward<Args>(args)...);
        std::cout << "Optimized\t";
        time_t timeOpt = make_time_decorator(optOp)(*imgOptSrc, *imgOpt, std::forward<Args>(args)...);

        std::cout << opName << " time shortened " << static_cast<double>(timeRef) / timeOpt << " times" << std::endl;
        std::cout << "-------------------------------------------------------" << std::endl;

        totalRefTime += timeRef;
        totalOptTime += timeOpt;

        std::swap(imgRef, imgRefSrc);
        std::swap(imgOpt, imgOptSrc);
    }

    void performBenchmark();

    void saveImage();

    std::string imageName;

    std::string imgPath;

    time_t totalRefTime = 0;
    time_t totalOptTime = 0;

    std::unique_ptr<Image> imgRefSrc;
    std::unique_ptr<Image> imgOptSrc;
    std::unique_ptr<Image> imgRef;
    std::unique_ptr<Image> imgOpt;

    std::vector<Operation> ops;

    std::queue<OpEnum> opsQueue;
    std::queue<uint8_t> ucharOpQueue;
    std::queue<double> doubleOpQueue;
    std::queue<std::string> pathQueue;

};
