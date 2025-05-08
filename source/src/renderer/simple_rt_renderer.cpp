#include "renderer/simple_rt_renderer.hpp"
#include "util/frame.hpp"
#include "util/rng.hpp"
//#include "simple_rt_renderer.hpp"

glm::vec3 SimpleRTRenderer::render_pixel(const glm::ivec2 &pixel_coord) {
    auto ray = camera.generateRay(pixel_coord, {rng.uniform(), rng.uniform()});
    glm::vec3 beta = {1, 1, 1}; // 总反照率
    glm::vec3 color = {0, 0, 0};

    // 反射
    while (true) {
        auto hit_info = scene.intersect(ray);
        if (hit_info.has_value()) {
            color += beta * hit_info->material->emissive;
            beta *= hit_info->material->albedo;

            // 反射光线起点为交点
            ray.origin = hit_info->hit_point;

            // 反射方向考虑镜面反射和漫反射
            Frame frame(hit_info->normal); // 转换为局部坐标
            glm::vec3 light_dircetion;
            if (hit_info->material->is_specular) {
                // 镜面反射
                glm::vec3 view_direction = frame.local_from_world(-ray.direction);
                light_dircetion = {-view_direction.x, view_direction.y, -view_direction.z};
            } else {
                // 漫反射
                // 生成一个采样点
                do {
                    light_dircetion = {rng.uniform(), rng.uniform(), rng.uniform()};
                    light_dircetion = light_dircetion * 2.f - 1.f;
                } while (glm::length(light_dircetion) > 1);

                // 反射光线为半球方向
                if (light_dircetion.y < 0) {
                    light_dircetion.y = -light_dircetion.y;
                }
            }
            ray.direction = frame.world_from_local(light_dircetion);
        } else {
            break;
        }
    }
    
    return color;

}
