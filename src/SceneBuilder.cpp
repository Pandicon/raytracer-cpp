#include <algorithm>
#include <atomic>
#include <cmath>
#include <thread>
#include <stack>
#include <iostream>

#include "BVH.hpp"
#include "Colour.hpp"
#include "shapes/Composite.hpp"
#include "Hittable.hpp"
#include "SceneBuilder.hpp"
#include "SceneObject.hpp"

#include "matching.hpp"

SceneBuilder::SceneBuilder() : SceneBuilder(1.0) {}

SceneBuilder::SceneBuilder(double void_index_of_refraction) : void_index_of_refraction_(void_index_of_refraction), rgb2spec(rgb2spec_load("srgb.coeff"))
{
    if (!rgb2spec)
    {
        throw std::runtime_error("Failed to load srgb.coeff");
    }
}

SceneBuilder::SceneBuilder(std::vector<SceneObject> objects, double void_index_of_refraction) : objects_(std::move(objects)), void_index_of_refraction_(void_index_of_refraction), rgb2spec(rgb2spec_load("srgb.coeff"))
{
    if (!rgb2spec)
    {
        throw std::runtime_error("Failed to load srgb.coeff");
    }
}

void SceneBuilder::add_object(SceneObject object)
{
    objects_.push_back(std::move(object));
}

uint32_t SceneBuilder::add_material(const Material &material)
{
    materials_.push_back(material);
    return materials_.size() - 1;
}

Scene SceneBuilder::build()
{
    std::vector<Hittable> hittable_objects;
    hittable_objects.reserve(objects_.size());
    this->flatten_objects_into(hittable_objects);

    objects_.clear();

    BVH bvh = BVH(hittable_objects);

    return Scene(std::move(hittable_objects), std::move(materials_), void_index_of_refraction_, bvh, rgb2spec);
}

void SceneBuilder::flatten_objects_into(std::vector<Hittable> &flat_list)
{
    for (auto &object : objects_)
    {
        std::visit(overloaded{[&flat_list](std::unique_ptr<Composite> &composite)
                              {
                                  composite->flatten_into(flat_list);
                              },
                              [&flat_list](auto &primitive)
                              {
                                  flat_list.push_back(std::move(primitive));
                              }},
                   object);
    }
}

