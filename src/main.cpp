#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include <Shader.h>
#include <Camera.h>
#include <PointLight.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>


const int screenWidth = 800;
const int screenHeight = 800;

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

    // **********************************准备VAO EBO**********************************

    // 盒子的坐标和normal
    float data[] = {
        // positions          // normals           // texture coords
         -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
          0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
          0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
          0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
         -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
         -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

         -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
          0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
          0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
          0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
         -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
         -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

         -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
         -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
         -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
         -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

          0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
          0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
          0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
          0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
          0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
          0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

         -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
          0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
          0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
          0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
         -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
         -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

         -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
          0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
          0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
          0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
         -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
         -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
    };

    // 多个盒子的位置
    glm::vec3 cubePositions[] = {
        glm::vec3(0.0f,  0.0f,  0.0f),
        glm::vec3(2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3(2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3(1.3f, -2.0f, -2.5f),
        glm::vec3(1.5f,  2.0f, -2.5f),
        glm::vec3(1.5f,  0.2f, -1.5f),
        glm::vec3(-1.3f,  1.0f, -1.5f)
    };

    // 多个点光源
    glm::vec3 pointLightPositions[] = {
        glm::vec3(1.2f, 1.0f, 2.0f),
        glm::vec3(0.f, 5.f, -6.f),
        glm::vec3(1.f, -2.f, 0.f),
        glm::vec3(0.f, 3.f, 1.f)
    };

    // 设置物体的VAO
    unsigned int objectVao;
    glGenVertexArrays(1, &objectVao);
    glBindVertexArray(objectVao);

    unsigned int objectVbo;
    glGenBuffers(1, &objectVbo);
    glBindBuffer(GL_ARRAY_BUFFER, objectVbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof data, data, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(3 * sizeof(float)));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);


    // 设置光源的VAO
    unsigned int lightVao;
    glGenVertexArrays(1, &lightVao);
    glBindVertexArray(lightVao);

    glBindBuffer(GL_ARRAY_BUFFER, objectVbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)0);
    glEnableVertexAttribArray(0);



    // **********************************准备纹理**********************************

    unsigned int texture = loadTexture("resources/container.png");
    unsigned int textureSpec = loadTexture("resources/container_specular.png");

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, textureSpec);

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
        glClearColor(.1f, .1f, .1f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // 计算view和projection矩阵
        glm::mat4 view = cam.viewMatrix();
        glm::mat4 projection = glm::perspective(glm::radians(45.f), (float)screenWidth / screenHeight, .1f, 100.0f);


        // 绘制箱子
        objectShader.use();

        for (int i = 0; i < 4; i++) {
            glm::vec3 white(1.f);
            PointLight light(pointLightPositions[i], white * .1f, white * .3f, white);
            light.setUniform(objectShader, std::string("lights") + "[" + std::to_string(i) + "]");
        }
        glm::vec3 lightColor = glm::vec3(1.f);


        objectShader.setFloat("constant", 1.f);
        objectShader.setFloat("linear", .09f);
        objectShader.setFloat("quadratic", .032f);

        objectShader.setInt("material.diffuse", 0);
        objectShader.setInt("material.specular", 1);
        objectShader.setFloat("material.shininess", 128.0f);

        objectShader.setMat4("view", view);
        objectShader.setMat4("projection", projection);
        
        glBindVertexArray(objectVao);

        for (int i = 0; i < 10; i ++ ) {
            glm::mat4 objectModel = glm::mat4(1.f);
            objectModel = glm::translate(objectModel, cubePositions[i]);
            float angle = 20.f * i;
            objectModel = glm::rotate(objectModel, angle, glm::vec3(0.f, 1.f, 0.f));
            objectShader.setMat4("model", objectModel);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        // 绘制光源
        glBindVertexArray(lightVao);
        lightShader.use();
        for (int i = 0; i < 4; i++) {
            glm::mat4 model = glm::mat4(1.f);
            model = glm::translate(model, pointLightPositions[i]);
            model = glm::scale(model, glm::vec3(.2f));
            lightShader.setMat4("model", model);
            lightShader.setMat4("view", view);
            lightShader.setMat4("projection", projection);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        
        

        glfwSwapBuffers(window);

        // 更新窗口状态，检查事件，调用对应的callbacks
        glfwPollEvents();
    }

    // 使用结束后释放glfw的资源
    glfwTerminate();
    return 0;
}