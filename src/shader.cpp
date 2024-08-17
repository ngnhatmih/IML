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

        }

        fFile.close();

        const char *vertexShaderSource = vSource.c_str(); 
        const char *fragmentShaderSource = fSource.c_str();

        vertexShader = glCreateShader(GL_VERTEX_SHADER);
        fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

        glShaderSource(vertexShader, 1, &vertexShaderSource, 0);
        glShaderSource(fragmentShader, 1, &fragmentShaderSource, 0);

        int success;
        char infoLog[512] = "";

        glCompileShader(vertexShader);
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(vertexShader, 512, 0, infoLog);
            SDL_Log("ERROR:VERTEX::COMPILE: %s", infoLog);
        }

        glCompileShader(fragmentShader);
        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(fragmentShader, 512, 0, infoLog);
            SDL_Log("ERROR:FRAGMENT::COMPILE: %s", infoLog);
        }

        shaderProgram = glCreateProgram();
        glAttachShader(shaderProgram, vertexShader);
        glAttachShader(shaderProgram, fragmentShader);
        glLinkProgram(shaderProgram);

        
    } else {
        SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "ERROR:LOAD_SHADER");
    }
}

GLuint Shader::getProgramID() {
    return shaderProgram;
}

void Shader::useProgram() {
    if (shaderProgram)
        glUseProgram(shaderProgram);
}

void Shader::deleteProgram() {
    if (shaderProgram)
        glDeleteProgram(shaderProgram);
}


void Shader::setUniform(const char *name, int value) {
    int uniform_location = glGetUniformLocation(shaderProgram, name);
    glUniform1i(uniform_location, value);
}

void Shader::setUniform(const char *name, float value) {
    int uniform_location = glGetUniformLocation(shaderProgram, name);
    glUniform1f(uniform_location, value);
}

void Shader::setUniform(const char *name, bool value) {
    setUniform(name, (int) value);
}

void Shader::setUniform(const char *name, glm::vec3 value) {
    int uniform_location = glGetUniformLocation(shaderProgram, name);
    glUniform3fv(uniform_location, 1, glm::value_ptr(value));
}
