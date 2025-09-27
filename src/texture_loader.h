#pragma once
#include <iostream>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/texture.h>

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <string>

using namespace std;

class TextureLoader {
    public:
        static unsigned int TextureFromFile(const char *path, const string &directory);
        static unsigned int TextureFromEmbedded(const aiTexture* texture);
};