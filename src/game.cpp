#define STB_IMAGE_IMPLEMENTATION
#include "game.h" 

void Game::clean() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    shader->deleteProgram();
    delete shader;

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

    // Load shaders and create shader program
    shader = new Shader("res/shaders/vert.glsl", "res/shaders/frag.glsl");

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
    static float alpha = 0;
    alpha = glm::pi<float>() * glm::sin(SDL_GetTicks()/1000.f);
    static bool upside_down = false;
    static int current = 1;
    static int texture_unit = 0;
    static float x_offset = 0.f;
    static float y_offset = 0.f;
    static int shape = 0;
    const char *modes[3] = {"LINE", "FILL", "POINT"};
    const char *texture_units[3] = {"texture1", "texture2", "none"};
    {
        ImGui::SetNextWindowSize(ImVec2(200, 200), ImGuiCond_FirstUseEver);
        ImGui::Begin("HELLO");
        ImGui::ColorEdit3("BG COLOR", rgb);
        ImGui::ListBox("MODE", &current, modes, IM_ARRAYSIZE(modes));
        if (current == 0) {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        } else if (current == 1) {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        } else {
            glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
        }

        ImGui::DragFloat("alpha", &alpha, 0.01f, 0.f, glm::pi<float>());

        float padding = 4.f;
        ImVec2 textSize = ImGui::CalcTextSize("OFF");
        ImVec2 buttonSize(textSize.x + padding * 2, textSize.y + padding * 2);
        ImGui::Text("UPSIDE DOWN: "); ImGui::SameLine();
        if (ImGui::Button(upside_down ? "ON" : "OFF", buttonSize)) {
            upside_down = !upside_down;
        }
        ImGui::DragFloat("x_offset", &x_offset, 0.01f, -10.f, 10.f);
        ImGui::DragFloat("y_offset", &y_offset, 0.01f, -10.f, 10.f);

        if (ImGui::RadioButton("RECTANGLE", &shape, 0)) {
            x_offset = y_offset = 0;
        }; 
        ImGui::SameLine();
        
        if (ImGui::RadioButton("TRIANGLE", &shape, 1)) {
            x_offset = y_offset = 0;
        } 
        ImGui::SameLine();

        ImGui::RadioButton("CIRCLE", &shape, 2);

        ImGui::ListBox("TEXTURE UNIT", &texture_unit,  texture_units, IM_ARRAYSIZE(texture_units));
        ImGui::End();
    }

    ImGui::Render();

    glViewport(0, 0, ImGui::GetIO().DisplaySize.x, ImGui::GetIO().DisplaySize.y);
    glClearColor(rgb[0], rgb[1], rgb[2], 0.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture2);

    shader->useProgram();
    shader->setUniform("alpha", alpha);
    shader->setUniform("upside_down", upside_down);
    shader->setUniform("x_offset", x_offset);
    shader->setUniform("y_offset", y_offset);
    shader->setUniform("texture_unit", texture_unit);

    glBindVertexArray(VAO);
    if (shape == 0) {
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    } else if (shape == 1) {
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (void *)(6 * sizeof(GLuint)));
    }
    
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
    
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
    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "PROCESSING DATA...");

    glGenVertexArrays(1, &VAO);
    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "GENERATED VAO = %u", VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "GENERATED VBO = %u", VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), 0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) (3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) (6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glGenBuffers(1, &EBO);
    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "GENERATED EBO = %u", EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // load texture
    int w, h, channels;
    const char *path = "res/textures/brick001.jpg";
    unsigned char *data = stbi_load(path, &w, &h, &channels, 0);
    // if (data) {
    //     SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "LOADED IMAGE %s: (%d, %d, %d)", path, w, h, channels);
    // }

    glGenTextures(1, &texture1);
    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "GENERATED texture = %u", texture1);
    glBindTexture(GL_TEXTURE_2D, texture1); // 2d texture/ image texture
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &w);
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &h);
    // SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "ADDED IMAGE TO TEXTURE: (%d, %d)", w, h);

    stbi_image_free(data);
    
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

    glBindTexture(GL_TEXTURE_2D, 0);

    path = "res/textures/marble.jpg";
    data = stbi_load(path, &w, &h, &channels, 4);
    // if (data) {
    //     SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "LOADED IMAGE %s: (%d, %d, %d)", path, w, h, channels);
    // }
    
    glGenTextures(1, &texture2);
    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "GENERATED texture = %u", texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &w);
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &h);
    // SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "ADDED IMAGE TO TEXTURE: (%d, %d)", w, h);

    stbi_image_free(data);

    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

    glBindTexture(GL_TEXTURE_2D, 0);

    shader->useProgram();
    shader->setUniform("_texture1", 0);
    shader->setUniform("_textur2e2", 1);

    // GLint t1, t2;
    // glGetUniformiv(shader->getProgramID(), glGetUniformLocation(shader->getProgramID(), "_texture1"), &t1);
    // glGetUniformiv(shader->getProgramID(), glGetUniformLocation(shader->getProgramID(), "_textur2e2"), &t2);

    // SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "uniform _texture1: %d", t1);
    // SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "uniform _textur2e2: %d", t2);
    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "FINISHED PROCESSING DATA");
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
