#pragma once
#include <glad.h>
#include <iostream>
#include <iomanip>

// ----------------------- ERROR CHECKING -----------------------
#define ASSERT(x) if (!(x)) __debugbreak();
#define glCall(x) GLClearError();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__));
// error function internal to gl
static void GLClearError()
{
    while (glGetError() != GL_NO_ERROR);
}
static bool GLLogCall(const char* function, const char* file, int line)
{
    while (GLenum error = glGetError())
    {
        // printing <error> as hexadeximal value
        std::cout << "[OpenGL Error] (0x" << std::setfill('0') << std::setw(4) << std::hex << error << ")"
            << function << " in " << file << ":" << std::dec << line << std::endl;
        return false;
    }
    return true;
}