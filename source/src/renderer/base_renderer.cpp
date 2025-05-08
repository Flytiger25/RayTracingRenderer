#include "renderer/base_renderer.hpp"
#include "util/progress.hpp"
#include <thread/thread_pool.hpp>
#include <iostream>

void BaseRenderer::render(int spp, const std::filesystem::path& filename) {
    int current_spp = 0;
    auto film = camera.getFilm();
    int increase = 1;
    Progress progress(film.getWidth() * film.getHeight() * spp);
    while (current_spp < spp) {
        // 渲染当前spp
        thread_pool.parallel_for(film.getWidth(), film.getHeight(), [&](int x, int y) {
            for (int i = 0; i < increase; i++) {
                film.addSample(x, y, render_pixel({x, y}));
            }
            progress.update(increase);
        });
        thread_pool.wait();

        // 保存当前spp的图像
        current_spp += increase;
        increase = std::min(current_spp, 32);

        film.save(filename);
        std::cout << "save " << current_spp << " spp"  << " to " << filename << std::endl;
    }
}
