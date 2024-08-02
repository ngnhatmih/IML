#include <SDL3/SDL.h>
#include <iostream>

SDL_Window *w = 0;
SDL_Renderer *r = 0;

int main() {
    if (!SDL_Init(SDL_INIT_VIDEO || SDL_INIT_EVENTS)) {
        SDL_Log("Init-ed successfully");
    } else {
        SDL_Log("Init-ed failed");
        return EXIT_FAILURE;
    }

    w = SDL_CreateWindow("HelloWorld", 300, 500, 0);
    if (w != nullptr) { 
        SDL_Log("Created window successfully");
    } else {
        SDL_Log("Failed to create window");
        return EXIT_FAILURE;
    }

    r = SDL_CreateRenderer(w, 0);
    if (r != nullptr) {
        SDL_Log("Created renderer successfully");
    } else {
        SDL_Log("Failed to create renderer");
        return EXIT_FAILURE;
    }

    SDL_Event e;
    SDL_Color color;
    SDL_Color colors[12] = {
        {255, 0,   0,   0},
        {255, 127, 0,   0},
        {255, 255, 0,   0},
        {127, 255, 0,   0},
        {0,   255, 0,   0},
        {0,   255, 127, 0},
        {0,   255, 255, 0},
        {0,   127, 255, 0},
        {0,   0,   255, 0},
        {127, 0,   255, 0},
        {255, 0,   255, 0},
        {255, 0,   127, 0},
    };

    int i = 0;

    while (1) {
        color = colors[i];
        if (SDL_PollEvent(&e)) {
            if (e.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED) {
                break;
            }
        }
        SDL_RenderClear(r);
        SDL_SetRenderDrawColor(r, color.r, color.g, color.b, 0);
        SDL_RenderPresent(r);
        if (i++ == 13) {
            i = 0;
        }

        SDL_Delay(100);
    }

    
    

    SDL_DestroyRenderer(r);
    SDL_DestroyWindow(w);
    SDL_Quit();

    return EXIT_SUCCESS;
}