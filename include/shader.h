#ifndef SHADER_H
#define SHADER_H
#include <SDL3/SDL.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <string>
#include <fstream>

class Shader {
public:
    Shader(const char *vertex_path, const char *fragment_path); 
    Shader(const char *vertex_path, const char *geometry_path, const char *fragment_path);

    void setUniform(const char *name, int value);
    void setUniform(const char *name, size_t value);
    void setUniform(const char *name, float value);
    void setUniform(const char *name, bool value);
    void setUniform(const char *name, glm::vec2 value);
    void setUniform(const char *name, glm::vec3 value);
    void setUniform(const char *name, glm::vec4 value);
    void setUniform(const char *name, glm::ivec2 value);
    void setUniform(const char *name, glm::ivec3 value);
    void setUniform(const char *name, glm::ivec4 value);
    void setUniform(const char *name, glm::mat3 value, bool Transpose = false);
    void setUniform(const char *name, glm::mat4 value, bool Transpose = false);

    GLuint getProgramID();
    void useProgram();
    void deleteProgram();

private:
    GLuint vertexShader;
    GLuint geometryShader;
    GLuint fragmentShader;

    GLuint shaderProgram = 0;
};


#endif /* SHADER_H */