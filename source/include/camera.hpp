#pragma once

#include "film.hpp"
#include "ray.hpp"

class Camera
{
public:
    Camera(Film &film, const glm::vec3 &pos, const glm::vec3 &viewpoint, float fovy);

    Ray generateRay(const glm::ivec2 &pixel_coord, const glm::vec2 &offset = {0.5, 0.5}) const;

    Film &getFilm() { return film; }
    const Film &getFilm() const { return film; }

private:
    Film &film; // 持有的胶片
    glm::vec3 pos; // 相机位置
    glm::vec3 viewpoint;

    glm::mat4 camera_from_clip; // 从裁剪空间到相机空间的矩阵
    glm::mat4 world_from_camera; // 从相机空间到世界空间
};

