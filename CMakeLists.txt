cmake_minimum_required(VERSION 3.20)
project(Raytracer CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SDL2 CONFIG REQUIRED)

file(GLOB_RECURSE SOURCES "src/*.cpp")

add_executable(
    Raytracer
    src/main.cpp
    src/Sphere.cpp
    src/Scene.cpp
    src/RandomUtils.cpp
)

target_include_directories(Raytracer PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include")

target_link_libraries(Raytracer 
    PRIVATE 
    $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
    $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>
)