#include "shape/model.hpp"
#include <fstream>
#include <sstream>
#include <iostream>

Model::Model(const std::filesystem::path &fileName) {
    std::vector<glm::vec3> positions;
    std::vector<glm::vec3> normals;

    std::vector<Triangle> triangles;

    std::ifstream file(fileName);
    if (!file) {
        std::cout << "file open fail!" << std::endl;
        return;
    }

    std::string line;
    char trash;
    while (!file.eof()) {
        std::getline(file, line);
        std::istringstream iss(line);

        if (line.compare(0, 2, "v ") == 0) {
            // v 0.206225 -0.272783 0.202053
            glm::vec3 pos;
            iss >> trash >> pos.x >> pos.y >> pos.z;
            positions.push_back(pos);
        } 
        else if (line.compare(0, 3, "vn ") == 0) {
            // vn -0.0865 0.3765 -0.9224
            glm::vec3 normal;
            iss >> trash >> trash >> normal.x >> normal.y >> normal.z;
            normals.push_back(normal);
        }
        else if (line.compare(0, 2, "f ") == 0) {
            //f 95//481 47//482 165//483
            glm::ivec3 idx_v, idx_vn;
            iss >> trash;
            iss >> idx_v.x >> trash >> trash >> idx_vn.x;
            iss >> idx_v.y >> trash >> trash >> idx_vn.y;
            iss >> idx_v.z >> trash >> trash >> idx_vn.z;
            Triangle triangle(positions[idx_v.x - 1], positions[idx_v.y- 1], positions[idx_v.z - 1], 
                              normals[idx_vn.x - 1], normals[idx_vn.y - 1], normals[idx_vn.z - 1]);
            triangles.push_back(triangle);
        }
    }
    bvh.build(std::move(triangles));
}

std::optional<HitInfo> Model::intersect(const Ray &ray, float t_min, float t_max) const {
    return bvh.intersect(ray, t_min, t_max);

    // if (!aabb.has_intersection(ray, t_min, t_max)) {
    //     return {};
    // }
    
    // std::optional<HitInfo> closest_hit_info {};

    // for (const auto &triangle : triangles) {
    //     auto hit_info = triangle.intersect(ray, t_min, t_max);
    //     if (hit_info.has_value()) {
    //         t_max = hit_info->t;
    //         closest_hit_info = hit_info;
    //     }
    // }

    // return closest_hit_info;
}

// void Model::build() {
//     for (const auto &triangle : triangles) {
//         aabb.expand(triangle.p0);
//         aabb.expand(triangle.p1);
//         aabb.expand(triangle.p2);
//     }
// }