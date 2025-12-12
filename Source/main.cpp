#ifdef _WIN64
#include <SDL.h>
#else
#include <SDL2/SDL.h>
#endif
#include "Kartoffel.h"

SDL_Surface*  surface;
SDL_Renderer* renderer = NULL;
SDL_Window*   window = NULL;
SDL_Event     windowEvent;
int           down = 0;
int           running = 1;

void iterate();

// for old version of emcc, it was 2.0
float _scale = 1.0;

void shared_main() {
    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow("Kartoffel", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 320 * 3, 240 * 3, 0);
    surface = SDL_GetWindowSurface(window);

    setup();
    boardRepaint();
}

#ifdef EMCC
#include <emscripten.h>

void asyncLoop(void* args) {
    emscripten_set_main_loop(iterate, 10, 0);
}

int main(int argc, char** argv) {
    shared_main();
    emscripten_async_call(asyncLoop, NULL, 200);
    return EXIT_SUCCESS;
}
#else

int main(int argc, char** argv) {
    shared_main();
    while (running) {
        iterate();
    }
    SDL_DestroyWindow(window);
    SDL_Quit();
    return EXIT_SUCCESS;
}
#endif

void iterate() {
    SDL_Delay(20);
    loop();
    SDL_UpdateWindowSurface(window);
    while (SDL_PollEvent(&windowEvent)) {
        switch (windowEvent.type) {
            case SDL_QUIT:
                running = 0;
                break;
            case SDL_MOUSEBUTTONDOWN:
                down = 1;
                setTouchX(windowEvent.motion.x * _scale);
                setTouchY(windowEvent.motion.y * _scale);
                setMouseUp(1);
                loop();
                SDL_UpdateWindowSurface(window);
                break;
            case SDL_MOUSEMOTION:
                // This is a piece of garbage
                if (down && windowEvent.motion.y > 650) {
                    setTouchX(windowEvent.motion.x * _scale);
                    setTouchY(windowEvent.motion.y * _scale);
                    setMouseUp(1);
                    loop();
                    SDL_UpdateWindowSurface(window);
                }
                break;
            case SDL_MOUSEBUTTONUP:
                down = 0;
                break;
        }
    }
}
