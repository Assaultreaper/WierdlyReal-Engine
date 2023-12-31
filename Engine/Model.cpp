#include "Model.h"

#include "Model.h"

Model::Model(const std::string& path, bool gamma) : gammaCorrection(gamma), directory() {
    loadModel("assets/Models/" + path + "/" + path + ".ply");
}

void Model::Draw(Shader& shader) const {
    for (unsigned int i = 0; i < meshes.size(); i++)
        meshes[i].Draw(shader);
}

void Model::loadPLYModel(const std::string& path)
{
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cout << "ERROR::MODEL::Failed to open PLY file: " << path << std::endl;
        return;
    }

    std::string line;
    unsigned int numVertices = 0;
    unsigned int numFaces = 0;

    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string element;
        iss >> element;

        if (element == "element") {
            std::string elementType;
            iss >> elementType;

            if (elementType == "vertex") {
                iss >> numVertices;
            }
            else if (elementType == "face") {
                iss >> numFaces;
            }
        }
        else if (element == "end_header") {
            break;
        }
    }

    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    for (unsigned int i = 0; i < numVertices; ++i) {
        Vertex vertex;
        file >> vertex.Position.x >> vertex.Position.y >> vertex.Position.z
            >> vertex.Normal.x >> vertex.Normal.y >> vertex.Normal.z
            >> vertex.TexCoords.x >> vertex.TexCoords.y;

        vertices.push_back(vertex);
    }

    for (unsigned int i = 0; i < numFaces; ++i) {
        unsigned int numIndices;
        file >> numIndices;

        if (numIndices == 3) {  // Assuming triangles
            unsigned int index1, index2, index3;
            file >> index1 >> index2 >> index3;
            indices.push_back(index1);
            indices.push_back(index2);
            indices.push_back(index3);
        }
    }

    file.close();

    cMesh mesh(vertices, indices, std::vector<Texture>());
    meshes.push_back(mesh);
}

Physics::AABB Model::CalculateAABB() const
{
    if (meshes.empty())
    {
        // Return an invalid AABB if there are no meshes
        return Physics::AABB();
    }

    std::vector<glm::vec3> vertices;

    for (const cMesh& mesh : meshes)
    {
        const std::vector<Vertex>& meshVertices = mesh.vertices;

        for (const Vertex& vertex : meshVertices)
        {
            vertices.push_back(vertex.Position);
        }
    }

    if (vertices.empty())
    {
        // Return an invalid AABB if there are no vertices
        return Physics::AABB();
    }

    glm::vec3 minBounds = vertices[0];
    glm::vec3 maxBounds = vertices[0];

    for (const glm::vec3& vertex : vertices)
    {
        minBounds = glm::min(minBounds, vertex);
        maxBounds = glm::max(maxBounds, vertex);
    }

    return Physics::AABB(minBounds, maxBounds);
}

