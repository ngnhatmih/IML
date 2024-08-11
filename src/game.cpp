#include "game.h"

void Game::clean() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(2, VBO);
    glDeleteProgram(shaderProgram);

    if (gl_context && w) {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplSDL3_Shutdown();
        ImGui::DestroyContext();
    }

    if (gl_context)
        SDL_GL_DestroyContext(gl_context);

    if (w)
        SDL_DestroyWindow(w);
    
    SDL_Quit();
}

bool Game::init() {
    SDL_SetLogPriority(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_DEBUG);

    // Init SDL subsystems
    if (SDL_Init(SDL_INIT_FLAGS)) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "FAILED TO INIT SDL: %s", SDL_GetError());
        return false;
    }
    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "INITIALIZE SDL: SUCCESS");

    // Set OpenGL attributes
    // TODO: LOG IF ERROR THEN RETURN FALSE
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "SET OPENGL ATTRIBUTES: SUCCESS");

    // Create SDL window
    if ((w = SDL_CreateWindow(WINDOW_TITLE, WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_FLAGS)) == nullptr) {
        SDL_LogCritical(SDL_LOG_CATEGORY_ERROR, "FAILED TO CREATE WINDOW: %s", SDL_GetError());
        clean();
        return false;
    }
    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "CREATE WINDOW: SUCCESS");

    // Create OpenGL context
    if ((gl_context = SDL_GL_CreateContext(w)) == nullptr) {
        SDL_LogCritical(SDL_LOG_CATEGORY_ERROR, "FAILED TO CREATE OPENGL CONTEXT: %s", SDL_GetError());
        clean();
        return false;
    }
    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "CREATE OPENGL CONTEXT: SUCCESS");
    
    // Set the OpenGL context to window
    if (SDL_GL_MakeCurrent(w, gl_context)) {
        SDL_LogCritical(SDL_LOG_CATEGORY_ERROR, "COULD NOT SET OPENGL CONTEXT TO WINDOW %s: %s", SDL_GetWindowTitle(w), SDL_GetError());
        clean();
        return false;
    }
    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "SET OPENGL CONTEXT TO WINDOW: SUCCESS");
    
    // Load pointers to OpenGL functions
    if (!gladLoadGLLoader((GLADloadproc) SDL_GL_GetProcAddress)) {
        SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, "FAILED TO LOAD PTRS TO OPENGL FUNCTIONS: %s", SDL_GetError());
        clean();
        return false;
    }
    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "INITIALIZE GLAD: SUCCESS");


    // Check version and create Imgui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    // Enable Imgui features 
    ImGuiIO &io = ImGui::GetIO(); (void) io;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

    ImGui::StyleColorsDarkRed();

    // Init platform and renderer backends for Imgui
    ImGui_ImplSDL3_InitForOpenGL(w, gl_context);
    ImGui_ImplOpenGL3_Init(GLSL_VERSION);

    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "INITIALIZE GAME: SUCCESS");
    return true;
}

void Game::processEvent() {
    SDL_Event e;
    // Process SDL events
    if (SDL_PollEvent(&e)) {
        ImGui_ImplSDL3_ProcessEvent(&e);
        if (e.type == SDL_EVENT_QUIT) {
            running = false;
        }

        if (e.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED && e.window.windowID == SDL_GetWindowID(w)) {
            running = false;
        }
    }
}

void Game::render() {
    ImGui_ImplSDL3_NewFrame();
    ImGui_ImplOpenGL3_NewFrame();
    ImGui::NewFrame();

    static float rgb[3] = {1.f, 1.f, 1.f};
    static float vertex_1[3] = {-0.5f, -0.5f, 0.0f};
    static float vertex_2[3] = {0.5f, -0.5f, 0.0f};
    static float vertex_3[3] = {0.0f, 0.5f, 0.0f};
    {
        ImGui::SetNextWindowSize(ImVec2(200, 200), ImGuiCond_FirstUseEver);
        ImGui::Begin("HELLO");
        ImGui::ColorEdit3("BG COLOR", rgb);
        
        ImGui::DragFloat3("Vertex 1", vertex_1, 0.1f, -10.f, 10.f, "%.2f");
        ImGui::DragFloat3("Vertex 2", vertex_2, 0.1f, -10.f, 10.f, "%.2f");
        ImGui::DragFloat3("Vertex 3", vertex_3, 0.1f, -10.f, 10.f, "%.2f");

        ImGui::End();
    }

    vertices[0] = vertex_1[0]; vertices[1] = vertex_1[1]; vertices[2] = vertex_1[2];
    vertices[3] = vertex_2[0]; vertices[4] = vertex_2[1]; vertices[5] = vertex_2[2];
    vertices[6] = vertex_3[0]; vertices[7] = vertex_3[1]; vertices[8] = vertex_3[2];

    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    ImGui::Render();

    glViewport(0, 0, ImGui::GetIO().DisplaySize.x, ImGui::GetIO().DisplaySize.y);
    glClearColor(rgb[0], rgb[1], rgb[2], 0.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    
    if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
            SDL_Window *backup_window = SDL_GL_GetCurrentWindow();
            SDL_GLContext backup_context = SDL_GL_GetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            SDL_GL_MakeCurrent(backup_window, backup_context);
    }
    SDL_GL_SwapWindow(w);
}

void Game::processData() {
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(2, VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT,GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Game::compileShaders() {
    // vertex shader
    const char *vertexShaderSource = "#version 330 core\n"
        "layout (location = 0) in vec3 Pos;\n"
        "layout (location = 1) in vec3 in_Color;\n"
        "out vec3 _Color;\n"
        "void main() {\n"
        "   gl_Position = vec4(Pos, 1.0);\n"
        "   _Color = in_Color;\n"
        "}\0";

    // fragment shader
    const char *fragmentShaderSource = "#version 330 core\n"
        "in vec3 _Color;\n"
        "out vec4 Color;\n"
        "void main() {\n"
        "   Color = vec4(_Color, 1.0);\n"
        "}\0";

    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(vertexShader, 1, &vertexShaderSource, 0);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, 0);

    glCompileShader(vertexShader);
    glCompileShader(fragmentShader);
}

void Game::linkShaders() {
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, fragmentShader);
    glAttachShader(shaderProgram, vertexShader);
    glLinkProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    glUseProgram(shaderProgram);
}

bool Game::getRunningState() {
    return running;
}

SDL_Window *Game::getWindow() {
    return w;
}

SDL_GLContext Game::getGLContext() {
    return gl_context;
}
