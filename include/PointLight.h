#pragma once

#include <string>

#include <Shader.h>
#include <glm/glm.hpp>

class PointLight {

private:
    glm::vec3 position;
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;

public:
    PointLight(const glm::vec3& pos, const glm::vec3& amb, const glm::vec3& diff, const glm::vec3& spec)
        : position(pos), ambient(amb), diffuse(diff), specular(spec) { }

    void setUniform(Shader& shader, const std::string &varname) const {
        shader.use();
        shader.setVec3(varname + ".pos", position);
        shader.setVec3(varname + ".ambient", ambient);
        shader.setVec3(varname + ".diffuse", diffuse);
        shader.setVec3(varname + ".specular", specular);
    }
};