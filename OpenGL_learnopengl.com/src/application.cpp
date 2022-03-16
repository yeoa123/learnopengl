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
#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"
#include "VertexArray.hpp"
#include "VertexBufferLayout.hpp"


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



int main(void)
{
    // glfw initialization stuff
    {
        glfwInit();
        // set the version to 3.3, and use the core profile
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    }

    // Window object creation
    GLFWwindow* window;
    {
        window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
        if (window == NULL)
        {
            std::cout << "Failed to create GLFW window" << std::endl;
            glfwTerminate();
            return -1;
        }
        // make this window our main context on this thread
        glfwMakeContextCurrent(window);

        // initialize glad, so that we can use opengl functions
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            std::cout << "Failed to initialize GLAD" << std::endl;
            return -1;
        }

        // setup the viewport
        // rendering size and coordinates respective to the window
        glViewport(0, 0, 800, 600);
        // activate callback function on resize event
        glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    }

    // begin opengl context
    {
        // vertex shader setup
        unsigned int vertexShader = compileShaderFromFile("res/shaders/basic.vert");

        // fragment shader setup
        unsigned int fragmentShader = compileShaderFromFile("res/shaders/basic.frag");

        // shader program setup
        unsigned int shaderProgram;
        {
            shaderProgram = glCreateProgram();
            glAttachShader(shaderProgram, vertexShader);
            glAttachShader(shaderProgram, fragmentShader);
            glLinkProgram(shaderProgram);
            //setup error messages
            int  success;
            char infoLog[512];
            // shader programm errors
            glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
            if (!success) {
                glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
            }
            glDeleteShader(vertexShader);
            glDeleteShader(fragmentShader);
        }

        // some vertices and stuff to work with
        unsigned int indices[] = {
            0, 1, 3,   // first triangle
            1, 2, 3    // second triangle
        };
        float vertices[] = {
             0.5f,  0.5f, 0.0f,  // top right
             0.5f, -0.5f, 0.0f,  // bottom right
            -0.5f, -0.5f, 0.0f,  // bottom left
            -0.5f,  0.5f, 0.0f   // top left 
        };

        // construct VertexArray
        VertexArray va;
        // construct VertexBuffer with the positions of the rectangle
        VertexBuffer vb(vertices, sizeof(vertices));
        // construct IndexBuffer with the indices of the rectangle stuff
        IndexBuffer ib(indices, sizeof(indices));
        // construct the layout
        VertexBufferLayout layout;
        // layout the attribute of one vertex: 3 floats for position
        layout.push<float>(3);
        // bind the Buffer with its layout to this VertexArray
        va.addBuffer(vb, layout);

        // start render loop
        while (!glfwWindowShouldClose(window))
        {
            // user inputs
            processInput(window);

            // clear color
            glClearColor(0.15f, 0.2f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            // timing stuff
            float timeValue = (float)glfwGetTime();
            float pulse = (sin(timeValue) / 2.0f) + 0.5f;
            int vertexColorLocation = glGetUniformLocation(shaderProgram, "pulse");
            if (vertexColorLocation == -1)
                std::cout << "ERROR::SHADER::UNIFORM_NOT_EXISTING\n" << std::endl;

            // rendering stuff
            glUseProgram(shaderProgram);
            va.bind();
            ib.bind();

            // make error to see what happens GL_UNSIGNED_INT -> GL_INT
            glCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0));


            glUniform2f(vertexColorLocation, 8 * pulse, pulse);

            // check and call events. swap buffers
            glfwSwapBuffers(window);
            glfwPollEvents();
        }
    }
    // terminate
    glfwTerminate();
    return 0;
}