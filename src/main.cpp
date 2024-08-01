#include <SDL3/SDL.h>

SDL_Window *w = nullptr;
SDL_Renderer *r = nullptr;

int main() {
    SDL_Init(SDL_INIT_VIDEO || SDL_INIT_EVENTS);
    w = SDL_CreateWindow("HelloWorld", 300, 500, 0);
    r = SDL_CreateRenderer(w, 0);

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
    return 0;
}