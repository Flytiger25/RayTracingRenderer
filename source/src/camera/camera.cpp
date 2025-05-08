#include "camera/camera.hpp"
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

Camera::Camera(Film &film, const glm::vec3 &pos, const glm::vec3 &viewpoint, float fovy)  : film(film), pos(pos) {
    camera_from_clip = glm::inverse(glm::perspective(
        glm::radians(fovy), 
        static_cast<float>(film.getWidth()) / static_cast<float>(film.getHeight()), 
        1.f, // near
        2.f // far
    )); // 透视投影矩阵的逆矩阵
    world_from_camera = glm::inverse(glm::lookAt(pos, viewpoint, {0, 1, 0}));
}

Ray Camera::generateRay(const glm::ivec2 &pixel_coord, const glm::vec2 &offset) const {
    glm::vec2 ndc_coord = (glm::vec2(pixel_coord) + offset) / glm::vec2(film.getWidth(), film.getHeight());
    // ndc空间y轴向上，屏幕空间y轴向下，需要反转y轴
    ndc_coord.y = 1.f - ndc_coord.y;
    // [0, 1] --> [-1, 1]
    ndc_coord = 2.f * ndc_coord - 1.f;

    // (x, y, 0, near) --> (x/near, y/near, 0, 1)
    glm::vec4 clip_coord { ndc_coord, 0, 1 }; // 裁剪空间下的齐次坐标
    // 转换到世界空间下的坐标
    glm::vec3 world_coord = world_from_camera * camera_from_clip * clip_coord;

    return Ray {pos, glm::normalize(world_coord - pos)};
}