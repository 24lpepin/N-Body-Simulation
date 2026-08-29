cmake_minimum_required(VERSION 3.20)
project(orbital_simulation)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

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

add_executable(
    orbital_simulation
    src/main.cpp
    src/geometry.cpp
    src/object.cpp
    src/simulation.cpp
    src/renderer.cpp
    src/forces/direct_force_calculator.cpp
)

target_link_libraries(
    orbital_simulation
    PRIVATE
    sfml-graphics
    sfml-window
    sfml-system
    nlohmann_json::nlohmann_json
)