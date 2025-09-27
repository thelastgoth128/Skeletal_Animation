#include "model.h"
#include "texture_loader.h"

void Model::Draw(Shader &shader) {
    for(unsigned int i = 0; i < meshes.size(); i++) {
        meshes[i].Draw(shader);
    }
}

void Model::loadModel(string path) {
    Assimp::Importer import;
    const aiScene *scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        cout << "ERROR::ASSIMP::" <<import.GetErrorString() << endl;
        return;
    }
    directory = path.substr(0, path.find_last_of('/'));

    processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode *node, const aiScene *scene) {
    //process all the node's meshes (if any)
    for(unsigned int i = 0; i < node->mNumMeshes; i++) {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(processMesh(mesh, scene));
    }
    // then do the same for each of its childeren
    for (unsigned int i = 0; i < node->mNumChildren; i++) {
        processNode(node->mChildren[i], scene);
    }
}

Mesh Model::processMesh(aiMesh* mesh, const aiScene *scene) {
    vector<Vertex> vertices;
    vector<unsigned int> indices;
    vector<Texture> textures;

    for(unsigned int i = 0; i < mesh->mNumVertices; i++) {
        Vertex vertex;
        //process vertex positions, normals and texture coordinates
        glm::vec3 vector;
        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].y;
        vector.z = mesh->mVertices[i].z;
        vertex.Position = vector;

        vector.x = mesh->mNormals[i].x;
        vector.y = mesh->mNormals[i].y;
        vector.z = mesh->mNormals[i].z;
        vertex.Normal = vector;

        //does the mesh contain texture coordinates?
        if (mesh->mTextureCoords[0]) {
            glm::vec2 vec;
            vec.x = mesh->mTextureCoords[0][i].x;
            vec.y = mesh->mTextureCoords[0][i].y;
            vertex.TexCoords = vec;
        }
        else {
            vertex.TexCoords = glm::vec2(0.0f, 0.0f);
        }

        vertices.push_back(vertex);
    }
    //process indices
    for(unsigned int i = 0; i < mesh->mNumFaces; i++) {
        aiFace face =mesh->mFaces[i];
        for(unsigned int j = 0; j < face.mNumIndices; j++){
            indices.push_back(face.mIndices[j]);
        }
    }
    //process material
    if(mesh->mMaterialIndex >= 0) {
        aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
        vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse", scene);
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
        vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular",scene);
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    }
    return Mesh(vertices, indices, textures);
}

vector<Texture> Model::loadMaterialTextures(aiMaterial *mat, aiTextureType type, string typeName, const aiScene* scene)
{
    std::cout << "getting texture diffuse" << endl;
    vector<Texture> textures;
    
    for(unsigned int i = 0; i < mat->GetTextureCount(type); i++)
    {
        aiString str;
        mat->GetTexture(type, i, &str);
        
        // Check if already loaded
        bool skip = false;
        for(unsigned int j = 0; j < textures_loaded.size(); j++)
        {
            if(std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0)
            {
                textures.push_back(textures_loaded[j]);
                skip = true;
                break;
            }
        }
        
        if(!skip)
        {
            std::cout << "Getting texture from embedded" << endl;
            std::cout << "[TextureLoader] Attempting to load texture: " << str.C_Str() << " of type: " << typeName << std::endl;
            
            // Try embedded texture first
            const aiTexture* embeddedTex = scene->GetEmbeddedTexture(str.C_Str());
            
            if (embeddedTex) {
                std::cout << "[TextureLoader] Embedded texture found: " << str.C_Str() << std::endl;
                try {
                    Texture texture;
                    texture.id = TextureLoader::TextureFromEmbedded(embeddedTex);
                    texture.type = typeName;
                    texture.path = str.C_Str();
                    textures.push_back(texture);
                    textures_loaded.push_back(texture);
                    
                    // IMPORTANT: Continue to next texture, don't try file loading
                    continue;
                    
                } catch (...) {
                    std::cerr << "[TextureLoader] Failed to load embedded texture: " << str.C_Str() << std::endl;
                    // If embedded fails, try external file below
                }
            }
            
            // Only try external file if embedded texture wasn't found or failed
            if (!embeddedTex) {
                std::cout << "[TextureLoader] Trying external texture: " << str.C_Str() << std::endl;
                try {
                    Texture texture;
                    texture.id = TextureLoader::TextureFromFile(str.C_Str(), directory);
                    texture.type = typeName;
                    texture.path = str.C_Str();
                    textures.push_back(texture);
                    textures_loaded.push_back(texture);
                    std::cout << "Done loading external texture" << endl;
                } catch (...) {
                    std::cerr << "[TextureLoader] Failed to load external texture: " << str.C_Str() << std::endl;
                }
            }
        }
    }
    
    // // Only add fallback if NO textures were loaded at all
    // if (textures.empty()) {
    //     std::cout << "[TextureLoader] No textures found. Creating default texture.\n";
        
    //     // Create a simple default texture instead of trying to load from GLB file
    //     Texture fallback;
    //     fallback.id = createDefaultTexture(); // You need to implement this
    //     fallback.type = typeName;
    //     fallback.path = "default_generated";
    //     textures.push_back(fallback);
    //     textures_loaded.push_back(fallback);
    // }

    return textures;
}