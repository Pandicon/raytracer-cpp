#include <algorithm>

#include "BVH.hpp"

bool BVHNode::is_leaf() const { return primitives_count > 0; }

BVHNode::BVHNode(uint32_t left_child_, uint32_t primitives_count_) : left_child(left_child_), primitives_count(primitives_count_) {};

constexpr uint32_t MIN_PRIMITIVES = 200;
void BVH::split_node(uint32_t node_index, std::vector<Hittable> &hittables, std::vector<BVHNode> &nodes)
{
    AABB node_bb;
    uint32_t first = nodes[node_index].left_child;
    uint32_t count = nodes[node_index].primitives_count;

    for (uint32_t i = 0; i < count; i += 1)
    {
        const auto &hittable = hittables[first + i];
        AABB prim_bb = std::visit([](const auto &shape)
                                  { return shape.bounding_box(); }, hittable);
        node_bb = node_bb.union_with(prim_bb);
    }
    nodes[node_index].bounding_box = node_bb;

    if (nodes[node_index].primitives_count <= MIN_PRIMITIVES)
    {
        return;
    }

    CartesianAxis longest_axis = nodes[node_index].bounding_box.longest_axis();

    uint32_t middle = first + (count / 2);
    std::nth_element(
        hittables.begin() + first,
        hittables.begin() + middle,
        hittables.begin() + first + count,
        [longest_axis](const auto &hittable_a, const auto &hittable_b)
        {
            auto centroid_a = std::visit([](const auto &shape)
                                         { return shape.get_centroid(); }, hittable_a);
            auto centroid_b = std::visit([](const auto &shape)
                                         { return shape.get_centroid(); }, hittable_b);
            return centroid_a.get_axis(longest_axis) < centroid_b.get_axis(longest_axis);
        });

    uint32_t left_child_id = nodes.size();
    uint32_t right_child_id = left_child_id + 1;

    nodes.emplace_back();
    nodes.emplace_back();

    nodes[left_child_id].left_child = first;
    nodes[left_child_id].primitives_count = count / 2;

    nodes[right_child_id].left_child = middle;
    nodes[right_child_id].primitives_count = count - (count / 2);

    nodes[node_index].left_child = left_child_id;
    nodes[node_index].primitives_count = 0;

    split_node(left_child_id, hittables, nodes);
    split_node(right_child_id, hittables, nodes);
}

BVH::BVH(std::vector<Hittable> &hittables)
{
    std::vector<BVHNode> nodes;
    nodes.emplace_back(BVHNode(0, hittables.size()));
    BVH::split_node(0, hittables, nodes);

    nodes_ = nodes;
}

std::optional<HitRecord> BVH::intersect_with_ray(Ray &ray, const std::vector<Hittable> &hittables) const
{
    std::optional<HitRecord> closest_hit = {};

    uint32_t stack[64];
    uint32_t stack_ptr = 0;

    if (!nodes_[0].bounding_box.intersects_ray(ray))
    {
        // The ray misses the root node, so no need to do anything else
        return closest_hit;
    }

    stack[stack_ptr++] = 0;

    while (stack_ptr > 0)
    {
        uint32_t node_index = stack[--stack_ptr];
        const BVHNode &node = nodes_[node_index];

        if (!node.is_leaf())
        {
            // Push the children which are hit
            if (nodes_[node.left_child].bounding_box.intersects_ray(ray))
            {
                stack[stack_ptr++] = node.left_child;
            }
            if (nodes_[node.left_child + 1].bounding_box.intersects_ray(ray))
            {
                stack[stack_ptr++] = node.left_child + 1;
            }

            continue;
        }

        for (uint32_t i = 0; i < node.primitives_count; i += 1)
        {
            const auto &object = hittables[node.left_child + i];

            std::visit([&](const auto &concrete_obj)
                       {
                    if (auto result = concrete_obj.hit(ray))
                    {
                        if (ray.valid_range.contains_value(result->t))
                        {
                            ray.valid_range.max = result->t;
                            closest_hit = result;
                        }
                    } }, object);
        }
    }

    return closest_hit;
}