#include "LasershowRenderer.hxx"

#include "Lasershow.h"
#include "Log.hxx"

#include <cmath>
#include <sstream>
#include <vector>
#include <iostream>

#define NUM_TRIANGLES 4*512

#define GL {\
    GLenum err;\
    while ((err = glGetError()) != GL_NO_ERROR)\
    {\
        std::stringstream ss;\
        ss << __FILE__ << "(" << __LINE__ << "): OpenGL: " << glGetError();\
        Log::errAndQuit(ss.str());\
    }\
}

LaserShowRenderer::LaserShowRenderer()
{
    generators = getLasershows(&numGenerators);
    if (numGenerators < 1)
    {
        Log::errAndQuit("No Lasershows found!");
    }

    // Initialize OpenGL
    {
        GLfloat g_vertex_buffer_data[9 * NUM_TRIANGLES];
        for (int i = 0; i < NUM_TRIANGLES; i++)
        {
            float p0 = i / float(NUM_TRIANGLES);
            float p1 = (i + 1) / float(NUM_TRIANGLES);

            g_vertex_buffer_data[9 * i + 0] = 0.0f;
            g_vertex_buffer_data[9 * i + 1] = 0.9f;
            g_vertex_buffer_data[9 * i + 2] = 0.0f;

            g_vertex_buffer_data[9 * i + 3] = 0.0f - 2.5f * cosf(p0 * M_PI);
            g_vertex_buffer_data[9 * i + 4] = 0.9f - 2.5f * sinf(p0 * M_PI);
            g_vertex_buffer_data[9 * i + 5] = 0.0f;

            g_vertex_buffer_data[9 * i + 6] = 0.0f - 2.5f * cosf(p1 * M_PI);
            g_vertex_buffer_data[9 * i + 7] = 0.9f - 2.5f * sinf(p1 * M_PI);
            g_vertex_buffer_data[9 * i + 8] = 0.0f;
        }
        GL(glGenBuffers(1, &vertexbuffer));
        GL(glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer));
        GL(glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 9 * NUM_TRIANGLES, g_vertex_buffer_data, GL_STATIC_DRAW));
    }

    {
        GLfloat g_vertex_buffer_data[] =
        {
            -1.0f, -1.0f, 0.0f,
            -1.0f, 1.0f, 0.0f,
            1.0f, 1.0f, 0.0f,

            1.0f, 1.0f, 0.0f,
            1.0f, -1.0f, 0.0f,
            -1.0f, -1.0f, 0.0f
        };
        GL(glGenBuffers(1, &vertexbuffer2));
        GL(glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer2));
        GL(glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 18, g_vertex_buffer_data, GL_STATIC_DRAW));
    }

    GL(glGenBuffers(1, &colorbuffer));

    // Initialize shaders
    {
        GLint Result = GL_FALSE;
        int InfoLogLength;
        GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
        GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

        // vertex shader
        GL(glShaderSource(VertexShaderID, 1, &vertexShaderSrc, NULL));
        GL(glCompileShader(VertexShaderID));

        GL(glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result));
        GL(glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength));
        if (InfoLogLength > 0)
        {
            std::vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
            GL(glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, VertexShaderErrorMessage.data()));
            Log::errAndQuit("\n" + std::string(VertexShaderErrorMessage.begin(), VertexShaderErrorMessage.end()));
        }

        // fragment shader
        GL(glShaderSource(FragmentShaderID, 1, &fragmentShaderSrc, NULL));
        GL(glCompileShader(FragmentShaderID));

        GL(glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result));
        GL(glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength));
        if (InfoLogLength > 0)
        {
            std::vector<char> FragmentShaderErrorMessage(InfoLogLength + 1);
            glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, FragmentShaderErrorMessage.data());
            Log::errAndQuit("\n" + std::string(FragmentShaderErrorMessage.begin(), FragmentShaderErrorMessage.end()));
        }

        // program
        program = glCreateProgram();
        GL(glAttachShader(program, VertexShaderID));
        GL(glAttachShader(program, FragmentShaderID));
        GL(glLinkProgram(program));

        glGetProgramiv(program, GL_LINK_STATUS, &Result);
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &InfoLogLength);
        if (InfoLogLength > 0)
        {
            std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
            glGetProgramInfoLog(program, InfoLogLength, NULL, ProgramErrorMessage.data());
            Log::errAndQuit("\n" + std::string(ProgramErrorMessage.begin(), ProgramErrorMessage.end()));
        }

        // cleanup
        GL(glDetachShader(program, VertexShaderID));
        GL(glDetachShader(program, FragmentShaderID));

        GL(glDeleteShader(VertexShaderID));
        GL(glDeleteShader(FragmentShaderID));
    }

    // Initialize shaders
    {
        GLint Result = GL_FALSE;
        int InfoLogLength;
        GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
        GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

        // vertex shader
        GL(glShaderSource(VertexShaderID, 1, &vertexShaderSrc2, NULL));
        GL(glCompileShader(VertexShaderID));

        GL(glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result));
        GL(glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength));
        if (InfoLogLength > 0)
        {
            std::vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
            GL(glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, VertexShaderErrorMessage.data()));
            Log::errAndQuit("\n" + std::string(VertexShaderErrorMessage.begin(), VertexShaderErrorMessage.end()));
        }

        // fragment shader
        GL(glShaderSource(FragmentShaderID, 1, &fragmentShaderSrc2, NULL));
        GL(glCompileShader(FragmentShaderID));

        GL(glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result));
        GL(glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength));
        if (InfoLogLength > 0)
        {
            std::vector<char> FragmentShaderErrorMessage(InfoLogLength + 1);
            glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, FragmentShaderErrorMessage.data());
            Log::errAndQuit("\n" + std::string(FragmentShaderErrorMessage.begin(), FragmentShaderErrorMessage.end()));
        }

        // program
        program2 = glCreateProgram();
        GL(glAttachShader(program2, VertexShaderID));
        GL(glAttachShader(program2, FragmentShaderID));
        GL(glLinkProgram(program2));

        glGetProgramiv(program2, GL_LINK_STATUS, &Result);
        glGetProgramiv(program2, GL_INFO_LOG_LENGTH, &InfoLogLength);
        if (InfoLogLength > 0)
        {
            std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
            glGetProgramInfoLog(program2, InfoLogLength, NULL, ProgramErrorMessage.data());
            Log::errAndQuit("\n" + std::string(ProgramErrorMessage.begin(), ProgramErrorMessage.end()));
        }

        // cleanup
        GL(glDetachShader(program2, VertexShaderID));
        GL(glDetachShader(program2, FragmentShaderID));

        GL(glDeleteShader(VertexShaderID));
        GL(glDeleteShader(FragmentShaderID));
    }
}

