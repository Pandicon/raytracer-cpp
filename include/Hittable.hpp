#pragma once

#include <variant>

#include "shapes/Quadrilateral.hpp"
#include "shapes/Sphere.hpp"
#include "shapes/Triangle.hpp"

using Hittable = std::variant<Quadrilateral, Sphere, Triangle>;