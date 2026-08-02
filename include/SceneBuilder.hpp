#pragma once

#include <memory>
#include <vector>

#include "external/rgb2spec/rgb2spec.h"

#include "Scene.hpp"
#include "SceneObject.hpp"
#include "Material.hpp"

class SceneBuilder
{
private:
    std::vector<SceneObject> objects_;
    std::vector<Material> materials_;
    double void_index_of_refraction_;

public:
    SceneBuilder();
    SceneBuilder(double void_index_of_refraction);
    SceneBuilder(std::vector<SceneObject> objects, double void_index_of_refraction);

    void add_object(SceneObject object);
    uint32_t add_material(const Material &material);

    RGB2Spec *rgb2spec;

    Scene build();

    void flatten_objects_into(std::vector<Hittable> &flat_list);

    static SceneBuilder prism_rainbow();
};