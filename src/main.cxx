#include "Log.hxx"
#include "LasershowRenderer.hxx"

#define GLEW_STATIC
#include <GL/glew.h>
#define SDL_MAIN_HANDLED
#include <SDL.h>

#include <iostream>
#include <sstream>
#include <chrono>

#define SDL(func){\
    if((func)!=0){\
        std::stringstream ss;\
        ss << __FILE__ << "(" << __LINE__ << "): SDL: " << SDL_GetError();\
        Log::errAndQuit(ss.str());\
    }\
}

int main(int, char *[])
{
    srand(time(NULL));
    // Initialize SDL
    SDL_SetMainReady();
    SDL(SDL_Init(SDL_INIT_EVERYTHING));

    // Create window
    SDL_Window *window = SDL_CreateWindow("Lasershow Simulator",
                                          SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                          800, 600,
                                          SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    if (!window)
        Log::errAndQuit(SDL_GetError());

    {
        int x, y, w, h;
        SDL_GetWindowPosition(window, &x, &y);
        SDL_GetWindowSize(window, &w, &h);
        SDL_SetWindowPosition(window, x - w / 2 - 1, y);
    }

    SDL(SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3));
    SDL(SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3));
    SDL(SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE));

    SDL_GLContext glContext = SDL_GL_CreateContext(window);
    if (!glContext)
        Log::errAndQuit(SDL_GetError());

    SDL(SDL_GL_SetSwapInterval(1));

    // Create window2
    SDL_Window *window2 = SDL_CreateWindow("Current Show",
                                           SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                           640, 480,
                                           SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    if (!window2)
        Log::errAndQuit(SDL_GetError());

    {
        int x, y, w, h;
        SDL_GetWindowPosition(window2, &x, &y);
        SDL_GetWindowSize(window2, &w, &h);
        SDL_SetWindowPosition(window2, x + w / 2 + 1, y);
    }

    SDL(SDL_GL_MakeCurrent(window2, glContext));
    SDL(SDL_GL_MakeCurrent(window, glContext));

    // Initialize GLEW
    {
        glewExperimental = GL_TRUE;
        GLenum err = glewInit();
        if (err != GLEW_OK)
            Log::errAndQuit((const char *)glewGetErrorString(err));
    }
    LaserShowRenderer laserShowRenderer;
    laserShowRenderer.setShow("Moving Waves");

    // Run main loop
    auto t_previous = std::chrono::high_resolution_clock::now();
    bool loop = true;
    while (loop)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
                loop = false;
            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)
                loop = false;
            if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE)
                loop = false;
        }

        // get delta time
        auto t_now = std::chrono::high_resolution_clock::now();
        std::chrono::duration<float> t_diff = t_now - t_previous;
        t_previous = t_now;
        float dT = t_diff.count();

        //std::cout << dT << std::endl;

        laserShowRenderer.update(dT);

        {
            SDL(SDL_GL_MakeCurrent(window, glContext));
            int w, h;
            SDL_GL_GetDrawableSize(window, &w, &h);
            glViewport(0, 0, w, h);
            laserShowRenderer.render(w, h);
            SDL_GL_SwapWindow(window);
        }

        {
            SDL(SDL_GL_MakeCurrent(window2, glContext));
            int w, h;
            SDL_GL_GetDrawableSize(window2, &w, &h);
            glViewport(0, 0, w, h);
            laserShowRenderer.render2DVisualization(w, h);
            SDL_GL_SwapWindow(window2);
        }
    }

    SDL_GL_DeleteContext(glContext);
    SDL_DestroyWindow(window2);
    SDL_DestroyWindow(window);
    SDL_Quit();
    exit(EXIT_SUCCESS);
}
