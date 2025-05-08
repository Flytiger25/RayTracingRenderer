#pragma once

#include <glm/glm.hpp>

struct Material {
    glm::vec3 albedo = {1, 1, 1}; // 反照率
    bool is_specular = false; // 是否镜面反射
    glm::vec3 emissive = {0, 0, 0}; // 自发光

};