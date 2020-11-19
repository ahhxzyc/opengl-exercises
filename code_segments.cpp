void checkCompile(GLuint shaderId, int shaderType) {
    int success;
    char log[512];
    if (shaderType == 0) {
        glGetProgramiv(shaderId, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shaderId, sizeof log, NULL, log);
            std::cout << "ERROR: LINK\n" << log << std::endl;
        }
    } else {
        char *type;
        if (shaderType == 1)    type = "VERTEX";
        else                    type = "FRAGMENT";
        glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shaderId, sizeof log, NULL, log);
            std::cout << "ERROR: " << type << " SHADER\n" << log << std::endl;
        }
    }
}

void drawLine(float *p, float *q) {
    // Vertex shader
    GLuint vshader = glCreateShader(GL_VERTEX_SHADER);
    const char *vsource = "#version 430 core\n"
        "layout (location = 0) in vec3 in_pos;\n"
        "void main() {\n"
        "   gl_Position = vec4(in_pos, 1.0);\n"
        "}\n";
    glShaderSource(vshader, 1, &vsource, NULL);
    glCompileShader(vshader);
    checkCompile(vshader, 1);
    // Fragment shader
    GLuint fshader = glCreateShader(GL_FRAGMENT_SHADER);
    const char *fsource = "#version 430 core\n"
        "out vec4 color;\n"
        "void main() {\n"
        "   color = vec4(1.0f, 1.0f, 1.0f, 1.0f);\n"
        "}\n";
    glShaderSource(fshader, 1, &fsource, NULL);
    glCompileShader(fshader);
    checkCompile(fshader, 2);
    // Shader program
    GLuint program = glCreateProgram();
    glAttachShader(program, vshader);
    glAttachShader(program, fshader);
    glLinkProgram(program);
    checkCompile(program, 0);
    // VAO
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    // VBO
    GLuint vbuffer;
    glGenBuffers(1, &vbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vbuffer);
    GLfloat positions[] = {p[0], p[1], p[2], q[0], q[1], q[2]};
    glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);
    // Fill vao
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // Draw
    glBindVertexArray(vao);
    glUseProgram(program);
    glPointSize(5.f);
    glDrawArrays(GL_LINE_STRIP, 0, 2);
}
