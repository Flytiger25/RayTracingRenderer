#include "camera/film.hpp"
#include "util/rgb.hpp"
#include <fstream>

Film::Film(int width, int height) : width(width), height(height) {
    pixels.resize(width * height);
}

void Film::save(const std::filesystem::path &fileName) {
    std::ofstream file(fileName, std::ios::binary);
    file << "P6\n" << width << ' ' << height << "\n255\n";

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            auto pixel = getPixel(x, y);
            RGB rgb(pixel.color / static_cast<float>(pixel.sample_count));
            file << static_cast<uint8_t>(rgb.get_r()) << static_cast<uint8_t>(rgb.get_g()) << static_cast<uint8_t>(rgb.get_b());
        }
    }
}