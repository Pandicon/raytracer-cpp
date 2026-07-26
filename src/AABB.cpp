#include "AABB.hpp"

AABB::AABB(Interval x_interval, Interval y_interval, Interval z_interval) : x_interval_(x_interval), y_interval_(y_interval), z_interval_(z_interval) {};

AABB AABB::extend_to_include(const Vec3 &position) const
{
    return AABB(x_interval_.extend_to_include(position.x), y_interval_.extend_to_include(position.y), z_interval_.extend_to_include(position.z));
}

AABB AABB::union_with(const AABB &other) const
{
    return AABB(x_interval_.union_with(other.x_interval_), y_interval_.union_with(other.y_interval_), z_interval_.union_with(other.z_interval_));
}

bool AABB::intersects_ray(const Ray &ray) const
{
    Interval x_intersection_interval = x_interval_.ray_intersect(ray.origin.x, ray.direction.x);
    Interval y_intersection_interval = y_interval_.ray_intersect(ray.origin.y, ray.direction.y);
    Interval z_intersection_interval = z_interval_.ray_intersect(ray.origin.z, ray.direction.z);

    Interval t_overlaps = ray.valid_range.intersect_with(x_intersection_interval).intersect_with(y_intersection_interval).intersect_with(z_intersection_interval);
    return !t_overlaps.is_empty();
}

CartesianAxis AABB::longest_axis() const
{
    if (x_interval_.length() > y_interval_.length())
    {
        if (z_interval_.length() > x_interval_.length())
        {
            return CartesianAxis::Z;
        }
        else
        {
            return CartesianAxis::X;
        }
    }
    else
    {
        if (z_interval_.length() > y_interval_.length())
        {
            return CartesianAxis::Z;
        }
        else
        {
            return CartesianAxis::Y;
        }
    }
}