# The Final Frontier
SIMD, cache, and multithreading optimizations of most common image processing procedures.

## Generate binaries

- The only prerequisite is that you have **CMake** and **MinGW** installed
- After doing this, simply run **Generate-Binaries.bat** and run the executable

## Usage

<program_name> [options] <input_file>

## Options

    -a, --add <value>: Perform addition operation with the specified value
    -s, --sub <value>: Perform subtraction operation with the specified value
    -z, --isub <value>: Perform inverse subtraction operation with the specified value
    -m, --mul <value>: Perform multiplication operation with the specified value
    -d, --div <value>: Perform division operation with the specified value
    -c, --idiv <value>: Perform inverse division operation with the specified value
    -p, --pow <value>: Perform power operation with the specified floating point value
    -l, --log: Perform logarithm operation
    -b, --abs: Perform absolute value operation
    -n, --min <value>: Perform minimum operation with the specified value
    -x, --max <value>: Perform maximum operation with the specified value
    -i, --inv: Perform inversion operation
    -g, --gray: Perform grayscale conversion
    -e, --sobel: Perform Sobel filter operation
    -f, --filter <input_file>: Perform filter operation with the specified matrix file
    -k, --benchmark <value>: Perform benchmark normalized over N given iterations
    -o, --output <output_file>: Specify the output image name
    -h, --help: Print usage

## Notes

- The image input file needs to be in the folder InputImg
- The output image files will be placed in the OutputImg folder, with two copies, one generated by the referential and one generated by the optimized functions
- The input matrix file needs to be placed in the Kernel folder (using the same format as the examples)
- Replace <program_name> with the actual name of the program you are using.
- Replace <input_file> with the path to the input file you want to process.
- Replace <value> with the desired numerical value for the corresponding operation.
- Replace <output_file> with the desired name for the output image.

## Examples

### Addition operation:

    ./TheFinalFrontier.exe -a 5 boreal8k.jpg

### Grayscale conversion:

    ./TheFinalFrontier.exe -g boreal8k.jpg

### Sobel filter and logarithm performed one after another:

    ./TheFinalFrontier.exe -e -l boreal8k.jpg

### Perform benchmark normalized over 10 iterations:

    ./TheFinalFrontier.exe -k 10 boreal8k.jpg

### Perform filter operation with a matrix file:

    ./TheFinalFrontier.exe -f matrix.txt boreal8k.jpg

### Help:

    ./TheFinalFrontier.exe -h

