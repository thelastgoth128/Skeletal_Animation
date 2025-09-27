#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <map>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/texture.h>

#include "shaders/shader.h"
#include "mesh.h"
#include "assimp_glm_helpers.h"

using namespace std;

struct BoneInfo
{
    /*id is index in finalBoneMatrices*/
    int id;

    /*offset matrix transforms vertex from model space to bone space*/
    glm::mat4 offset;

};


class Model {
    public:
        Model(const char *path){
            loadModel(path);
        }
        void Draw(Shader &shader);
    private:
        map<string, BoneInfo> m_BoneInfoMap;
        int m_BoneCounter = 0;

        auto& GetBoneInfoMap() { return m_BoneInfoMap;}
        int& GetBoneCount() { return m_BoneCounter; }
        //model data
        vector<Mesh> meshes;
        string directory;
        vector<Texture> textures_loaded;

        void SetVertexBoneDataToDefault(Vertex& vertex);
        void SetVertexBoneData(Vertex& vertex, int boneID, float weight);
        void ExtractBoneWeightForVertices(std::vector<Vertex>& vertices, aiMesh* mesh, const aiScene* scene);
        void loadModel(string path);
        void processNode(aiNode *node, const aiScene *scene);
        Mesh processMesh(aiMesh *mesh, const aiScene *scene);
        vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, string typeName, const aiScene* scene);
};