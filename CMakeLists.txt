cmake_minimum_required(VERSION 3.22)
project(TheFinalFrontier)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS "${CMakeLists.txt} -march=native")

add_executable(TheFinalFrontier
        main.cpp
        Image/stb_image/stb_image.h
        Image/stb_image/stb_image_write.h
        Utils/_Timer.h Utils/AlignedAllocator.h Image/Image.hpp Image/Image.cpp ImagePredefined/Inversion.cpp ImagePredefined/Grayscale.cpp ImagePredefined/Inversion.hpp)
