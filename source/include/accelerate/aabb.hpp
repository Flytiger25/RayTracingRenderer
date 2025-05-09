#pragma once

#include "glm/glm.hpp"
#include "camera/ray.hpp"

struct AABB {
    AABB() : b_min(glm::vec3(std::numeric_limits<float>::infinity())), b_max(glm::vec3(-std::numeric_limits<float>::infinity())) {}

    AABB(const glm::vec3 &b_min, const glm::vec3 &b_max) : b_min(b_min), b_max(b_max) {}

    void expand(const glm::vec3 &point) {
        b_min = glm::min(b_min, point);
        b_max = glm::max(b_max, point);
    }

    bool has_intersection(const Ray &ray, float &t_min, float &t_max) const;

    glm::vec3 get_diagonal() const {
        return b_max - b_min;
    }

    glm::vec3 b_min;
    glm::vec3 b_max;
};

