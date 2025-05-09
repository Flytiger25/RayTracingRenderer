#pragma once

#include <filesystem>
#include "triangle.hpp"
#include "accelerate/aabb.hpp"
#include "accelerate/bvh.hpp"

class Model : public Shape {
public:
    Model(const std::vector<Triangle> &triangles) {
        auto triangles_copy = triangles;
        bvh.build(std::move(triangles_copy));
    }

    Model(const std::filesystem::path &fileName);

    virtual std::optional<HitInfo> intersect(const Ray &ray, float t_min, float t_max) const override;

private:
    // AABB aabb {};
    BVH bvh {};
    //void build(); // 使包围盒包括所有三角形

    //std::vector<Triangle> triangles;
};