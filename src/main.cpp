#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>


int main() {

    // **********************************��ʼ��**********************************

    // glfw��ʹ��ǰ�����ʼ��
    glfwInit();
    // ����glfw����ʹ�õ�opengl�汾
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // ��ȷָ��ʹ��opengl��core profile
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // �������ڶ���
    GLFWwindow *window = glfwCreateWindow(400, 400, "the window", NULL, NULL);
    if (window == NULL) {
        std::cout << "failed to create window" << std::endl;
        return -1;
    }
    // ����opengl context
    glfwMakeContextCurrent(window);

    // ��ʼ��glad
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // **********************************׼��VAO**********************************

    float data[] = {
        // ��������              ������ɫ
        -0.5f, -0.5f, 0.f,      1.f, 0.f, 0.f,
        .5f, -.5f, 0.f,         0.f, 1.f, 0.f,
        0.f, 0.5f, 0.f,         0.f, 0.f, 1.f
    };

    // ����һ��vertex array
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    // �����vertex array��������buffer�İ󶨺����ö���洢�ڰ󶨵�vertex array��
    glBindVertexArray(VAO);


    // ����һ��buffer������ŵ����������
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    // �����buffer
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // ���������ݸ��Ƶ��󶨵�buffer��
    glBufferData(GL_ARRAY_BUFFER, sizeof data, data, GL_STATIC_DRAW);
    // ָ�����ݵĽ����ʽ
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (void*)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (void*)(3 * sizeof(float)));
    // ����attribute
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);



    // **********************************׼��shaders**********************************

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

    // ����һ��opengl��shader
    unsigned int vso = glCreateShader(GL_VERTEX_SHADER);
    unsigned int fso = glCreateShader(GL_FRAGMENT_SHADER);
    // ָ��shader��Դ����
    glShaderSource(vso, 1, &vs, NULL);
    glShaderSource(fso, 1, &fs, NULL);
    // ����shader
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

    // ����һ��opengl��program
    unsigned int pg = glCreateProgram();
    // �������ɱ��shader
    glAttachShader(pg, vso);
    glAttachShader(pg, fso);
    // ����program
    glLinkProgram(pg);
    glGetProgramiv(pg, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(pg, sizeof log, NULL, log);
        std::cout << "ERROR: LINKING\n" << log << std::endl;
    }


    // **********************************����**********************************

    // ָ�����ƶ���vao)��ָ�����ƹ��ߣ�program��
    glBindVertexArray(VAO);
    glUseProgram(pg);
    

    while (!glfwWindowShouldClose(window)) {
        // ���֡����
        glClear(GL_COLOR_BUFFER_BIT);

        // ����
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glfwSwapBuffers(window);

        // ���´���״̬������¼������ö�Ӧ��callbacks
        glfwPollEvents();
    }

    // ʹ�ý������ͷ�glfw����Դ
    glfwTerminate();
    return 0;
}