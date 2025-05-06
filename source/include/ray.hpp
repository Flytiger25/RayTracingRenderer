#pragma once

#include <glm/glm.hpp>

struct Ray {
    glm::vec3 origin;
    glm::vec3 dirction;

    glm::vec3 hit(float t) const { return origin + t * dirction; }
};
