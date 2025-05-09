#include <chrono>
#include <iostream>
#include <glm/glm.hpp>
#include "thread/thread_pool.hpp"
#include "camera/film.hpp"
#include "camera/camera.hpp"
#include "shape/sphere.hpp"
#include "shape/model.hpp"
#include "shape/plane.hpp"
#include "shape/scene.hpp"
#include "util/frame.hpp"
#include "util/rgb.hpp"
#include "util/rng.hpp"
#include "util/progress.hpp"
#include "renderer/normal_renderer.hpp"
#include "renderer/simple_rt_renderer.hpp"

class SimpleTask : public Task {
public:
    void run() override {
        std::cout << "Hello World!" << std::endl;
    }
};

int main() {

    std::atomic<int> count = 0;

    RNG rng(23451334);
    int spp = 128;

    Film film {1920, 1080};
    Camera camera {film, {-3.6, 0, 0}, {0, 0, 0}, 45};
    //glm::vec3 light_pos {-1, 2, 1};

    Model model ("D:\\work\\RayTracingRenderer\\models\\dragon_87k.obj");
    Sphere sphere {{0, 0, 0}, 1.f};
    Plane plane {{0, 0, 0}, {0, 1, 0}};

    Scene scene {};
    scene.add_shape_instance(model, {RGB(202, 159, 117)}, {0, 0, 0}, {1, 3, 2});
    scene.add_shape_instance(sphere, {{1, 1, 1}, false, RGB{255, 128, 128}}, {0, 0, 2.5});
    scene.add_shape_instance(sphere, {{1, 1, 1}, false, RGB{128, 128, 255}}, {0, 0, -2.5});
    scene.add_shape_instance(sphere, {{1, 1, 1}, true}, {3, 0.5, -2});
    scene.add_shape_instance(plane, {RGB(120, 204, 157)}, {0, -0.5, 0});

    NormalRenderer normal_renderer {camera, scene};
    normal_renderer.render(spp, "D:\\work\\RayTracingRenderer\\normal.ppm");

    film.clear();

    SimpleRTRenderer simple_rt_renderer {camera, scene};
    simple_rt_renderer.render(spp, "D:\\work\\RayTracingRenderer\\simple_rt.ppm");

/*
    Progress progress(film.getWidth() * film.getHeight() * spp);

    thread_pool.parallel_for(film.getWidth(), film.getHeight(), [&](int x, int y) {
        for (int i = 0; i < spp; i++) {
            auto ray = camera.generateRay({x, y}, {rng.uniform(), rng.uniform()});
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
    
            film.addSample(x, y, color);
        }

        // auto hit_info = scene.intersect(ray);
        // if (hit_info.has_value()) {
        //     auto light_direction = glm::normalize(light_pos - hit_info->hit_point);
        //     float cosine = glm::max(0.f, glm::dot(hit_info->normal, light_direction));

        //     film.setPixel(x, y, {cosine, cosine, cosine});
        // }

        // 进度条
        progress.update(spp);
    });
    thread_pool.wait();

    // for (int i = 0; i < film.getWidth(); i++) {
    //     for (int j = 0; j < film.getHeight(); j++) {
    //         auto ray = camera.generateRay({i, j});
    //         auto hit_info = shape.intersect(ray);
    //         if (hit_info) {
    //             auto light_direction = glm::normalize(light_pos - hit_info->hit_point);
    //             float cosine = glm::max(0.f, glm::dot(hit_info->normal, light_direction));

    //             film.setPixel(i, j, {cosine, cosine, cosine});
    //         }
    //         count++;
    //         if (count % film.getWidth() == 0) {
    //             std::cout << static_cast<float>(count) / (film.getWidth() * film.getHeight()) << std::endl;
    //         }
    //     }
    // }

    film.save("D:\\work\\RayTracingRenderer\\test.ppm");
    // auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(time);
    // ThreadPool thread_pool {};
    // thread_pool.addTask(new SimpleTask());
    // thread_pool.addTask(new SimpleTask());
    // thread_pool.addTask(new SimpleTask());
    // thread_pool.addTask(new SimpleTask());
*/
    return 0;
}