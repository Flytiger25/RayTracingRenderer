#include "accelerate/aabb.hpp"

bool AABB::has_intersection(const Ray &ray, float &t_min, float &t_max) const {
    // 计算射线与AABB的交点
    glm::vec3 t1 = (b_min - ray.origin) / ray.direction;
    glm::vec3 t2 = (b_max - ray.origin) / ray.direction;
    glm::vec3 tmin = glm::min(t1, t2);
    glm::vec3 tmax = glm::max(t1, t2);

    // 判断有无交集
    float near = glm::max(glm::max(glm::max(tmin.x, tmin.y), tmin.z), t_min);
    float far = glm::min(glm::min(glm::min(tmax.x, tmax.y), tmax.z), t_max);

    return near <= far;
    //return near <= far && far > 0;
}
