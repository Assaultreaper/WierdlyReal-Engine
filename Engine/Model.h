#ifndef MODEL_H
#define MODEL_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <stb/stb_image.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "cMesh.h"
#include "Shader.h"
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>
#include <glm/gtc/matrix_transform.hpp>

unsigned int TextureFromFile(const char* path, const std::string& directory, bool gamma = false);

class Model {
public:
    std::vector<Texture> textures_loaded;
    std::vector<cMesh> meshes;
    std::string directory;
    bool gammaCorrection;

    Model(const std::string& path, bool gamma = false);

    void Draw(Shader& shader);

    void loadPLYModel(const std::string& path);

    void Translate(const glm::vec3& translation, Shader& shader);
    void Scale(const glm::vec3& scale, Shader& shader);

private:
    void loadModel(const std::string& path);
    void processNode(aiNode* node, const aiScene* scene);
    cMesh processMesh(aiMesh* mesh, const aiScene* scene);
    std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
};

#endif
