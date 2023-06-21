
#include "Image/Image.hpp"

#include "Predefined/Inversion.hpp"
#include "Predefined/Grayscale.hpp"
#include "Filter/Filter.hpp"
#include "Arithmetic/Arithmetic.hpp"
#include "Functions/Functions.hpp"
#include "ImageProcessor/ImageProcessor.hpp"

#include <string>
#include <iostream>
#include <getopt.h>

void printUsage() {
    std::cerr << "Invalid usage! Please use the following command line options:\n";
    std::cerr << "Usage: <program_name> [options] <input_file>\n";
    std::cerr << "Options:\n";
    std::cerr << "  -a, --add <value>          : Perform addition operation with the specified value\n";
    std::cerr << "  -s, --sub <value>          : Perform subtraction operation with the specified value\n";
    std::cerr << "  -z, --isub <value>         : Perform inverse subtraction operation with the specified value\n";
    std::cerr << "  -m, --mul <value>          : Perform multiplication operation with the specified value\n";
    std::cerr << "  -d, --div <value>          : Perform division operation with the specified value\n";
    std::cerr << "  -c, --idiv <value>         : Perform inverse division operation with the specified value\n";
    std::cerr << "  -p, --pow <value>          : Perform power operation with the specified value\n";
    std::cerr << "  -l, --log                  : Perform logarithm operation\n";
    std::cerr << "  -b, --abs                  : Perform absolute value operation\n";
    std::cerr << "  -n, --min <value>          : Perform minimum operation with the specified value\n";
    std::cerr << "  -x, --max <value>          : Perform maximum operation with the specified value\n";
    std::cerr << "  -i, --inv                  : Perform inversion operation\n";
    std::cerr << "  -g, --gray                 : Perform grayscale conversion\n";
    std::cerr << "  -f, --filter <input_file>  : Perform filter operation with the specified matrix file\n";
    std::cerr << "  -k, --benchmark <value>    : Perform benchmark\n";
    std::cerr << "  -o, --output <output_file> : Specify the output file name\n";
}

int main(int argc, char **argv) {
    static struct option long_options[] = {
            {"add",       required_argument, nullptr, 'a'},
            {"sub",       required_argument, nullptr, 's'},
            {"isub",      required_argument, nullptr, 'z'},
            {"mul",       required_argument, nullptr, 'm'},
            {"div",       required_argument, nullptr, 'd'},
            {"idiv",      required_argument, nullptr, 'c'},
            {"pow",       required_argument, nullptr, 'p'},
            {"log",       no_argument,       nullptr, 'l'},
            {"abs",       no_argument,       nullptr, 'b'},
            {"min",       required_argument, nullptr, 'n'},
            {"max",       required_argument, nullptr, 'x'},
            {"inv",       no_argument,       nullptr, 'i'},
            {"gray",      no_argument,       nullptr, 'g'},
            {"filter",    required_argument, nullptr, 'f'},
            {"benchmark", required_argument, nullptr, 'k'},
            {"output",    required_argument, nullptr, 'o'},
            {nullptr,     0,                 nullptr, 0}
    };

    ImageProcessor imgProc;

    std::string in_file;

    int c;
    while ((c = getopt_long(argc, argv, "a:s:z:m:d:c:p:lbn:x:igf:ko:", long_options, nullptr)) != -1) {
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
            case 'k':
                imgProc.addOperation(ImageProcessor::BENCH);
                break;
            case 'o':
                imgProc.setOutputName(optarg);
                break;
            default:
                printUsage();
                return -1;
        }
    }


    // Get the input file after parsing the options
    if (optind < argc) {
        imgProc.setInputFile(argv[optind]);
    } else {
        printUsage();
        return -3;
    }

    try {
        imgProc.performOperations();
    } catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
        return -2;
    }


    return 0;
}
