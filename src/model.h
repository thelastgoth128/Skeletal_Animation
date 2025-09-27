#pragma once
#include <iostream>
#include <vector>
#include <string>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/texture.h>

#include "shaders/shader.h"
#include "mesh.h"

using namespace std;

struct Texture {
    unsigned int id;
    string type;
    string path; // store path of texture to compare with other textures
};

class Model {
    public:
        Model(char *path){
            loadModel(path);
        }
        void Draw(Shader &shader);
    public:
        //model data
        vector<Mesh> meshes;
        string directory;
        vector<Texture> textures_loaded;

        void loadModel(string path);
        void processNode(aiNode *node, const aiScene *scene);
        Mesh processNode(aiMesh *mesh, const aiScene *scene);
        vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, string typeName, const aiScene* scene);
};