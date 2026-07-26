#pragma once

#include <vector>

#include "AABB.hpp"
#include "Hittable.hpp"
#include "Ray.hpp"

struct BVHNode
{
    AABB bounding_box;
    uint32_t left_child;
    uint32_t primitives_count;

    bool is_leaf() const;

    BVHNode() = default;
    BVHNode(uint32_t left_child_, uint32_t primitives_count_);
};

class BVH
{
private:
    std::vector<BVHNode> nodes_;

    static void split_node(uint32_t node_index, std::vector<Hittable> &hittables, std::vector<BVHNode> &nodes);

public:
    BVH(std::vector<Hittable> &hittables);

    std::optional<HitRecord> intersect_with_ray(Ray &ray, const std::vector<Hittable> &hittables) const;
};