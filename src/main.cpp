#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>


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
    GLFWwindow *window = glfwCreateWindow(400, 400, "the window", NULL, NULL);
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

    // **********************************准备VAO**********************************

    float data[] = {
        // 坐标数据              顶点颜色
        -0.5f, -0.5f, 0.f,      1.f, 0.f, 0.f,
        .5f, -.5f, 0.f,         0.f, 1.f, 0.f,
        0.f, 0.5f, 0.f,         0.f, 0.f, 1.f
    };

    // 生成一个vertex array
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    // 绑定这个vertex array，后续对buffer的绑定和配置都会存储在绑定的vertex array中
    glBindVertexArray(VAO);


    // 生成一个buffer用来存放点的坐标数据
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    // 绑定这个buffer
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // 将顶点数据复制到绑定的buffer中
    glBufferData(GL_ARRAY_BUFFER, sizeof data, data, GL_STATIC_DRAW);
    // 指定数据的解读方式
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (void*)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (void*)(3 * sizeof(float)));
    // 激活attribute
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);



    // **********************************准备shaders**********************************

    const char* vs = "# version 330 core\n"
        "layout(location = 0) in vec3 aPos;\n"
        "layout(location = 1) in vec3 aColor;\n"
        "out vec3 bColor;\n"
        "void main() {\n"
        "gl_Position = vec4(aPos, 1.f);\n"
        "bColor = aColor;\n"
        "}\n";
    const char* fs = "# version 330 core\n"
        "in vec3 bColor;\n"
        "out vec4 outColor;\n"
        "void main() {\n"
        "outColor = vec4(bColor, 1.f);\n"
        "}\n";

    // 创建一个opengl的shader
    unsigned int vso = glCreateShader(GL_VERTEX_SHADER);
    unsigned int fso = glCreateShader(GL_FRAGMENT_SHADER);
    // 指定shader的源代码
    glShaderSource(vso, 1, &vs, NULL);
    glShaderSource(fso, 1, &fs, NULL);
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
    unsigned int pg = glCreateProgram();
    // 绑定两个可编程shader
    glAttachShader(pg, vso);
    glAttachShader(pg, fso);
    // 链接program
    glLinkProgram(pg);
    glGetProgramiv(pg, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(pg, sizeof log, NULL, log);
        std::cout << "ERROR: LINKING\n" << log << std::endl;
    }


    // **********************************绘制**********************************

    // 指定绘制对象（vao)，指定绘制管线（program）
    glBindVertexArray(VAO);
    glUseProgram(pg);
    

    while (!glfwWindowShouldClose(window)) {
        // 清除帧缓冲
        glClear(GL_COLOR_BUFFER_BIT);

        // 绘制
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glfwSwapBuffers(window);

        // 更新窗口状态，检查事件，调用对应的callbacks
        glfwPollEvents();
    }

    // 使用结束后释放glfw的资源
    glfwTerminate();
    return 0;
}