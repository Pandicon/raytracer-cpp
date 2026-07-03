#pragma once

#include <optional>
#include <variant>

#include "Colour.hpp"
#include "Ray.hpp"
#include "Vec3.hpp"

#include "Dielectric.hpp"
#include "Diffuse.hpp"
#include "Light.hpp"
#include "Metal.hpp"
#include "Mirror.hpp"

using Material = std::variant<Dielectric, Diffuse, Light, Metal, Mirror>;