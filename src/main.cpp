#include <SDL3/SDL.h>
#include "imgui.h"
#include "imgui_impl_sdl3.h"
#include "imgui_impl_sdlrenderer3.h"

#include<theme.h>

int main() {
    SDL_Init(SDL_INIT_VIDEO||SDL_INIT_EVENTS);
    SDL_Window *w = SDL_CreateWindow("window", 800, 500, SDL_WINDOW_RESIZABLE);
    SDL_Renderer *r = SDL_CreateRenderer(w, 0);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO(); (void) io;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    ImGui_ImplSDL3_InitForSDLRenderer(w, r);
    ImGui_ImplSDLRenderer3_Init(r);

    ImGui::StyleColorsClassic();

    SDL_Event e;
    while(1) {
        ImGui_ImplSDL3_ProcessEvent(&e);

        if (SDL_PollEvent(&e)) {
            if (e.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED) {
                break;
            }
        }

        ImGui_ImplSDL3_NewFrame();
        ImGui_ImplSDLRenderer3_NewFrame();
        ImGui::NewFrame();

        ImGuiID dockspace_id = ImGui::GetID("My dockspace");
        ImGui::DockSpaceOverViewport(dockspace_id);
        
        static float rgb[] = {1, 1, 1};
        // window 1
        {
            ImGui::Begin("imgui");
            ImGui::SetWindowSize(ImVec2(200, 200), ImGuiCond_FirstUseEver);
            //ImGui::SetWindowFontScale(2.5);

            if (ImGui::TreeNode("parent")) {
                ImGui::Text("child");   
                ImGui::TreePop(); 
            }
            
            if (ImGui::TreeNode("Imgui style")) {
                static int current_item = 0;
                const char *const items[] = {"classic", "dark", "light", "Moonlight", "Dark ruda"};
                ImGui::ListBox("", &current_item, items, IM_ARRAYSIZE(items));
                if (current_item == 0) {
                    ImGui::StyleColorsClassic();        
                } else if (current_item == 1) {
                    ImGui::StyleColorsDark();
                } else if (current_item == 2) {
                    ImGui::StyleColorsLight();
                } else if (current_item == 3) {
                    ImGui::StyleColorsMoonlight();
                } else if (current_item == 4) {
                    ImGui::StyleColorsDarkRuda();
                }
                ImGui::TreePop();
            } 

            if (ImGui::TreeNode("change background color")) {
                ImGui::ColorEdit3("", rgb);
                ImGui::TreePop();
            }
    
            if (ImGui::TreeNode("Information")) {               
                ImGui::Text("mouse position: (%.2f, %.2f)", ImGui::GetMousePos().x, ImGui::GetMousePos().y);
                ImGui::Text("Window position: (%.2f, %.2f)", ImGui::GetWindowPos().x, ImGui::GetWindowPos().y);
                ImGui::Text("Cursor screen position: (%.2f, %.2f)", ImGui::GetCursorScreenPos().x, ImGui::GetCursorScreenPos().y);
                ImGui::Text("Cursor position: (%.2f, %.2f)", ImGui::GetCursorPos().x, ImGui::GetCursorPos().y);
                ImGui::Text("Content region available: (%.2f, %.2f)", ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y);
                ImGui::Text("Content region max: (%.2f, %.2f)", ImGui::GetContentRegionMax().x, ImGui::GetContentRegionMax().y);
                ImGui::Text("Window content region max: (%.2f, %.2f)", ImGui::GetWindowContentRegionMax().x, ImGui::GetWindowContentRegionMax().y);
                ImGui::Text("Window content region min: (%.2f, %.2f)", ImGui::GetWindowContentRegionMin().x, ImGui::GetWindowContentRegionMin().y);
                ImGui::Text("Window padding: (%.2f, %.2f)", ImGui::GetStyle().WindowPadding.x, ImGui::GetStyle().WindowPadding.y);
                
                ImGuiViewport *viewport = ImGui::GetMainViewport();
                ImGui::Text("Viewport position: (%.2f, %.2f)", viewport->Pos.x, viewport->Pos.y);
                ImGui::Text("Viewport position: (%.2f, %.2f)", viewport->GetCenter().x, viewport->GetCenter().y);
                ImGui::TreePop();
            }

            ImGui::End();
        }

        // window 2
        {   
            ImGui::Begin("window 2");
            ImGui::SetWindowSize(ImVec2(200, 200), ImGuiCond_FirstUseEver);
            static char text1[32] = "";
            static char text2[32] = "";
            static char text3[32] = "";
            static char text4[32] = "";
            ImGui::InputText("default", text1, IM_ARRAYSIZE(text1));
            ImGui::InputText("decimal", text2, IM_ARRAYSIZE(text2), ImGuiInputTextFlags_CharsDecimal);
            ImGui::InputText("hexadecimal", text3, IM_ARRAYSIZE(text3), ImGuiInputTextFlags_CharsHexadecimal | ImGuiInputTextFlags_CharsUppercase);
            ImGui::InputText("no blank", text4, IM_ARRAYSIZE(text4), ImGuiInputTextFlags_CharsNoBlank);

            ImGui::End();
        }

        // window 3
        {
            ImGui::Begin("window 3");
            ImGui::SetWindowSize(ImVec2(200, 200), ImGuiCond_FirstUseEver);

            static char text[60 * 60] = "";
            ImGui::InputTextMultiline("label", text, IM_ARRAYSIZE(text), ImVec2(-FLT_MIN, -FLT_MIN));
            ImGui::End();
        }
        
        ImGui::Render();
        SDL_RenderClear(r);
        SDL_SetRenderDrawColorFloat(r, rgb[0], rgb[1], rgb[2], 0);
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