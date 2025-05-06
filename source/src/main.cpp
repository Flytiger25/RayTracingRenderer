#include <chrono>
#include <iostream>
#include <glm/glm.hpp>
#include "thread_pool.hpp"
#include "film.hpp"
#include "camera.hpp"
#include "sphere.hpp"
#include "model.hpp"

class SimpleTask : public Task {
public:
    void run() override {
        std::cout << "Hello World!" << std::endl;
    }
};

int main() {

    ThreadPool thread_pool;
    std::atomic<int> count = 0;

    Film film {1920, 1080};
    Camera camera {film, {-0.6, 0, 0}, {0, 0, 0}, 90};
    Model model ("models/simple_dragon.obj");
    Sphere sphere {{0, 0, 0}, 0.5f};
    Shape &shape = model;
    glm::vec3 light_pos {-1, 2, 1};

    thread_pool.parallel_for(film.getWidth(), film.getHeight(), [&](int x, int y) {
        auto ray = camera.generateRay({x, y});
        auto hit_info = shape.intersect(ray);
        if (hit_info) {
            auto light_direction = glm::normalize(light_pos - hit_info->hit_point);
            float cosine = glm::max(0.f, glm::dot(hit_info->normal, light_direction));

            film.setPixel(x, y, {cosine, cosine, cosine});
        }
        count++;
        if (count % film.getWidth() == 0) {
            std::cout << static_cast<float>(count) / (film.getWidth() * film.getHeight()) << std::endl;
        }
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

    film.save("test.ppm");
    // auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(time);
    // ThreadPool thread_pool {};
    // thread_pool.addTask(new SimpleTask());
    // thread_pool.addTask(new SimpleTask());
    // thread_pool.addTask(new SimpleTask());
    // thread_pool.addTask(new SimpleTask());
    return 0;
}