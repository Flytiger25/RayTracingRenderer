#pragma once

#include "glm/glm.hpp"
#include<filesystem>
#include <vector>

class Film {
public:
    Film(int width, int height);

    void save(const std::filesystem::path &fileName);

    int getWidth() const { return width; }
    int getHeight() const { return height; }
    glm::vec3 getPixel(int x, int y) { return pixels[y * width + x]; }
    void setPixel(int x, int y, const glm::vec3 &color) { pixels[y * width + x] = color; }

private:
    int width;
    int height;
    std::vector<glm::vec3> pixels;
};