#define TINYOBJLOADER_IMPLEMENTATION

#include "obj_loader.h"

#include "tiny_obj_loader.h"

void components_to_vec3s(const std::vector<float> components, std::vector<glm::vec3>& vecs) {
    for(size_t vec_start = 0; vec_start < components.size(); vec_start += 3) {
        vecs.push_back(
            glm::vec3(components[vec_start],
                components[vec_start+1],
                components[vec_start+2]
            ));
    }
}

// vertices contains a list of components in (v1, v2, v3, n1, n2, n3) format
// indexes contain a list of indexes into vertices that separate distinct scene objects
void loadObj(std::string objFilePath, std::vector<float> &vertices, std::vector<int> &indexes) {
    vertices = std::vector<float>();
    indexes = std::vector<int>();
    indexes.push_back(0);

    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> objmaterials;
    std::string warn;
    std::string err;

    bool success = tinyobj::LoadObj(&attrib, &shapes, &objmaterials, &warn, &err, objFilePath.c_str(), NULL, true);

    if (!err.empty()) {
        std::cerr << err << std::endl;
    }
    if (!success) {
        exit(1);
    }

    std::vector<glm::vec3> verts = std::vector<glm::vec3>();
    components_to_vec3s(attrib.vertices, verts);
    std::vector<glm::vec3> norms = std::vector<glm::vec3>();
    components_to_vec3s(attrib.normals, norms);

    for (auto shape = shapes.begin(); shape < shapes.end(); shape++) {
        const std::vector<tinyobj::index_t> &indices = shape->mesh.indices;

        for (int i = 0; i < indices.size(); i++) {
            glm::vec3 vertex = verts[indices[i].vertex_index];
            glm::vec3 normal = norms[indices[i].normal_index];
            vertices.push_back(vertex[0]);
            vertices.push_back(vertex[1]);
            vertices.push_back(vertex[2]);
            vertices.push_back(normal[0]);
            vertices.push_back(normal[1]);
            vertices.push_back(normal[2]);
        }

        indexes.push_back(vertices.size() / 6);
    }
}
