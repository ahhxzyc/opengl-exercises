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
    // Fragment shader
    GLuint fshader = glCreateShader(GL_FRAGMENT_SHADER);
    const char *fsource = "#version 430 core\n"
        "out vec4 color;\n"
        "void main() {\n"
        "   color = vec4(1.0f, 1.0f, 1.0f, 1.0f);\n"
        "}\n";
    glShaderSource(fshader, 1, &fsource, NULL);
    glCompileShader(fshader);
    // Shader program
    GLuint program = glCreateProgram();
    glAttachShader(program, vshader);
    glAttachShader(program, fshader);
    glLinkProgram(program);
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