SceneBuilder SceneBuilder::metal_spheres()
{
    SceneBuilder scene_builder = SceneBuilder(1.0);

    {
        Metal metal_material = Metal::silver(1.0);
        uint32_t material_id = scene_builder.add_material(metal_material);
        Sphere sphere = Sphere(Vec3(-0.8, -0.8, 7.0), 0.3, material_id);
        scene_builder.add_object(std::move(sphere));
    }
    {
        Metal metal_material = Metal::silver(0.5);
        uint32_t material_id = scene_builder.add_material(metal_material);
        Sphere sphere = Sphere(Vec3(0.0, -0.8, 7.0), 0.3, material_id);
        scene_builder.add_object(std::move(sphere));
    }
    {
        Metal metal_material = Metal::silver(0.0);
        uint32_t material_id = scene_builder.add_material(metal_material);
        Sphere sphere = Sphere(Vec3(0.8, -0.8, 7.0), 0.3, material_id);
        scene_builder.add_object(std::move(sphere));
    }
    {
        Metal metal_material = Metal::copper(1.0);
        uint32_t material_id = scene_builder.add_material(metal_material);
        Sphere sphere = Sphere(Vec3(-0.8, 0.0, 7.0), 0.3, material_id);
        scene_builder.add_object(std::move(sphere));
    }
    {
        Metal metal_material = Metal::copper(0.5);
        uint32_t material_id = scene_builder.add_material(metal_material);
        Sphere sphere = Sphere(Vec3(0.0, 0.0, 7.0), 0.3, material_id);
        scene_builder.add_object(std::move(sphere));
    }
    {
        Metal metal_material = Metal::copper(0.0);
        uint32_t material_id = scene_builder.add_material(metal_material);
        Sphere sphere = Sphere(Vec3(0.8, 0.0, 7.0), 0.3, material_id);
        scene_builder.add_object(std::move(sphere));
    }
    {
        Metal metal_material = Metal::gold(1.0);
        uint32_t material_id = scene_builder.add_material(metal_material);
        Sphere sphere = Sphere(Vec3(-0.8, 0.8, 7.0), 0.3, material_id);
        scene_builder.add_object(std::move(sphere));
    }
    {
        Metal metal_material = Metal::gold(0.5);
        uint32_t material_id = scene_builder.add_material(metal_material);
        Sphere sphere = Sphere(Vec3(0.0, 0.8, 7.0), 0.3, material_id);
        scene_builder.add_object(std::move(sphere));
    }
    {
        Metal metal_material = Metal::gold(0.0);
        uint32_t material_id = scene_builder.add_material(metal_material);
        Sphere sphere = Sphere(Vec3(0.8, 0.8, 7.0), 0.3, material_id);
        scene_builder.add_object(std::move(sphere));
    }

    std::vector<SceneObject> box_objects;
    {
        Diffuse diffuse_material = Diffuse(ColourRGB::fromRGB(0, 255, 0), scene_builder.rgb2spec);
        uint32_t material_id = scene_builder.add_material(diffuse_material);
        Quadrilateral right_wall = Quadrilateral(Vec3(2.0, -2.1, 3.9), Vec3(0.0, 0.0, 4.2), Vec3(0.0, 4.2, 0.0), material_id);
        box_objects.push_back(std::move(right_wall));
    }
    {
        Diffuse diffuse_material = Diffuse(ColourRGB::fromRGB(255, 0, 0), scene_builder.rgb2spec);
        uint32_t material_id = scene_builder.add_material(diffuse_material);
        Quadrilateral left_wall = Quadrilateral(Vec3(-2.0, -2.1, 3.9), Vec3(0.0, 4.2, 0.0), Vec3(0.0, 0.0, 4.2), material_id);
        box_objects.push_back(std::move(left_wall));
    }
    {
        Diffuse diffuse_material = Diffuse(ColourRGB::fromRGB(255, 255, 255), scene_builder.rgb2spec);
        uint32_t material_id = scene_builder.add_material(diffuse_material);
        Quadrilateral back_wall = Quadrilateral(Vec3(-2.1, -2.1, 8.0), Vec3(0.0, 4.2, 0.0), Vec3(4.2, 0.0, 0.0), material_id);
        box_objects.push_back(std::move(back_wall));
    }
    {
        Diffuse diffuse_material = Diffuse(ColourRGB::fromRGB(255, 255, 255), scene_builder.rgb2spec);
        uint32_t material_id = scene_builder.add_material(diffuse_material);
        Quadrilateral bottom_wall = Quadrilateral(Vec3(-2.1, -2.0, 3.9), Vec3(0.0, 0.0, 4.2), Vec3(4.2, 0.0, 0.0), material_id);
        scene_builder.add_object(std::move(bottom_wall));
    }
    {
        Diffuse diffuse_material = Diffuse(ColourRGB::fromRGB(255, 255, 255), scene_builder.rgb2spec);
        uint32_t material_id = scene_builder.add_material(diffuse_material);
        Quadrilateral top_wall = Quadrilateral(Vec3(-2.1, 2.0, 3.9), Vec3(4.2, 0.0, 0.0), Vec3(0.0, 0.0, 4.2), material_id);
        box_objects.push_back(std::move(top_wall));
    }
    {
        double size = 1.0;
        BlackBody black_body_material = BlackBody(5700, 2.0 / 150.0 / (size * size));
        uint32_t material_id = scene_builder.add_material(black_body_material);
        Vec3 center = Vec3(0.0, 1.999, 5.0);
        Vec3 a = Vec3(size, 0.0, 0.0);
        Vec3 b = Vec3(0.0, 0.0, size);
        Quadrilateral top_light = Quadrilateral(center - a * 0.5 + b * 0.5, a, b, material_id);
        box_objects.push_back(std::move(top_light));
    }
    auto box = std::make_unique<Composite>(std::move(box_objects));
    scene_builder.add_object(std::move(box));

    return scene_builder;
}

