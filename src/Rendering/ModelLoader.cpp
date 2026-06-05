#include "Rendering/ModelLoader.h"

#include <iostream>
#include <memory>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include "Rendering/Model.h"
#include "Rendering/Texture2D.h"

bool ModelLoader::LoadObj(const std::string& path, Model& model, Shader* shader, Texture2D* fallbackTexture) {
    Assimp::Importer importer;

    const aiScene* scene = importer.ReadFile(
        path,
        aiProcess_Triangulate |
        aiProcess_FlipUVs |
        aiProcess_GenNormals |
        aiProcess_JoinIdenticalVertices
    );

    if (scene == nullptr || scene->mRootNode == nullptr) {
        std::cerr << "Failed to load model: " << path << "\n";
        return false;
    }

    const std::string directory = GetDirectory(path);

    for (unsigned int meshIndex = 0; meshIndex < scene->mNumMeshes; ++meshIndex) {
        aiMesh* sourceMesh = scene->mMeshes[meshIndex];

        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;

        for (unsigned int i = 0; i < sourceMesh->mNumVertices; ++i) {
            Vertex vertex{};
            vertex.position = { sourceMesh->mVertices[i].x, sourceMesh->mVertices[i].y, sourceMesh->mVertices[i].z };
            vertex.normal = { sourceMesh->mNormals[i].x, sourceMesh->mNormals[i].y, sourceMesh->mNormals[i].z };

            if (sourceMesh->HasTextureCoords(0)) {
                vertex.texCoord = { sourceMesh->mTextureCoords[0][i].x, sourceMesh->mTextureCoords[0][i].y };
            }

            vertices.push_back(vertex);
        }

        for (unsigned int i = 0; i < sourceMesh->mNumFaces; ++i) {
            const aiFace& face = sourceMesh->mFaces[i];
            for (unsigned int j = 0; j < face.mNumIndices; ++j) {
                indices.push_back(face.mIndices[j]);
            }
        }

        ModelMesh modelMesh;
        modelMesh.mesh.Create(vertices, indices);
        modelMesh.material.SetShader(shader);
        modelMesh.material.SetTexture(fallbackTexture);
        modelMesh.material.SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });

        model.AddMesh(std::move(modelMesh));
    }

    return true;
}

std::string ModelLoader::GetDirectory(const std::string& path) const {
    const std::size_t slash = path.find_last_of("/\\");

    if (slash == std::string::npos) {
        return ".";
    }

    return path.substr(0, slash);
}