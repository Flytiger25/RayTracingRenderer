#include "camera/film.hpp"
#include "util/rgb.hpp"
#include "thread/thread_pool.hpp"
#include <fstream>

Film::Film(int width, int height) : width(width), height(height) {
    pixels.resize(width * height);
}

void Film::save(const std::filesystem::path &fileName) {
    std::ofstream file(fileName, std::ios::binary);
    file << "P6\n" << width << ' ' << height << "\n255\n";

    std::vector<uint8_t> buffer(width * height * 3);

    // 使用线程池保存图像
    thread_pool.parallel_for(width, height, [&](int x, int y) {
        auto pixel = getPixel(x, y);
        RGB rgb(pixel.color / static_cast<float>(pixel.sample_count));
        buffer[y * width * 3 + x * 3] = rgb.get_r();
        buffer[y * width * 3 + x * 3 + 1] = static_cast<uint8_t>(rgb.get_g());
        buffer[y * width * 3 + x * 3 + 2] = static_cast<uint8_t>(rgb.get_b());
    });
    thread_pool.wait();

    file.write(reinterpret_cast<const char *>(buffer.data()), buffer.size());

    // for (int y = 0; y < height; y++) {
    //     for (int x = 0; x < width; x++) {
    //         auto pixel = getPixel(x, y);
    //         RGB rgb(pixel.color / static_cast<float>(pixel.sample_count));
    //         file << static_cast<uint8_t>(rgb.get_r()) << static_cast<uint8_t>(rgb.get_g()) << static_cast<uint8_t>(rgb.get_b());
    //     }
    // }
}