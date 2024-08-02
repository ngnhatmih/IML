#include <SDL3/SDL.h>
#include "imgui.h"
#include "imgui_impl_sdl3.h" // platform: input, window, ...
#include "imgui_impl_sdlrenderer3.h" // renderer

#include "theme.h"

int main() {
    // sdl init
    SDL_Init(SDL_INIT_EVENTS || SDL_INIT_VIDEO);
    SDL_Window *w = SDL_CreateWindow("ss", 1200, 500, SDL_WINDOW_RESIZABLE);
    SDL_Renderer *r = SDL_CreateRenderer(w, 0);
    SDL_ShowWindow(w);

    // imgui init
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO(); (void) io;
    
    ImGui::StyleColorsDark();

    ImGui_ImplSDL3_InitForSDLRenderer(w, r); // init platform
    ImGui_ImplSDLRenderer3_Init(r); // init renderer

    SDL_Event e;
    while (1) {
        ImGui_ImplSDL3_ProcessEvent(&e);
        static float rgb[] = {1, 1, 1}; // float is from 0 to 1

        if (SDL_PollEvent(&e)) {
            if (e.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED) {
                break;
            }
        }

        ImGui_ImplSDL3_NewFrame();
        ImGui_ImplSDLRenderer3_NewFrame();
        ImGui::NewFrame();

        
        {
            ImGui::Begin("change style");
            ImGui::SetWindowSize(ImVec2(1000, 200), ImGuiCond_FirstUseEver);
            
            const char* styles[] = {"dark", "light", "classic", "ruda", "moonlight", "purple comfy"};
            static int current = 0;
            ImGui::ListBox("style list box", &current, styles, 6);
            if (styles[current] == "dark") {
                ImGui::StyleColorsDark();
            } else if (styles[current] == "light") {
                ImGui::StyleColorsLight();
            } else if (styles[current] == "ruda") {
                ImGui::StyleColorsDarkRuda();
            } else if (styles[current] == "classic") {
                ImGui::StyleColorsClassic();
            } else if (styles[current] == "moonlight") {
                ImGui::StyleColorsMoonlight();
            } else if (styles[current] == "purple comfy") {
                ImGui::StyleColorsPurpleComfy();
            }

            
            ImGui::ColorEdit3("my color", rgb);

            ImGui::End();
        }
        

        ImGui::Render();
        SDL_RenderClear(r);
        SDL_SetRenderDrawColorFloat(r ,rgb[0], rgb[1], rgb[2], 0);
        ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), r);
        SDL_RenderPresent(r);
    }

    ImGui_ImplSDL3_Shutdown();
    ImGui_ImplSDLRenderer3_Shutdown();
    ImGui::DestroyContext();

    SDL_DestroyWindow(w);
    SDL_DestroyRenderer(r);
    SDL_Quit();
    return 0;
}