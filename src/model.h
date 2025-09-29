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
#include "bone_info.h"

using namespace std;



class Model {
    public:
        Model(const char *path){
            loadModel(path);
        }
        void Draw(Shader &shader);
        //remember to make private and add getters and setters
        auto& GetBoneInfoMap() { return m_BoneInfoMap;}
        int& GetBoneCount() { return m_BoneCounter; }
    private:
        map<string, BoneInfo> m_BoneInfoMap;
        int m_BoneCounter = 0;

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