#pragma once
#include <optional>

#include "HitRecord.hpp"
#include "Ray.hpp"

class Hittable
{
public:
    virtual ~Hittable() = default;

    virtual std::optional<HitRecord> hit(const Ray &ray) const = 0;
};