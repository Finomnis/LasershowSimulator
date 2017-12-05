#pragma once

#define GLEW_STATIC
#include "GL/glew.h"

#include <string>

struct LaserShowGenerator;

class LaserShowRenderer
{
    public:
        LaserShowRenderer();
        ~LaserShowRenderer();
        void render(int w, int h);
        void render2DVisualization(int w, int h);
        void update(float dt);

        void setShow(std::string name);

    private:
        float currentBeat = 0.0f;
        float bpm = 120.0f;

        bool random = false;

        size_t currentGenerator = 0;
        size_t numGenerators;
        LaserShowGenerator *generators;

    private:
        GLuint vertexbuffer;
        GLuint colorbuffer;
        GLuint program;

        static const char *fragmentShaderSrc;
        static const char *vertexShaderSrc;

};
