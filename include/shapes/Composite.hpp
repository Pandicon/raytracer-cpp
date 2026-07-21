#pragma once

#include <vector>

#include "Hittable.hpp"
#include "SceneObject.hpp"

class Composite
{
private:
    std::vector<SceneObject> inner_;

public:
    Composite() = default;
    Composite(std::vector<SceneObject> objects);

    void add_object(SceneObject object);
    void set_material(uint32_t material_id);

    void flatten_into(std::vector<Hittable> &flat_list);
};