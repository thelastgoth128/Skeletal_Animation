#pragma once
#include <glm/glm.hpp>
#include <iostream>
#include <vector>
#include "shaders/shader.h"

using namespace std;

struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
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