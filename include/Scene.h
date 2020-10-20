#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <string>
#include <vector>

#include "Mesh.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

class Scene {

public:
    Scene(const std::string &path) {
        loadScene(path);
    }

    // ��������mesh��draw����
    void draw(Shader &shader) const {
        for (auto& m : meshes) {
            m.draw(shader);
        }
    }


private :
    // һϵ��Ҫdraw��mesh
    std::vector<Mesh> meshes;

    void loadScene(const std::string& path) {

        Assimp::Importer importer;
        const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate);

        processNode(scene->mRootNode, scene);
    }

    // assimp�������洢Ϊ���νṹ���ݹ鴦����
    void processNode(aiNode* node, const aiScene* scene) {
        // ��������ڵ����е�mesh
        for (int i = 0; i < node->mNumMeshes; i++) {
            processMesh(scene->mMeshes[node->mMeshes[i]], scene);
        }
        // �ݹ�������еĺ��ӽڵ�
        for (int i = 0; i < node->mNumChildren; i++) {
            processNode(node->mChildren[i], scene);
        }
    }

    void processMesh(const aiMesh* mesh, const aiScene *scene) {
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        std::vector<Texture> textures;

        // ��������
        for (int i = 0; i < mesh->mNumVertices; i++) {
            Vertex vertex;

            const auto &vert = mesh->mVertices[i];
            vertex.pos = glm::vec3(vert.x, vert.y, vert.z);

            if (mesh->HasNormals()) {
                const auto& norm = mesh->mNormals[i];
                vertex.normal = glm::vec3(norm.x, norm.y, norm.z);
            }

            if (mesh->mTextureCoords[0]) {
                const auto& texc = mesh->mTextureCoords[0][i];
                vertex.texCoords = glm::vec2(texc.x, texc.y);
            } else {
                vertex.texCoords = glm::vec2(0.f, 0.f);
            }
            
            vertices.push_back(vertex);
        }

        // ����
        for (int i = 0; i < mesh->mNumFaces; i ++) {
            const auto &face = mesh->mFaces[i];
            for (int j = 0; j < face.mNumIndices; j++) {
                indices.push_back(face.mIndices[j]);
            }
        }

        // ���� һ��meshֻ��һ��material
        if (mesh->mMaterialIndex >= 0) {
            aiMaterial *mat = scene->mMaterials[mesh->mMaterialIndex];
            // �����Լ�д�ĺ�������texture vector
            std::vector<Texture> diffuseTextures = loadMaterialTextures(mat, aiTextureType_DIFFUSE, "diffuse");
            std::vector<Texture> specularTextures = loadMaterialTextures(mat, aiTextureType_SPECULAR, "specular");
            textures.insert(textures.end(), diffuseTextures.begin(), diffuseTextures.end());
            textures.insert(textures.end(), specularTextures.begin(), specularTextures.end());
        }

        Mesh tempMesh(vertices, textures, indices);
        meshes.push_back(tempMesh);
    }

    // ����material��ĳһ��texture��diffuse��specular���������Ǵ���vector����
    std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, const std::string& typeName) const {
        std::vector<Texture> v;
        for (int i = 0; i < mat->GetTextureCount(type); i++) {
            // ȡ�����texture��·��
            aiString s;
            mat->GetTexture(type, i, &s);

            Texture tex;
            tex.id = getTextureAt(s.C_Str());
            tex.type = typeName;
            
            v.push_back(tex);
        }

        return v;
    }

    // ����ͼƬ��·������һ��texture����������id
    unsigned int getTextureAt(const std::string& path) const {

        unsigned int texture;

        int height, width, channels;
        stbi_set_flip_vertically_on_load(true);
        unsigned char *imgData = stbi_load(("resources/nanosuit/" + path).c_str(), &height, &width, &channels, 0);

        if (imgData) {
            int format;
            if (channels == 1)      format = GL_RED;
            else if (channels == 3) format = GL_RGB;
            else                    format = GL_RGBA;

            glGenTextures(1, &texture);
            glBindTexture(GL_TEXTURE_2D, texture);
            glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, imgData);
            glGenerateMipmap(GL_TEXTURE_2D);

        } else {
            std::cout << "ERROR: failed to load texture image: " << path << std::endl;
            exit(0);
        }

        stbi_image_free(imgData);

        return texture;
    }
};