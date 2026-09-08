cmake_minimum_required(VERSION 3.20)

project(orbital_simulation)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

# --------------------
# Dependencies
# --------------------

FetchContent_Declare(
    SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.0
)

FetchContent_MakeAvailable(SFML)

FetchContent_Declare(
    nlohmann_json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG v3.11.2
)

FetchContent_MakeAvailable(nlohmann_json)

# --------------------
# Simulation library
# --------------------

add_library(simulation
    src/simulation.cpp
    src/object.cpp
    src/geometry.cpp

    src/forces/direct_force_calculator.cpp
    src/forces/barnes_hut_calculator.cpp
)

target_include_directories(simulation PUBLIC
    src
)

target_link_libraries(simulation
    PUBLIC
)
# --------------------
# Demo
# --------------------

add_executable(
    demo
    src/main.cpp
    src/renderer.cpp
)

target_link_libraries(
    demo
    PRIVATE
    simulation
    SFML::Graphics
    SFML::Window
    SFML::System
    nlohmann_json::nlohmann_json
)

# --------------------
# Benchmark
# --------------------

add_executable(
    benchmark
    src/benchmark.cpp
)

target_link_libraries(
    benchmark
    PRIVATE
    simulation
)