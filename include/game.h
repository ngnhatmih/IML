#ifndef GAME_H
#define GAME_H

#define EXIT_GAME_SUCCESS 0
#define EXIT_GAME_FAILURE 1

#include <SDL3/SDL.h>
#define VERSION "0.1"
#define WINDOW_TITLE "IML " VERSION
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
const SDL_InitFlags SDL_INIT_FLAGS = SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_TIMER;
const SDL_WindowFlags WINDOW_FLAGS = SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL;

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#define GLSL_VERSION "#version 330"

#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_sdl3.h"
#include "theme.h"

class Game {
public:
    static Game& getInstance() {
        static Game g_instance;
        return g_instance;
    }

    Game(Game const&) = delete;
    void operator=(Game const&) = delete;
    
    bool init();
    void processEvent();
    void render();
    void clean();
    void processData();
    void compileShaders();
    void linkShaders();

    SDL_Window *getWindow();
    SDL_GLContext getGLContext();
    bool getRunningState();

private: 
    Game() {}
    SDL_Window *w = nullptr;
    SDL_GLContext gl_context = nullptr;
    bool running = true;
    unsigned int VBO;
    unsigned int VAO;
    unsigned int vertexShader;
    unsigned int fragmentShader;
    unsigned int shaderProgram;
    float vertices[9] = {
        -0.5f, -0.5f, 0.f, 
        0.5f, -0.5f, 0.f,
        0.f, 0.5f, 0.f,
    };
};

#endif /* GAME_H */