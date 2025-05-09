#pragma once

#include <filesystem>
#include "triangle.hpp"
#include "accelerate/aabb.hpp"

class Model : public Shape {
public:
    Model(const std::vector<Triangle> &triangles) : triangles(triangles) {build();}

    Model(const std::filesystem::path &fileName);

    virtual std::optional<HitInfo> intersect(const Ray &ray, float t_min, float t_max) const override;

private:
    AABB aabb {};
    void build(); // 使包围盒包括所有三角形

    std::vector<Triangle> triangles;
};