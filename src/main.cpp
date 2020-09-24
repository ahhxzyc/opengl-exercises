#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include <Shader.h>
#include <Camera.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>


const int screenWidth = 800;
const int screenHeight = 800;

glm::vec3 lightPos = glm::vec3(1.2f, 1.0f, 2.0f);
glm::vec3 objectColor = glm::vec3(1.0f, 0.5f, 0.31f);

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

    // **********************************准备VAO EBO**********************************

    // 盒子的坐标和normal
    float data[] = {
       -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
    };


    // 设置物体的VAO
    unsigned int objectVao;
    glGenVertexArrays(1, &objectVao);
    glBindVertexArray(objectVao);

    unsigned int objectVbo;
    glGenBuffers(1, &objectVbo);
    glBindBuffer(GL_ARRAY_BUFFER, objectVbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof data, data, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (void*)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);


    // 设置光源的VAO
    unsigned int lightVao;
    glGenVertexArrays(1, &lightVao);
    glBindVertexArray(lightVao);

    glBindBuffer(GL_ARRAY_BUFFER, objectVbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (void*)0);
    glEnableVertexAttribArray(0);



    // **********************************准备纹理**********************************



    // **********************************准备shaders**********************************

    Shader objectShader("src/objectVertexShader.glsl", "src/objectFragShader.glsl");
    Shader lightShader("src/lightVertexShader.glsl", "src/lightFragShader.glsl");


    // **********************************绘制**********************************


    // 启用z缓冲
    glEnable(GL_DEPTH_TEST);


    while (!glfwWindowShouldClose(window)) {

        // 处理输入
        processInput(window);
        cam.processKeyboardInput(window);

        // 清除帧缓冲
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // 计算view和projection矩阵
        glm::mat4 view = cam.viewMatrix();
        glm::mat4 projection = glm::perspective(glm::radians(45.f), (float)screenWidth / screenHeight, .1f, 100.0f);


        // 绘制箱子
        glm::mat4 objectModel = glm::mat4(1.f);
        objectModel = glm::rotate(objectModel, (float)glfwGetTime(), glm::vec3(0.f, 1.f, 0.f));
        objectShader.use();
        objectShader.setVec3("lightColor", 1.f, 1.f, 1.f);
        objectShader.setVec3("objectColor", objectColor);
        objectShader.setVec3("lightPos", lightPos);
        objectShader.setVec3("viewPos", cam.getPos());
        objectShader.setMat4("model", objectModel);
        objectShader.setMat4("view", view);
        objectShader.setMat4("projection", projection);
        glBindVertexArray(objectVao);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // 绘制光源
        glm::mat4 lightModel = glm::mat4(1.f);
        lightModel = glm::translate(lightModel, lightPos);
        lightModel = glm::scale(lightModel, glm::vec3(.2f));
        lightShader.use();
        lightShader.setMat4("model", lightModel);
        lightShader.setMat4("view", view);
        lightShader.setMat4("projection", projection);
        glBindVertexArray(lightVao);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glfwSwapBuffers(window);


        // 更新窗口状态，检查事件，调用对应的callbacks
        glfwPollEvents();
    }

    // 使用结束后释放glfw的资源
    glfwTerminate();
    return 0;
}