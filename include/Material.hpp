#pragma once

#include <optional>
#include <variant>

#include "Colour.hpp"
#include "Ray.hpp"
#include "Vec3.hpp"

#include "materials/Dielectric.hpp"
#include "materials/Diffuse.hpp"
#include "materials/Light.hpp"
#include "materials/Metal.hpp"
#include "materials/Mirror.hpp"

using Material = std::variant<Dielectric, Diffuse, Light, Metal, Mirror>;