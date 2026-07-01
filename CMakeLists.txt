cmake_minimum_required(VERSION 3.20)
project(Raytracer CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SDL2 CONFIG REQUIRED)

add_executable(
    Raytracer
    src/main.cpp
    src/Scene.cpp
    src/Sphere.cpp
    src/materials/Diffuse.cpp
    src/materials/Light.cpp
    src/materials/Metal.cpp
    src/materials/Mirror.cpp
)

target_include_directories(Raytracer PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include")

target_link_libraries(Raytracer 
    PRIVATE 
    $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
    $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>
)