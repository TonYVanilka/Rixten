#pragma once
#include <fstream>
#include <vector>

struct Mesh {
    std::vector<float> vertices;
    std::vector<uint32_t> indices;
};

void  saveMesh(const Mesh& mesh, const char* filename) {
    std::ofstream file(filename);
    if (!file.is_open()) return;

    file << "V " << mesh.vertices.size() / 3 << "\n";
    for (float v : mesh.vertices) {
        file << v << " ";
    }
    file << "\n";

    file << "I " << mesh.indices.size() << "\n";
    for (uint32_t i : mesh.indices) {
        file << i << " ";
    }
    file << "\n";
}