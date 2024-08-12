#include "shader.h"

Shader::Shader(const char *vertex_path, const char *fragment_path) {
    // Load
    std::string vSource;
    std::string fSource;
    std::ifstream vFile(vertex_path);
    std::ifstream fFile(fragment_path);
 
    if (vFile.is_open() && fFile.is_open()) {
        while (!vFile.eof()) {
            char buf[5000];
            std::string line;
            vFile.getline(buf, 5000);
            line = buf;
            vSource += line + "\n";
        }

        vFile.close();

        while (!fFile.eof()) {
            char buf[5000];
            std::string line;
            fFile.getline(buf, 5000);
            line = buf;
            fSource += line + "\n";

            const char *vertexShaderSource = vSource.c_str(); 
            const char *fragmentShaderSource = fSource.c_str();

            vertexShader = glCreateShader(GL_VERTEX_SHADER);
            fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

            glShaderSource(vertexShader, 1, &vertexShaderSource, 0);
            glShaderSource(fragmentShader, 1, &fragmentShaderSource, 0);

            glCompileShader(vertexShader);
            glCompileShader(fragmentShader);

            shaderProgram = glCreateProgram();
            glAttachShader(shaderProgram, vertexShader);
            glAttachShader(shaderProgram, fragmentShader);
            glLinkProgram(shaderProgram);
        }

        fFile.close();
    } else {
        SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "ERROR:LOAD_SHADER");
    }
}

void Shader::useProgram() {
    if (shaderProgram)
        glUseProgram(shaderProgram);
}

void Shader::deleteProgram() {
    if (shaderProgram)
        glDeleteProgram(shaderProgram);
}

void Shader::setUniform(const char *name, float value) {
    int uniform_location = glGetUniformLocation(shaderProgram, name);
    glUniform1f(uniform_location, value);
}