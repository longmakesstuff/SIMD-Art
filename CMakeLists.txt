cmake_minimum_required(VERSION 3.0)

project(gravitation)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_CXX_FLAGS "-Wall -Wextra -O3 -g -mavx -march=haswell")

find_package(SFML 2.5 REQUIRED audio graphics window system)
find_package(OpenMP)

if (SFML_FOUND)
    include_directories(${SFML_INCLUDE_DIR})
else ()
    message(ERROR_FATAL "Can not find SFML")
endif ()

add_executable(particle_system main.cpp Field.h Field.cpp Utils.cpp Utils.h TimeIt.cpp TimeIt.h Simd.h)

target_link_libraries(particle_system
        ${SFML_LIBRARIES}
        ${SFML_DEPENDENCIES}
        PUBLIC OpenMP::OpenMP_CXX
        sfml-graphics
        sfml-window
        sfml-system
        m)

file(GLOB resource_files "${CMAKE_CURRENT_SOURCE_DIR}/data/*")
foreach (item ${resource_files})
    FILE(COPY ${item} DESTINATION "${CMAKE_BINARY_DIR}")
endforeach ()