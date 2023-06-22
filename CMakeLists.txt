cmake_minimum_required(VERSION 3.22)
project(TheFinalFrontier)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -march=native")

add_executable(TheFinalFrontier
        src/main.cpp
        inc/Image/stb_image/stb_image.h
        inc/Image/stb_image/stb_image_write.h
        inc/Utils/AlignedAllocator.h
        inc/Image/Image.hpp src/Image/Image.cpp
        src/Predefined/Inversion.cpp src/Predefined/Grayscale.cpp
        inc/Predefined/Inversion.hpp src/Filter/FilterRef.cpp
        inc/Filter/Filter.hpp src/Arithmetic/ArithmeticRef.cpp
        inc/Arithmetic/Arithmetic.hpp src/Arithmetic/ArithmeticSIMD.cpp
        inc/SIMD_Util/SIMD_Util.h inc/Functions/Functions.hpp
        src/Functions/FunctionsRef.cpp src/Functions/FunctionsSIMD.cpp
        inc/SIMD_Util/avx_mathfun.h src/Filter/FilterMtBlocking.cpp
        inc/CPU_Info/CPU_Info.h
        src/ImageProcessor/ImageProcessor.cpp
        inc/ImageProcessor/ImageProcessor.hpp
        inc/Utils/TimeDecorator.hpp)
