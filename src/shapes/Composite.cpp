#include "shapes/Composite.hpp"
#include "Hittable.hpp"
#include "SceneObject.hpp"

#include "matching.hpp"

Composite::Composite(std::vector<SceneObject> objects) : inner_(std::move(objects)) {};

void Composite::add_object(SceneObject object)
{
    inner_.push_back(std::move(object));
}

void Composite::set_material(uint32_t material_id)
{
    for (SceneObject &obj : inner_)
    {
        std::visit(overloaded{[&](std::unique_ptr<Composite> &composite)
                              {
                                  composite->set_material(material_id);
                              },
                              [&](auto &scene_object)
                              {
                                  scene_object.set_material(material_id);
                              }},
                   obj);
    }
}

void Composite::flatten_into(std::vector<Hittable> &flat_list)
{
    for (auto &object : inner_)
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

    inner_.clear();
}