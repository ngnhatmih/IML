#include <SDL3/SDL.h> // sdl
#include "imgui.h" // imgui
#include "imgui_impl_sdl3.h" // platform
#include "imgui_impl_sdlrenderer3.h" // renderer

int main() {
    // sdl init
    SDL_Init(SDL_INIT_VIDEO || SDL_INIT_EVENTS);
    SDL_Window *w = SDL_CreateWindow("hello world", 800, 500, SDL_WINDOW_RESIZABLE);
    SDL_Renderer *r = SDL_CreateRenderer(w, 0);

    // imgui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO(); (void) io;

    ImGui::StyleColorsDark();

    ImGui_ImplSDL3_InitForSDLRenderer(w, r); // init platform 
    ImGui_ImplSDLRenderer3_Init(r); // init renderer 

    SDL_Event e;
    while (1) {
        ImGui_ImplSDL3_ProcessEvent(&e);
        if (SDL_PollEvent(&e)) {
            if (e.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED) {
                break;
            }
        }

        ImGui_ImplSDL3_NewFrame();
        ImGui_ImplSDLRenderer3_NewFrame();
        ImGui::NewFrame();
        
        {
            ImGui::Begin("hello");
            ImGui::End();  
        }
        

        ImGui::Render();
        SDL_RenderClear(r);
        ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), r);
        SDL_RenderPresent(r);
    }

    ImGui_ImplSDL3_Shutdown();
    ImGui_ImplSDLRenderer3_Shutdown();
    ImGui::DestroyContext();

    SDL_DestroyRenderer(r);
    SDL_DestroyWindow(w);
    SDL_Quit();

    return 0;
}