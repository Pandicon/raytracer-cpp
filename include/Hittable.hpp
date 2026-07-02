#pragma once

#include <variant>

#include "Quadrilateral.hpp"
#include "Sphere.hpp"

using Hittable = std::variant<Quadrilateral, Sphere>;