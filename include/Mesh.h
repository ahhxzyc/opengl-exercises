#pragma once

#include <glad/glad.h>

#include <vector>
#include <string>

#include <Shader.h>

#include <glm/glm.hpp>

struct Vertex {
    glm::vec3 pos;
    glm::vec3 normal;
    glm::vec2 texCoords;
};

struct Texture {
    unsigned int id;
    std::string type;   // diffuse or specular
};


class Mesh {
public:
    std::vector<Vertex> vertices;
    std::vector<Texture> textures;
    std::vector<unsigned int> indices;

    Mesh(std::vector<Vertex> &vertices_,
        std::vector<Texture> &textures_,
        std::vector<unsigned int> &indices_) 
        : vertices(vertices_), textures(textures_), indices(indices_)
    {
        setupMesh();
    }

    void draw(Shader& shader) const  {
        
        shader.use();

        // 绑定各个texture
        int diffuseCnt = 0;
        int specularCnt = 0;

        for (int i = 0; i < textures.size(); i++) {
            auto &tex = textures[i];
            glActiveTexture(i);
            glBindTexture(GL_TEXTURE_2D, tex.id);
            
            if (tex.type == "diffuse") {
                shader.setInt(std::string("textureDiffuse") + std::to_string(diffuseCnt ++ ), i);
            } else if (tex.type == "specular") {
                shader.setInt(std::string("textureSpecular") + std::to_string(specularCnt ++ ), i);
            }
        }

        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }


    // 设置Mesh的数据缓冲
    void setupMesh() {
        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);
        glGenBuffers(1, &ebo);

        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, pos));
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);

        glBindVertexArray(0);
    }


private:
    unsigned int vao, vbo, ebo;
    
};