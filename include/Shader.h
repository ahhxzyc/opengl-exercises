#pragma once

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>

#include <glad/glad.h>


#include <direct.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader {

private:
    int programId;

public:
    Shader(const char* vertexPath, const char* fragPath) {


        std::string vertexString, fragString;

        std::ifstream vertexFile, fragFile;
        vertexFile.exceptions(std::fstream::failbit | std::fstream::badbit);
        fragFile.exceptions(std::fstream::failbit | std::fstream::badbit);


        // 将vertex shader和fragment shader的源码读入string中
        try {
            vertexFile.open(vertexPath);
            fragFile.open(fragPath);

            std::stringstream vertexStream, fragStream;
            vertexStream << vertexFile.rdbuf();
            fragStream << fragFile.rdbuf();
            vertexFile.close();
            fragFile.close();

            vertexString = vertexStream.str();
            fragString = fragStream.str();
        } catch (std::ifstream::failure e) {
            std::cout << "ERROR: failed to read GLSL code\n" << std::endl;
        }

        // 将glsl代码从string迁移到c字符串中
        const char *vertexCString = vertexString.c_str();
        const char* fragCString = fragString.c_str();

        // 创建shader
        unsigned int vso = glCreateShader(GL_VERTEX_SHADER);
        unsigned int fso = glCreateShader(GL_FRAGMENT_SHADER);
        // 指定shader的源代码
        glShaderSource(vso, 1, &vertexCString, NULL);
        glShaderSource(fso, 1, &fragCString, NULL);
        // 编译shader
        int success;
        char log[512];
        glCompileShader(vso);
        glGetShaderiv(vso, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(vso, sizeof log, NULL, log);
            std::cout << "ERROR: VERTEX SHADER COMPILATION\n" << log << std::endl;
        }
        glCompileShader(fso);
        glGetShaderiv(fso, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(fso, sizeof log, NULL, log);
            std::cout << "ERROR: FRAG SHADER COMPILATION\n" << log << std::endl;
        }


        // 创建一个opengl的program
        programId = glCreateProgram();
        // 绑定两个可编程shader
        glAttachShader(programId, vso);
        glAttachShader(programId, fso);
        // 链接program
        glLinkProgram(programId);
        glGetProgramiv(programId, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(programId, sizeof log, NULL, log);
            std::cout << "ERROR: LINKING\n" << log << std::endl;
        }
    }

    void use() const {
        glUseProgram(programId);
    }

    int id() const {return programId;}

    void setFloat(const std::string &varname, float value) const {
        glUniform1f(glGetUniformLocation(programId, varname.c_str()), value);
    }

    void setVec3(const std::string &varname, float x, float y, float z) const {
        glUniform3f(glGetUniformLocation(programId, varname.c_str()), x, y, z);
    }
    void setVec3(const std::string &varname, const glm::vec3 &vec) const {
        glUniform3f(glGetUniformLocation(programId, varname.c_str()), vec.x, vec.y, vec.z);
    }

    void setInt(const std::string &varname, int value) const {
        glUniform1i(glGetUniformLocation(programId, varname.c_str()), value);
    }

    void setMat4(const std::string &varname, const glm::mat4 &mat) const {
        glUniformMatrix4fv(glGetUniformLocation(programId, varname.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
    }
};