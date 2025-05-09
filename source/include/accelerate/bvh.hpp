#pragma once

#include "accelerate/aabb.hpp""
#include "shape/triangle.hpp"
#include "shape/shape.hpp"

struct BVHNode {

    void update_aabb() {
        for (auto &triangle : triangles) {
            aabb.expand(triangle.p0);
            aabb.expand(triangle.p1);
            aabb.expand(triangle.p2);
        }
    }

    AABB aabb {};
    std::vector<Triangle> triangles {};
    BVHNode *left = nullptr;
    BVHNode *right = nullptr;
};

class BVH : public Shape {
public:
    std::optional<HitInfo> intersect(const Ray &ray, float t_min, float t_max) const override;
    void build(std::vector<Triangle> &&triangles);
private:

    void recursive_split(BVHNode *node);
    void recursive_intersect(const BVHNode *node, const Ray &ray, float t_min, float t_max, std::optional<HitInfo> &closest_hit_info) const;

    BVHNode *root = nullptr;
    
};
