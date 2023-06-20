
#include "Image/Image.hpp"
#include "Utils/_Timer.h"

#include "Predefined/Inversion.hpp"
#include "Predefined/Grayscale.hpp"
#include "Filter/Filter.hpp"
#include "Arithmetic/Arithmetic.hpp"
#include "Functions/Functions.hpp"
#include "ImageProcessor/ImageProcessor.hpp"

#include <string>
#include <iostream>
#include <getopt.h>
#include <stdexcept>

int main(int argc, char **argv) {
    static struct option long_options[] = {
            {"add",    required_argument, nullptr, 'a'},
            {"sub",    required_argument, nullptr, 's'},
            {"isub",   required_argument, nullptr, 'z'},
            {"mul",    required_argument, nullptr, 'm'},
            {"div",    required_argument, nullptr, 'd'},
            {"idiv",   required_argument, nullptr, 'c'},
            {"pow",    required_argument, nullptr, 'p'},
            {"log",    no_argument,       nullptr, 'l'},
            {"abs",    no_argument,       nullptr, 'b'},
            {"min",    required_argument, nullptr, 'n'},
            {"max",    required_argument, nullptr, 'x'},
            {"inv",    no_argument,       nullptr, 'i'},
            {"gray",   no_argument,       nullptr, 'g'},
            {"filter", required_argument, nullptr, 'f'},
            {"output", required_argument, nullptr, 'o'},
            {nullptr,  0,                 nullptr, 0}
    };

    ImageProcessor imgProc;

    std::string in_file;

    int c;
    while ((c = getopt_long(argc, argv, "a:s:z:m:d:c:p:lbn:x:igf:", long_options, nullptr)) != -1) {
        switch (c) {
            case 'a':
                imgProc.addOperation(ImageProcessor::ADD);
                break;
            case 's':
                imgProc.addOperation(ImageProcessor::SUB);
                imgProc.addOperand(strtol(optarg, nullptr, 10));
                break;
            case 'z':
                imgProc.addOperation(ImageProcessor::ISUB);
                imgProc.addOperand(strtol(optarg, nullptr, 10));
                break;
            case 'm':
                imgProc.addOperation(ImageProcessor::MUL);
                imgProc.addOperand(strtol(optarg, nullptr, 10));
                break;
            case 'd':
                imgProc.addOperation(ImageProcessor::DIV);
                imgProc.addOperand(strtol(optarg, nullptr, 10));
                break;
            case 'c':
                imgProc.addOperation(ImageProcessor::IDIV);
                imgProc.addOperand(strtol(optarg, nullptr, 10));
                break;
            case 'p':
                imgProc.addOperation(ImageProcessor::POW);
                imgProc.addOperand(strtod(optarg, nullptr));
                break;
            case 'l':
                imgProc.addOperation(ImageProcessor::LOG);
                break;
            case 'b':
                imgProc.addOperation(ImageProcessor::ABS);
                break;
            case 'n':
                imgProc.addOperation(ImageProcessor::MIN);
                imgProc.addOperand(strtol(optarg, nullptr, 10));
                break;
            case 'x':
                imgProc.addOperation(ImageProcessor::MAX);
                imgProc.addOperand(strtol(optarg, nullptr, 10));
                break;
            case 'i':
                imgProc.addOperation(ImageProcessor::INV);
                break;
            case 'g':
                imgProc.addOperation(ImageProcessor::GRAY);
                break;
            case 'f':
                imgProc.addOperation(ImageProcessor::FILTER);
                break;
            default:
                std::cerr << "Invalid usage!\n" << std::endl;
                return -1;
        }
    }


    // Get the input file after parsing the options
    if (optind < argc) {
        in_file = argv[optind];
    } else {
        std::cerr << "Missing input file argument!\n";
        return -3;
    }

    try {
        imgProc.performOperations(in_file);
    } catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
        return -2;
    }


    return 0;
}
