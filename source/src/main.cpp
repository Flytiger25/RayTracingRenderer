#include <chrono>
#include <iostream>
#include <glm/glm.hpp>
#include "thread_pool.hpp"
#include "film.hpp"
#include "camera.hpp"
#include "sphere.hpp"

class SimpleTask : public Task {
public:
    void run() override {
        std::cout << "Hello World!" << std::endl;
    }
};

int main() {
    Film film {1920, 1080};
    Camera camera {film, {0, 0, 1}, {0, 0, 0}, 90};
    Sphere sphere {{0, 0, 0}, 0.5f};
    glm::vec3 light_pos {1, 1, 1};

    for (int i = 0; i < film.getWidth(); i++) {
        for (int j = 0; j < film.getHeight(); j++) {
            auto ray = camera.generateRay({i, j});
            auto result = sphere.intersect(ray);
            if (result) {
                auto hit_point = ray.hit(result.value());
                auto normal = glm::normalize(hit_point - sphere.center);
                auto light_direction = glm::normalize(light_pos - hit_point);
                float cosine = glm::max(0.f, glm::dot(normal, light_direction));

                film.setPixel(i, j, {cosine, cosine, cosine});
            }
        }
    }
    //auto now = std::chrono::high_resolution_clock::now();
    film.save("test.ppm");
    // auto time = std::chrono::high_resolution_clock::now() - now;
    // auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(time);
    // ThreadPool thread_pool {};
    // thread_pool.addTask(new SimpleTask());
    // thread_pool.addTask(new SimpleTask());
    // thread_pool.addTask(new SimpleTask());
    // thread_pool.addTask(new SimpleTask());
    return 0;
}