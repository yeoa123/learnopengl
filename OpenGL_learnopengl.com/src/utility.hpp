// ----------------------- INCLUDES -----------------------
// always include glad bevore glfw. it needs opengl headers.
#include <glad.h>
#include <GLFW/glfw3.h>
// stuff for io and strings
#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include <sstream>
// my own classes
#include "Renderer.hpp"


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


// ----------------------- OTHER FUNCTIONS -----------------------
// resize callback function from glfw -> change opengl viewport
void inline framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}
// processing keyboard inputs
void inline processInput(GLFWwindow* window)
{
    // close window if <ESC> pressed
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}


// ----------------------- EASY SHADER COMPILATION -----------------------
// load shader from file location
std::string inline readFile(const std::string& filepath)
{
    std::ifstream stream(filepath);
    std::string line;
    std::stringstream code;
    while (getline(stream, line))
    {
        code << line << '\n';
    }
    return code.str();
}
// compile Shader
unsigned int inline compileShaderFromFile(const std::string& filepath)
{
    unsigned int shader;
    // read shader sourcecode from file and compile shader
    std::string vertexCode = readFile(filepath);
    GLchar const* files[] = { vertexCode.c_str() };
    GLint lengths[] = { (GLint)vertexCode.size() };
    // create shader
    std::string shadertype;
    if (filepath.find(".vert") != std::string::npos)
    {
        shader = glCreateShader(GL_VERTEX_SHADER);
        shadertype = "VERTEX";
    }
    else if (filepath.find(".frag") != std::string::npos)
    {
        shader = glCreateShader(GL_FRAGMENT_SHADER);
        shadertype = "FRAGMENT";
    }
    else
    {
        std::cout << "ERROR::SHADER::TYPE_FAILED\n" << std::endl;
        return 0;
    }
    glShaderSource(shader, 1, files, lengths);
    glCompileShader(shader);
    //setup error messages
    int  success;
    char infoLog[512];
    // check for vertex compilation errors
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::" << shadertype << "::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    return shader;
}
