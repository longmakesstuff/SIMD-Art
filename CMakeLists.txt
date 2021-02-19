cmake_minimum_required(VERSION 3.0)

# Project's specific flags
project(SIMDART)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -O3 -g -march=native -fopenmp -std=c++20 -mavx -march=native")

# Adding CMake modules
set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${CMAKE_SOURCE_DIR}/cmake_modules/")

# Finding dependencies
find_package(SFML 2.5 REQUIRED audio graphics window system)
find_package(spdlog 1.8 REQUIRED)
find_package(Eigen3 3.3 REQUIRED NO_MODULE)
find_package(fmt 6.1 REQUIRED)
find_package(TGUI 0.9.0 REQUIRED)
set(LIBRARY sfml-graphics sfml-window sfml-system Eigen3::Eigen spdlog fmt tgui ${PROJECT_NAME})

# Add directories
add_subdirectory(src)
target_include_directories(${PROJECT_NAME} PUBLIC include)
add_subdirectory(bin)
add_subdirectory(tests)

# Copy running time files
file(GLOB resource_files "${CMAKE_CURRENT_SOURCE_DIR}/data/*")
foreach (item ${resource_files})
    FILE(COPY ${item} DESTINATION "${CMAKE_BINARY_DIR}/bin")
endforeach ()
