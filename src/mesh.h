#pragma once
#include <glm/glm.hpp>
#include <iostream>
#include <vector>
#include "shaders/shader.h"
#define MAX_BONE_INFLUENCE 4

using namespace std;

struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;

    // tangent
    glm::vec3 Tangent;
    // bitangent
    glm::vec3 Bitangent;

    //bone indexes which will influence this vertex
    int m_BoneIDs[MAX_BONE_INFLUENCE];
    //weights from each bone
    float m_Weights[MAX_BONE_INFLUENCE];
};

struct Texture {
    unsigned int id;
    string type;
    string path; // store path of texture to compare with other textures
};

class Mesh {
    public:
        //mesh data
        vector<Vertex>          vertices;
        vector<unsigned int>    indices;
        vector<Texture>         textures;

        Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures);
        void Draw(Shader &shader);
    private:
        //render data
        unsigned int VAO, VBO, EBO;

        void setupMesh();
};