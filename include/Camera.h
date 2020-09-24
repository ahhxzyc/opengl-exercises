#pragma once

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Camera {

private:
    glm::vec3 pos = glm::vec3(0.f);
    glm::vec3 up = glm::vec3(0.f, 1.f, 0.f);
    glm::vec3 front = glm::vec3(0.f, 0.f, -1.f);
    float pitch = 0.f;
    float yaw = 270.f;

    float moveSpeed = 0.05f;
    float mouseSensitivity = 0.05f;

public:
    
    void processKeyboardInput(GLFWwindow *window) {
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
            pos += front * moveSpeed;
        }
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
            pos += -front * moveSpeed;
        }
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
            pos += glm::normalize(glm::cross(up, front)) * moveSpeed;
        }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
            pos += -glm::normalize(glm::cross(up, front)) * moveSpeed;
        }
    }

    void processMouseInput(float diffX, float diffY) {
        // 利用差值更新pitch和yaw
        pitch += -diffY * mouseSensitivity;
        yaw += diffX * mouseSensitivity;

        // 限制pitch范围
        if (pitch > 89.f) pitch = 89.f;
        if (pitch < -89.f) pitch = -89.f;

        // 用pitch和yaw的值更新front
        front.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
        front.y = sin(glm::radians(pitch));
        front.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
    }

    glm::mat4 viewMatrix() const {
        return glm::lookAt(pos, pos + front, up);
    }

    glm::vec3 getPos() const {return pos;    }
};