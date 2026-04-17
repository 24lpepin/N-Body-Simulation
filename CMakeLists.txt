cmake_minimum_required(VERSION 3.20)
project(orbital_simulation)

include(FetchContent)
FetchContent_Declare(
    SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.0
)
FetchContent_MakeAvailable(SFML)

add_executable(orbital_simulation src/main.cpp src/geometry.cpp src/object.cpp)
target_link_libraries(orbital_simulation PRIVATE sfml-graphics sfml-window sfml-system)