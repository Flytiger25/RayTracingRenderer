#include "sphere.hpp"

std::optional<float> Sphere::intersect(const Ray &ray) const {
    // 解一元二次方程
    glm::vec3 co = ray.origin - center;
    float b = 2 * glm::dot(ray.dirction, co);
    float c = glm::dot(co, co) - radius * radius;
    float delta = b * b - 4 * c;
    if (delta < 0) {
        return {}; // 无解
    } 
    float hit_t = 0.5 * (-b - glm::sqrt(delta));
    if (hit_t < 0) {
        hit_t = 0.5 * (-b + glm::sqrt(delta));
    }
    if (hit_t > 0) {
        return hit_t;
    }
    return {};
}