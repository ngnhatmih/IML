#include <SDL3/SDL.h>
#include <iostream>

// int -> boolean;
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
    while (1) {
        if (SDL_PollEvent(&e)) {
            if (e.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED) {
                break;
            }
        }
    }
    

    SDL_DestroyRenderer(r);
    SDL_DestroyWindow(w);
    SDL_Quit();

    return EXIT_SUCCESS;
}