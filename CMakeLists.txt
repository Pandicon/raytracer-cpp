cmake_minimum_required(VERSION 3.20)
project(Raytracer CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SDL2 CONFIG REQUIRED)
find_package(Stb REQUIRED)

add_executable(
    Raytracer
    src/AABB.cpp
    src/BVH.cpp
    src/save_to_image.cpp
    src/main.cpp
    src/Scene.cpp
    src/SceneBuilder.cpp
    src/materials/BlackBody.cpp
    src/materials/Dielectric.cpp
    src/materials/Diffuse.cpp
    src/materials/DirectedEmitter.cpp
    src/materials/Light.cpp
    src/materials/Metal.cpp
    src/materials/Mirror.cpp
    src/shapes/Composite.cpp
    src/shapes/Quadrilateral.cpp
    src/shapes/Sphere.cpp
    src/shapes/Triangle.cpp
)

target_include_directories(Raytracer PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_include_directories(Raytracer PRIVATE ${Stb_INCLUDE_DIR})

add_subdirectory(include/external/rgb2spec)
target_link_libraries(Raytracer PRIVATE rgb2spec)

target_link_libraries(Raytracer 
    PRIVATE 
    $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
    $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>
)

if(MSVC)
    target_compile_options(Raytracer PRIVATE /fp:fast /arch:AVX512) # change /arch:AVX512 to /arch:AVX2 for more compatibility at the cost of a bit of performance
endif()