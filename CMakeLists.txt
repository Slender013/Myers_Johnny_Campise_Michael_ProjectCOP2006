cmake_minimum_required(VERSION 3.30)
project(untitled2)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include(FetchContent)
FetchContent_Declare(SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 3.0.0
        GIT_SHALLOW ON
        EXCLUDE_FROM_ALL
        SYSTEM)
FetchContent_MakeAvailable(SFML)

add_executable(untitled2 main.cpp)
target_compile_features(untitled2 PRIVATE cxx_std_17)
target_link_libraries(untitled2 PRIVATE SFML::Graphics)