LaserShowRenderer::~LaserShowRenderer()
{
    GL(glDeleteProgram(program));
    GL(glDeleteBuffers(1, &colorbuffer));
    GL(glDeleteBuffers(1, &vertexbuffer));
}

void
LaserShowRenderer::render(int, int)
{
    GL(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
    GL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

    unsigned char r[NUM_TRIANGLES];
    unsigned char g[NUM_TRIANGLES];
    unsigned char b[NUM_TRIANGLES];
    generators[currentGenerator].generate(currentBeat, NUM_TRIANGLES, r, g, b);

    GL(glUseProgram(program));

    GLfloat colors[NUM_TRIANGLES * 9];
    for (int i = 0; i < NUM_TRIANGLES; i++)
    {
        float cr = float(r[i]) / 255.0f;
        float cg = float(g[i]) / 255.0f;
        float cb = float(b[i]) / 255.0f;
        float brightness = std::max(cr, std::max(cg, cb));


        colors[9 * i + 0] = std::min(1.0f, 0.1f * brightness + 0.9f * cr);
        colors[9 * i + 1] = std::min(1.0f, 0.1f * brightness + 0.9f * cg);
        colors[9 * i + 2] = std::min(1.0f, 0.1f * brightness + 0.9f * cb);
        colors[9 * i + 3] = cr * 0.2f;
        colors[9 * i + 4] = cg * 0.2f;
        colors[9 * i + 5] = cb * 0.2f;
        colors[9 * i + 6] = cr * 0.2f;
        colors[9 * i + 7] = cg * 0.2f;
        colors[9 * i + 8] = cb * 0.2f;
    }
    glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 9 * NUM_TRIANGLES, colors, GL_STATIC_DRAW);

    GL(glEnableVertexAttribArray(0));
    GL(glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer));
    GL(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr));

    GL(glEnableVertexAttribArray(1));
    GL(glBindBuffer(GL_ARRAY_BUFFER, colorbuffer));
    GL(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr));

    GL(glDrawArrays(GL_TRIANGLES, 0, 3 * NUM_TRIANGLES));

    GL(glDisableVertexAttribArray(0));
    GL(glDisableVertexAttribArray(1));

}

void LaserShowRenderer::render2DVisualization(int, int)
{
    GL(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
    GL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

    GL(glUseProgram(program2));

    GL(glEnableVertexAttribArray(0));
    GL(glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer2));
    GL(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr));

    GL(glEnableVertexAttribArray(1));
    GL(glBindBuffer(GL_ARRAY_BUFFER, colorbuffer));
    GL(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr));

    GL(glDrawArrays(GL_TRIANGLES, 0, 6));

    GL(glDisableVertexAttribArray(0));
    GL(glDisableVertexAttribArray(1));
}

void LaserShowRenderer::update(float dT)
{
    currentBeat += dT * bpm / 60.0f;
    if (currentBeat > generators[currentGenerator].duration)
    {
        if (random)
        {
            // TODO
        }

        currentBeat -= int(currentBeat);
        generators[currentGenerator].reset();
    }
}

void LaserShowRenderer::setShow(std::string name)
{
    for (size_t i = 0; i < numGenerators; i++)
    {
        if (std::string(generators[i].name) == name)
        {
            currentGenerator = i;
            return;
        }
    }
}

const char *LaserShowRenderer::vertexShaderSrc = R"(
#version 330 core
layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec3 vertexColor;

out vec3 fragmentColor;

void main(){
    gl_Position.xyz = vertexPosition_modelspace;
    gl_Position.w = 1.0;
    fragmentColor = vertexColor;
}
)";

const char *LaserShowRenderer::fragmentShaderSrc = R"(
#version 330 core
in vec3 fragmentColor;
out vec3 color;
void main(){
    color = fragmentColor;
}
)";

const char *LaserShowRenderer::vertexShaderSrc2 = R"(
#version 330 core
layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec3 vertexColor;

out vec3 fragmentColor;

void main(){
    gl_Position.xyz = vertexPosition_modelspace;
    gl_Position.w = 1.0;
    fragmentColor = vertexColor;
}
)";

const char *LaserShowRenderer::fragmentShaderSrc2 = R"(
#version 330 core
in vec3 fragmentColor;
out vec3 color;
void main(){
    color = vec3(1.0,0.2,0.5);
}
)";
