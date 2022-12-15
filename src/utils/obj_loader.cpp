#include <unordered_map>
#include <string>
#include <iostream>

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

std::string process(std::string const& s)
{
    std::string::size_type pos = s.find('(');
    if (pos != std::string::npos)
    {
        return s.substr(0, pos);
    }
    else
    {
        return s;
    }
}

std::unordered_map<std::string, glm::vec4> getMaterialMap() {
    auto materialMap = std::unordered_map<std::string, glm::vec4>();

    materialMap["Dungeon_Big_Passage"] =
            glm::vec4(115, 106, 73, 255) / 255.f;

    materialMap["Dungeon_Big_Wall"] =
            glm::vec4(115, 106, 73, 255) / 255.f;

    materialMap["Basement_Wall_Var1"] =
            glm::vec4(115, 106, 73, 255) / 255.f;

    materialMap["Dungeon_Hall_Center"] =
            glm::vec4(51, 54, 37, 255) / 255.f;

    materialMap["Table_Big"] =
            glm::vec4(175, 138, 77, 255) / 255.f;

    materialMap["Chair"] =
            glm::vec4(175, 138, 77, 255) / 255.f;

    materialMap["Carpet_Red"] =
            glm::vec4(90, 58, 38, 255) / 255.f;

    materialMap["Chandelier"] =
            glm::vec4(175, 138, 77, 255) / 255.f;

    materialMap["Door_Wooden_Round_Left"] =
            glm::vec4(70, 55, 37, 255) / 255.f;

    materialMap["Banner"] =
            glm::vec4(87, 41, 30, 255) / 255.f;

    materialMap["Shelf"] =
            glm::vec4(128, 109, 75, 255) / 255.f;

    materialMap["Candlestick_Wall"] =
            glm::vec4(48, 46, 34, 255) / 255.f;

    materialMap["Barrel_Closed"] =
            glm::vec4(137, 114, 70, 255) / 255.f;

    materialMap["Barrel_Open"] =
            glm::vec4(137, 114, 70, 255) / 255.f;

    materialMap["Barrel_Big"] =
            glm::vec4(137, 114, 70, 255) / 255.f;

    materialMap["Bench"] =
            glm::vec4(114, 99, 70, 255) / 255.f;

    materialMap["Desk"] =
            glm::vec4(93, 80, 54, 255) / 255.f;

    materialMap["Door_Iron_Left"] =
            glm::vec4(51, 45, 35, 255) / 255.f;

    materialMap["Jail_Wall"] =
            glm::vec4(64, 56, 43, 255) / 255.f;

    materialMap["Jail_Gate"] =
            glm::vec4(64, 56, 43, 255) / 255.f;

    materialMap["Jail_Wall_broken"] =
            glm::vec4(64, 56, 43, 255) / 255.f;

    materialMap["Torch_Wall"] =
            glm::vec4(42, 37, 29, 255) / 255.f;

    materialMap["Skull"] =
            glm::vec4(225, 225, 225, 255) / 255.f;

    materialMap["Shield"] =
            glm::vec4(61, 58, 45, 255) / 255.f;

    materialMap["Sword"] =
            glm::vec4(55, 55, 49, 255) / 255.f;

    materialMap["Spear"] =
            glm::vec4(55, 55, 49, 255) / 255.f;

    materialMap["Chest"] =
            glm::vec4(49, 41, 30, 255) / 255.f;

    materialMap["Book_DeepRed"] =
            glm::vec4(62, 29, 21, 255) / 255.f;

    materialMap["Book_Small_Green"] =
            glm::vec4(52, 54, 32, 255) / 255.f;

    materialMap["Book_Big_Brown"] =
            glm::vec4(56, 46, 32, 255) / 255.f;

    materialMap["FirewoodStack"] =
            glm::vec4(66, 52, 34, 255) / 255.f;

    materialMap["Goblet"] =
            glm::vec4(249, 211, 91, 255) / 255.f;

    materialMap["Meat"] =
            glm::vec4(195, 118, 61, 255) / 255.f;

    materialMap["Carrot"] =
            glm::vec4(229, 115, 56, 255) / 255.f;

    materialMap["Potato"] =
            glm::vec4(163, 134, 79, 255) / 255.f;

    materialMap["Plate"] =
            glm::vec4(230, 202, 129, 255) / 255.f;

    materialMap["CoatHook_Full"] =
            glm::vec4(137, 112, 74, 255) / 255.f;

    materialMap["WeaponRack_Big"] =
            glm::vec4(59, 48, 34, 255) / 255.f;

    materialMap["Door_Wooden_Left"] =
            glm::vec4(70, 55, 37, 255) / 255.f;

    materialMap["Switch"] =
            glm::vec4(93, 79, 52, 255) / 255.f;

    materialMap["Switch_Base"] =
            glm::vec4(93, 79, 52, 255) / 255.f;

    materialMap["Switch_Lever"] =
            glm::vec4(93, 79, 52, 255) / 255.f;

    materialMap["Jug"] =
            glm::vec4(194, 140, 77, 255) / 255.f;

    materialMap["Bag"] =
            glm::vec4(113, 107, 80, 255) / 255.f;

    materialMap["Bottle"] =
            glm::vec4(168, 123, 69, 255) / 255.f;

    materialMap["Drinkinghorn"] =
            glm::vec4(252, 230, 147, 255) / 255.f;

    materialMap["Book_Red"] =
            glm::vec4(105, 71, 50, 255) / 255.f;

    materialMap["Book_Small_Brown"] =
            glm::vec4(149, 107, 59, 255) / 255.f;

    materialMap["Book_Small_DeepRed"] =
            glm::vec4(124, 53, 36, 255) / 255.f;

    materialMap["Book_Green"] =
            glm::vec4(52, 54, 32, 255) / 255.f;

    materialMap["Beaker_Full_Var1"] =
            glm::vec4(90, 85, 43, 255) / 255.f;

    materialMap["Cauldron_Small_Stand"] =
            glm::vec4(33, 25, 15, 255) / 255.f;

    materialMap["Chest_Lid"] =
            glm::vec4(49, 41, 30, 255) / 255.f;

    materialMap["Bucket"] =
            glm::vec4(49, 41, 30, 255) / 255.f;

    materialMap["SwitchSecret"] =
            glm::vec4(61, 59, 49, 255) / 255.f;

    materialMap["ChestSmall_Lid"] =
            glm::vec4(58, 49, 35, 255) / 255.f;

    materialMap["ChestSmall"] =
            glm::vec4(58, 49, 35, 255) / 255.f;

    materialMap["Scroll_Elder"] =
            glm::vec4(121, 108, 65, 255) / 255.f;

    materialMap["Tree_Branch"] =
            glm::vec4(66, 69, 46, 255) / 255.f;

    return materialMap;
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

    auto materialMap = getMaterialMap();

    for (auto shape = shapes.begin(); shape < shapes.end(); shape++) {
        const std::vector<tinyobj::index_t> &indices = shape->mesh.indices;

        std::string name = process(shape->name);
        std::string::iterator end_pos = std::remove(name.begin(), name.end(), ' ');
        name.erase(end_pos, name.end());

        bool hasColor = materialMap.count(name);

        for (int i = 0; i < indices.size(); i++) {
            glm::vec3 vertex = verts[indices[i].vertex_index];
            glm::vec3 normal = norms[indices[i].normal_index];
            vertices.push_back(vertex[0]);
            vertices.push_back(vertex[1]);
            vertices.push_back(vertex[2]);
            vertices.push_back(normal[0]);
            vertices.push_back(normal[1]);
            vertices.push_back(normal[2]);
            if (hasColor) {
                glm::vec4 colors = materialMap[name];
                vertices.push_back(colors[0]);
                vertices.push_back(colors[1]);
                vertices.push_back(colors[2]);
                vertices.push_back(colors[3]);
            } else {
                vertices.push_back(1);
                vertices.push_back(1);
                vertices.push_back(1);
                vertices.push_back(1);
            }
        }

        indexes.push_back(vertices.size() / 10);
    }
}
