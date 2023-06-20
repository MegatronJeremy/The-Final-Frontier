#pragma once

#include "../Image/Image.hpp"

#include <vector>
#include <queue>
#include <functional>
#include <memory>


class ImageProcessor {
public:
    ImageProcessor();

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
    };

    typedef std::function<void()> Operation;

    void addOperation(OpEnum o) {
        opsQueue.push(o);
    }

    void addOperand(long op) {
        charOpQueue.push(op);
    }

    void addOperand(double op) {
        doubleOpQueue.push(op);
    }

    void performOperations(const std::string &imagePath);

private:
    void add();

    void sub();

    void isub();

    void mul();

    void div();

    void idiv();

    void pow();

    void log();

    void abs();

    void min();

    void max();

    void inv();

    void grayscale();

    void filter();

private:
    std::unique_ptr<Image> img;
    std::unique_ptr<Image> imgRef;
    std::unique_ptr<Image> imgOpt;

    std::queue<OpEnum> opsQueue;
    std::vector<Operation> ops;
    std::queue<uint8_t> charOpQueue;
    std::queue<double> doubleOpQueue;
};
