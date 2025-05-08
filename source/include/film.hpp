#pragma once

#include "glm/glm.hpp"
#include<filesystem>
#include <vector>

struct Pixel {
    glm::vec3 color {0, 0, 0};
    int sample_count {0};
};


class Film {
public:
    Film(int width, int height);

    void save(const std::filesystem::path &fileName);

    int getWidth() const { return width; }
    int getHeight() const { return height; }
    Pixel getPixel(int x, int y) { return pixels[y * width + x]; }
    void addSample(int x, int y, const glm::vec3 &color) { 
        pixels[y * width + x].color += color; 
        pixels[y * width + x].sample_count++; 
    }

private:
    int width;
    int height;
    std::vector<Pixel> pixels;
};