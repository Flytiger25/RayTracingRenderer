#pragma once

#include <glm/glm.hpp>

class Frame {
public:
    Frame(const glm::vec3 &normal);

    glm::vec3 local_from_world(const glm::vec3 &direction_world) const;
    glm::vec3 world_from_local(const glm::vec3 &direction_local) const;

private:
    glm::vec3 x_axis, y_axis, z_axis;
};