#include "accelerate/bvh.hpp"

void BVH::build(std::vector<Triangle> &&triangles) {
    root = new BVHNode();
    root->triangles = std::move(triangles);
    root->update_aabb();
    recursive_split(root);
}

std::optional<HitInfo> BVH::intersect(const Ray &ray, float t_min, float t_max) const {
    std::optional<HitInfo> closest_hit_info {};
    recursive_intersect(root, ray, t_min, t_max, closest_hit_info);
    return closest_hit_info;
}

void BVH::recursive_split(BVHNode *node) {
    if (node->triangles.size() <= 1) {
        return;
    }

    auto diagonal = node->aabb.get_diagonal();

    // 找到最长的轴
    int max_axis = diagonal.x > diagonal.y ? (diagonal.x > diagonal.z ? 0 : 2) : (diagonal.y > diagonal.z ? 1 : 2);
    
    // 计算中心坐标
    auto box_center = node->aabb.b_min + diagonal * 0.5f;

    std::vector<Triangle> left_triangles;
    std::vector<Triangle> right_triangles;
    
    for (auto &triangle : node->triangles) {
        auto triangle_center = (triangle.p0 + triangle.p1 + triangle.p2) / 3.0f;
        if (triangle_center[max_axis] < box_center[max_axis]) {
            left_triangles.push_back(triangle);
        } else {
            right_triangles.push_back(triangle);
        }
    }

    if (left_triangles.size() == 0 || right_triangles.size() == 0) {
        return;
    }

    auto *left_node = new BVHNode {};
    auto *right_node = new BVHNode {};

    node->left = left_node;
    node->right = right_node;

    left_node->triangles = std::move(left_triangles);
    right_node->triangles = std::move(right_triangles);

    left_node->update_aabb();
    right_node->update_aabb();

    recursive_split(left_node);
    recursive_split(right_node);
}

void BVH::recursive_intersect(const BVHNode *node, const Ray &ray, float t_min, float t_max, std::optional<HitInfo> &closest_hit_info) const {
    if (!node->aabb.has_intersection(ray, t_min, t_max)) {
        return;
    }

    if (node->left == nullptr && node->right == nullptr) {
        // 叶子节点
        for (auto &triangle : node->triangles) {
            auto hit_info = triangle.intersect(ray, t_min, t_max);
            if (hit_info.has_value() && (!closest_hit_info.has_value() || hit_info->t < closest_hit_info->t)) {
                closest_hit_info = hit_info;
            }
        }
    } else {
        // 非叶子节点
        recursive_intersect(node->left, ray, t_min, t_max, closest_hit_info);
        recursive_intersect(node->right, ray, t_min, t_max, closest_hit_info);
    }
}
    
