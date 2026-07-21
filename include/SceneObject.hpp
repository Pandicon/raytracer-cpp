#pragma once

#include <memory>
#include <variant>

#include "shapes/Quadrilateral.hpp"
#include "shapes/Sphere.hpp"
#include "shapes/Triangle.hpp"

struct Composite;

using SceneObject = std::variant<Quadrilateral, Sphere, Triangle, std::unique_ptr<Composite>>;