/// @brief A helper method to generate a scene to render a rainbow on a wall using a high-dispersion glass prism
///
/// Scene optimised by Gemini after giving it a rough setup (prism, directional light, and the box around) to make it better for the purpose of a rainbow
/// @return The scene builder containing the scene for rendering a rainbow with a glass prism
SceneBuilder SceneBuilder::prism_rainbow()
{
    SceneBuilder scene_builder = SceneBuilder(1.0);
    constexpr bool rotated = true;
    if (!rotated)
    {
        // 1. High-dispersion glass material for the prism
        Dielectric glass_material = Dielectric(ColourRGB::fromRGB(255, 255, 255), {1.73759695, 0.313747346, 1.89878101}, {0.013188707, 0.0623068142, 155.23629}, scene_builder.rgb2spec);
        uint32_t glass_id = scene_builder.add_material(glass_material);

        // 2. Horizontal equilateral triangular prism (length 2.0 along X, height ~0.866 in Y-Z)
        // Front vertical entry face facing -Z
        Quadrilateral front_face = Quadrilateral(Vec3(-1.0, -1.5, 5.0), Vec3(0.0, 1.0, 0.0), Vec3(2.0, 0.0, 0.0), glass_id);
        // Bottom-back exit face sloping down and back
        Quadrilateral bottom_back_face = Quadrilateral(Vec3(-1.0, -1.5, 5.0), Vec3(2.0, 0.0, 0.0), Vec3(0.0, 0.5, 0.866025), glass_id);
        // Top-back face sloping up and back
        Quadrilateral top_back_face = Quadrilateral(Vec3(-1.0, -0.5, 5.0), Vec3(0.0, -0.5, 0.866025), Vec3(2.0, 0.0, 0.0), glass_id);
        // Left and right triangular side caps
        Triangle left_cap = Triangle::from_vertices(Vec3(-1.0, -1.5, 5.0), Vec3(-1.0, -1.0, 5.866025), Vec3(-1.0, -0.5, 5.0), glass_id);
        Triangle right_cap = Triangle::from_vertices(Vec3(1.0, -1.5, 5.0), Vec3(1.0, -0.5, 5.0), Vec3(1.0, -1.0, 5.866025), glass_id);

        scene_builder.add_object(std::move(front_face));
        scene_builder.add_object(std::move(bottom_back_face));
        scene_builder.add_object(std::move(top_back_face));
        scene_builder.add_object(std::move(left_cap));
        scene_builder.add_object(std::move(right_cap));

        // 3. Super bright directed spotlight placed out of camera view (above ceiling, in front of box)
        // double light_size = 0.5;
        double light_width = 0.5;
        double light_height = 0.0001;
        // BlackBody black_body_material = BlackBody(5700, 0.002 / (light_size * light_size));
        BlackBody black_body_material = BlackBody(5700, 0.01 / (light_width * light_height));
        // High exponent n=300.0 creates a tight, intense spotlight cone aimed along the normal
        DirectedEmitter directed_emitter = DirectedEmitter(black_body_material, 50.0, 0.999999);
        uint32_t light_id = scene_builder.add_material(directed_emitter);

        // Positioned at (0.0, 2.464, 3.0), aiming down and back at 60 degrees into the prism center at (0.0, -1.0, 5.0)
        // Quadrilateral prism_light = Quadrilateral(Vec3(-0.25, 2.3391, 2.7835), Vec3(0.5, 0.0, 0.0), Vec3(0.0, 0.25, 0.433013), light_id);
        Vec3 light_center = Vec3(0.0, 2.064177, 2.428849);
        Vec3 u = Vec3(1.0, 0.0, 0.0) * light_width;
        Vec3 v = Vec3(0.0, 0.642788, 0.766044) * light_height;
        Quadrilateral prism_light = Quadrilateral(light_center - u * 0.5 - v * 0.5, u, v, light_id);
        scene_builder.add_object(std::move(prism_light));
    }
    else
    {
        // 1. High-dispersion glass material for the prism
        Dielectric glass_material = Dielectric(ColourRGB::fromRGB(255, 255, 255), {1.73759695, 0.313747346, 1.89878101}, {0.013188707, 0.0623068142, 155.23629}, scene_builder.rgb2spec);
        uint32_t glass_id = scene_builder.add_material(glass_material);

        // 2. Horizontal equilateral triangular prism (length 2.0 along X, height ~0.866 in Y-Z)
        // Rotated around X-axis by +10° (pivot at front face center (0.0, -1.0, 5.0)) to shift the rainbow up by ~0.5 units
        // Front vertical entry face
        Quadrilateral front_face = Quadrilateral(Vec3(-1.0, -1.492404, 5.086824), Vec3(0.0, 0.984808, -0.173648), Vec3(2.0, 0.0, 0.0), glass_id);
        // Bottom-back exit face sloping down and back
        Quadrilateral bottom_back_face = Quadrilateral(Vec3(-1.0, -1.492404, 5.086824), Vec3(2.0, 0.0, 0.0), Vec3(0.0, 0.642788, 0.766044), glass_id);
        // Top-back face sloping up and back
        Quadrilateral top_back_face = Quadrilateral(Vec3(-1.0, -0.507596, 4.913176), Vec3(0.0, -0.342020, 0.939692), Vec3(2.0, 0.0, 0.0), glass_id);
        // Left and right triangular side caps
        Triangle left_cap = Triangle::from_vertices(Vec3(-1.0, -1.492404, 5.086824), Vec3(-1.0, -0.849616, 5.852868), Vec3(-1.0, -0.507596, 4.913176), glass_id);
        Triangle right_cap = Triangle::from_vertices(Vec3(1.0, -1.492404, 5.086824), Vec3(1.0, -0.507596, 4.913176), Vec3(1.0, -0.849616, 5.852868), glass_id);

        scene_builder.add_object(std::move(front_face));
        scene_builder.add_object(std::move(bottom_back_face));
        scene_builder.add_object(std::move(top_back_face));
        scene_builder.add_object(std::move(left_cap));
        scene_builder.add_object(std::move(right_cap));

        /*// 3. Super bright directed spotlight placed out of camera view (above ceiling, in front of box)
        double light_size = 0.5;
        BlackBody black_body_material = BlackBody(5700, 0.002 / (light_size * light_size));
        // High exponent n=300.0 creates a tight, intense spotlight cone aimed along the normal
        DirectedEmitter directed_emitter = DirectedEmitter(black_body_material, 50.0, 0.98);
        uint32_t light_id = scene_builder.add_material(directed_emitter);

        // Positioned at (0.0, 2.064178, 2.428850), rotated +10° around (0.0, -1.0, 5.0) to maintain a symmetric 60° incident angle into the prism center
        Quadrilateral prism_light = Quadrilateral(Vec3(-0.25, 1.903480, 2.237338), Vec3(0.5, 0.0, 0.0), Vec3(0.0, 0.321394, 0.383022), light_id);
        scene_builder.add_object(std::move(prism_light));*/
        double light_width = 0.5;
        double light_height = 0.2;
        // BlackBody black_body_material = BlackBody(5700, 0.002 / (light_size * light_size));
        BlackBody black_body_material = BlackBody(5700, 0.002 / (light_width * light_height));
        // High exponent n=300.0 creates a tight, intense spotlight cone aimed along the normal
        DirectedEmitter directed_emitter = DirectedEmitter(black_body_material, 50.0, 0.98);
        uint32_t light_id = scene_builder.add_material(directed_emitter);

        // Positioned at (0.0, 2.464, 3.0), aiming down and back at 60 degrees into the prism center at (0.0, -1.0, 5.0)
        // Quadrilateral prism_light = Quadrilateral(Vec3(-0.25, 2.3391, 2.7835), Vec3(0.5, 0.0, 0.0), Vec3(0.0, 0.25, 0.433013), light_id);
        Vec3 light_center = Vec3(0.0, 2.064177, 2.428849);
        Vec3 u = Vec3(1.0, 0.0, 0.0) * light_width;
        Vec3 v = Vec3(0.0, 0.642788, 0.766044) * light_height;
        Quadrilateral prism_light = Quadrilateral(light_center - u * 0.5 - v * 0.5, u, v, light_id);
        scene_builder.add_object(std::move(prism_light));
    }
    std::vector<SceneObject> box_objects;
    {
        Diffuse diffuse_material = Diffuse(ColourRGB::fromRGB(0, 255, 0), scene_builder.rgb2spec);
        uint32_t material_id = scene_builder.add_material(diffuse_material);
        Quadrilateral right_wall = Quadrilateral(Vec3(2.0, -2.1, 3.9), Vec3(0.0, 0.0, 4.2), Vec3(0.0, 4.2, 0.0), material_id);
        box_objects.push_back(std::move(right_wall));
    }
    {
        Diffuse diffuse_material = Diffuse(ColourRGB::fromRGB(255, 0, 0), scene_builder.rgb2spec);
        uint32_t material_id = scene_builder.add_material(diffuse_material);
        Quadrilateral left_wall = Quadrilateral(Vec3(-2.0, -2.1, 3.9), Vec3(0.0, 4.2, 0.0), Vec3(0.0, 0.0, 4.2), material_id);
        box_objects.push_back(std::move(left_wall));
    }
    {
        Diffuse diffuse_material = Diffuse(ColourRGB::fromRGB(255, 255, 255), scene_builder.rgb2spec);
        uint32_t material_id = scene_builder.add_material(diffuse_material);
        Quadrilateral back_wall = Quadrilateral(Vec3(-2.1, -2.1, 8.0), Vec3(0.0, 4.2, 0.0), Vec3(4.2, 0.0, 0.0), material_id);
        box_objects.push_back(std::move(back_wall));
    }
    {
        Diffuse diffuse_material = Diffuse(ColourRGB::fromRGB(255, 255, 255), scene_builder.rgb2spec);
        uint32_t material_id = scene_builder.add_material(diffuse_material);
        Quadrilateral bottom_wall = Quadrilateral(Vec3(-2.1, -2.0, 3.9), Vec3(0.0, 0.0, 4.2), Vec3(4.2, 0.0, 0.0), material_id);
        scene_builder.add_object(std::move(bottom_wall));
    }
    {
        Diffuse diffuse_material = Diffuse(ColourRGB::fromRGB(255, 255, 255), scene_builder.rgb2spec);
        uint32_t material_id = scene_builder.add_material(diffuse_material);
        Quadrilateral top_wall = Quadrilateral(Vec3(-2.1, 2.0, 3.9), Vec3(4.2, 0.0, 0.0), Vec3(0.0, 0.0, 4.2), material_id);
        box_objects.push_back(std::move(top_wall));
    }
    {
        /*Light light_material = Light(ColourRGB::fromRGB(255, 255, 255), 25.0 * 2.0 / 150.0, scene_builder.rgb2spec);
        uint32_t material_id = scene_builder.add_material(light_material);*/
        double size = 1.0;
        BlackBody black_body_material = BlackBody(5700, 0.25 / 150.0 / (size * size));
        // DirectedEmitter directed_emitter = DirectedEmitter(black_body_material, 100.0, 0.9848);
        uint32_t material_id = scene_builder.add_material(black_body_material);
        Vec3 center = Vec3(0.0, 1.999, 5.0);
        Vec3 a = Vec3(size, 0.0, 0.0);
        Vec3 b = Vec3(0.0, 0.0, size);
        Quadrilateral top_light = Quadrilateral(center - a * 0.5 + b * 0.5, a, b, material_id);
        box_objects.push_back(std::move(top_light));
    }
    auto box = std::make_unique<Composite>(std::move(box_objects));
    scene_builder.add_object(std::move(box));

    return scene_builder;
}