void Model::RenderAABB(const Shader& shader) const
{
    Physics::AABB aabb = CalculateAABB();
    glm::vec3 minBounds = aabb.GetMinBounds();
    glm::vec3 maxBounds = aabb.GetMaxBounds();

    // Define vertices and indices
    GLfloat vertices[] = {
        minBounds.x, minBounds.y, minBounds.z,
        maxBounds.x, minBounds.y, minBounds.z,
        maxBounds.x, maxBounds.y, minBounds.z,
        minBounds.x, maxBounds.y, minBounds.z,
        minBounds.x, minBounds.y, maxBounds.z,
        maxBounds.x, minBounds.y, maxBounds.z,
        maxBounds.x, maxBounds.y, maxBounds.z,
        minBounds.x, maxBounds.y, maxBounds.z
    };

    GLuint indices[] = {
        0, 1, 1, 2, 2, 3, 3, 0,
        4, 5, 5, 6, 6, 7, 7, 4,
        0, 4, 1, 5, 2, 6, 3, 7
    };

    // Generate and bind buffers
    GLuint VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Specify vertex attribute pointers
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Use the provided shader
    shader.use();

    // Set model matrix (position and size)
    glm::mat4 model = glm::mat4(1.0f);
    shader.setMat4("model", model);

    // Render the AABB
    glBindVertexArray(VAO);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glDrawElements(GL_LINES, 24, GL_UNSIGNED_INT, 0);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glBindVertexArray(0);

    // Clean up buffers
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

void Model::Translate(const glm::vec3& translation, Shader& shader)
{
    for (cMesh& mesh : meshes)
    {
        mesh.modelMatrix = glm::translate(mesh.modelMatrix, translation);
        shader.setMat4("model", mesh.modelMatrix);
    }
}

void Model::Scale(const glm::vec3& scale, Shader& shader)
{
    for (cMesh& mesh : meshes)
    {
        mesh.modelMatrix = glm::scale(mesh.modelMatrix, scale);
        shader.setMat4("model", mesh.modelMatrix);
    }
}

void Model::loadModel(const std::string& path)
{
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
        return;
    }

    directory = path.substr(0, path.find_last_of('/'));
    processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode* node, const aiScene* scene)
{
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
        meshes.push_back(processMesh(scene->mMeshes[node->mMeshes[i]], scene));

    for (unsigned int i = 0; i < node->mNumChildren; i++)
        processNode(node->mChildren[i], scene);
}

cMesh Model::processMesh(aiMesh* mesh, const aiScene* scene)
{
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
        Vertex vertex;
        glm::vec3 vector;

        // Positions
        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].y;
        vector.z = mesh->mVertices[i].z;
        vertex.Position = vector;

        // Normals
        if (mesh->HasNormals()) {
            vector.x = mesh->mNormals[i].x;
            vector.y = mesh->mNormals[i].y;
            vector.z = mesh->mNormals[i].z;
            vertex.Normal = vector;
        }

        // Texture Coordinates
        if (mesh->mTextureCoords[0]) {
            glm::vec2 vec;
            vec.x = mesh->mTextureCoords[0][i].x;
            vec.y = mesh->mTextureCoords[0][i].y;
            vertex.TexCoords = vec;

            // Tangent
            vector.x = mesh->mTangents[i].x;
            vector.y = mesh->mTangents[i].y;
            vector.z = mesh->mTangents[i].z;
            vertex.Tangent = vector;

            // Bitangent
            vector.x = mesh->mBitangents[i].x;
            vector.y = mesh->mBitangents[i].y;
            vector.z = mesh->mBitangents[i].z;
            vertex.Bitangent = vector;
        }
        else {
            vertex.TexCoords = glm::vec2(0.0f, 0.0f);
        }

        vertices.push_back(vertex);
    }

    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }

    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

    // Load diffuse maps
    std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
    textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

    // Load specular maps
    std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
    textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

    // Load normal maps
    std::vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
    textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());

    // Load height maps
    std::vector<Texture> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
    textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

    return cMesh(vertices, indices, textures);
}

std::vector<Texture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName) {
    std::vector<Texture> textures;
    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
    {
        aiString str;
        mat->GetTexture(type, i, &str);
        bool skip = false;
        for (unsigned int j = 0; j < textures_loaded.size(); j++)
        {
            if (std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0)
            {
                textures.push_back(textures_loaded[j]);
                skip = true;
                break;
            }
        }
        if (!skip)
        {
            Texture texture;
            texture.id = TextureFromFile(str.C_Str(), this->directory);
            texture.type = typeName;
            texture.path = str.C_Str();
            textures.push_back(texture);
            textures_loaded.push_back(texture);
        }
    }
    return textures;
}

unsigned int TextureFromFile(const char* path, const std::string& directory, bool gamma)
{
    std::string filename = std::string(path);
    filename = directory + '/' + filename;

    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
    if (data) {
        GLenum format{};
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}