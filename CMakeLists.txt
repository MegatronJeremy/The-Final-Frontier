cmake_minimum_required(VERSION 3.22)
project(TheFinalFrontier)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -march=native")

add_executable(TheFinalFrontier
        main.cpp
        Image/stb_image/stb_image.h
        Image/stb_image/stb_image_write.h
        Utils/AlignedAllocator.h
        Image/Image.hpp Image/Image.cpp
        Predefined/Inversion.cpp Predefined/Grayscale.cpp
        Predefined/Inversion.hpp Filter/FilterRef.cpp
        Filter/Filter.hpp Arithmetic/ArithmeticRef.cpp
        Arithmetic/Arithmetic.hpp Arithmetic/ArithmeticSIMD.cpp
        SIMD_Util/SIMD_Util.h Functions/Functions.hpp
        Functions/FunctionsRef.cpp Functions/FunctionsSIMD.cpp
        SIMD_Util/avx_mathfun.h Filter/FilterSIMD.cpp
        CPU_Info/CPU_Info.h
        ImageProcessor/ImageProcessor.cpp
        ImageProcessor/ImageProcessor.hpp
        Utils/TimeDecorator.hpp)
