#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include <Shader.h>
#include <Camera.h>
#include <PointLight.h>
#include "Scene.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


const int screenWidth = 800;
const int screenHeight = 800;

const int NUM_POINT_LIGHTS = 3;

Camera cam;

void processInput(GLFWwindow* window) {
    
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, 1);
    }
    
}

void mouseCallback(GLFWwindow* window, double x, double y) {

    static float prevX = screenWidth / 2.f;
    static float prevY = screenHeight / 2.f;

    static bool firstTimeCalled = true;

    float diffX = x - prevX, diffY = y - prevY;
    prevX = x;
    prevY = y;

    // 如果是第一次调用，为prevX、prevY赋值后立即return，以此避免视角的突变
    if (firstTimeCalled) {
        firstTimeCalled = false;
        return;
    }

    cam.processMouseInput(diffX, diffY);
}

unsigned int loadTexture(const std::string& path) {
    unsigned int texture;
    glGenTextures(1, &texture);

    int imgWidth, imgHeight, imgChannels;
    unsigned char* imgBuffer = stbi_load(path.c_str(), &imgWidth, &imgHeight, &imgChannels, 0);

    if (imgBuffer) {
        int format;
        if (imgChannels == 1)       format = GL_RED;
        else if (imgChannels == 3)  format = GL_RGB;
        else                        format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(GL_TEXTURE_2D, 0, format, imgWidth, imgHeight, 0, format, GL_UNSIGNED_BYTE, imgBuffer);
        glGenerateMipmap(GL_TEXTURE_2D);

        stbi_image_free(imgBuffer);
    } else {
        std::cout << "ERROR: failed to load texture img.\n";
    }

    return texture;
}

int main() {

    // **********************************初始化**********************************

    // glfw在使用前必须初始化
    glfwInit();
    // 告诉glfw我们使用的opengl版本
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // 明确指定使用opengl的core profile
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // 创建窗口对象
    GLFWwindow *window = glfwCreateWindow(screenWidth, screenHeight, "the window", NULL, NULL);
    if (window == NULL) {
        std::cout << "failed to create window" << std::endl;
        return -1;
    }
    // 创建opengl context
    glfwMakeContextCurrent(window);

    // 初始化glad
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // 限制鼠标,注册鼠标的回调函数
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouseCallback);

    // **********************************载入场景*********************************

    Scene scene("resources/nanosuit/nanosuit.obj");

    // **********************************准备shaders**********************************

    Shader objectShader("src/objectVertexShader.glsl", "src/objectFragShader.glsl");

    // **********************************绘制**********************************


    // 准备光源
    glm::vec3 lightPositions[NUM_POINT_LIGHTS] = {
        glm::vec3(0.f, 10.f, 5.f),
        glm::vec3(0.f, 15.f, 5.f),
        glm::vec3(0.f, 20.f, 5.f)
    };
    
    std::vector<PointLight> lights;
    for (const auto& pos : lightPositions) {
        PointLight light(
            pos,
            glm::vec3(.2f, .2f, .2f),
            glm::vec3(.5f, .5f, .5f),
            glm::vec3(1.f, 1.f, 1.f)
        );
        lights.push_back(light);
    }
    for (int i = 0; i < NUM_POINT_LIGHTS; i ++ ) {
        lights[i].setUniform(objectShader, std::string("lights[") + std::to_string(i) + "]");
    }

    // 启用z缓冲
    glEnable(GL_DEPTH_TEST);


    while (!glfwWindowShouldClose(window)) {

        // 处理输入
        processInput(window);
        cam.processKeyboardInput(window);

        // 清除帧缓冲
        glClearColor(.1f, .1f, .1f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // 计算view和projection矩阵
        glm::mat4 view = cam.viewMatrix();
        glm::mat4 projection = glm::perspective(glm::radians(45.f), (float)screenWidth / screenHeight, .1f, 100.0f);
        glm::mat4 model = glm::mat4(1.f);

        objectShader.setFloat("constant", 1.f);
        objectShader.setFloat("linear", .09f);
        objectShader.setFloat("quadratic", .032f);

        objectShader.setMat4("view", view);
        objectShader.setMat4("projection", projection);
        objectShader.setMat4("model", model);

        scene.draw(objectShader);
        
        glfwSwapBuffers(window);

        // 更新窗口状态，检查事件，调用对应的callbacks
        glfwPollEvents();
    }

    // 使用结束后释放glfw的资源
    glfwTerminate();
    return 0;
}