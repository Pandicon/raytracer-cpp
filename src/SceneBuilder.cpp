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