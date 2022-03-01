// always include glad bevore glfw. it needs opengl headers.
#include <glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream> 

// resize callback function from glfw -> change opengl viewport
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

// processing keyboard inputs
void processInput(GLFWwindow* window)
{
    // close window if <ESC> pressed
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// load shader from file location
const std::string loadShaderFromFile(const std::string& filepath)
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

// basic vertex shader
const char* vertexShaderSource = 
"#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

// basic fragment shader
const char* fragmentShaderSource = 
"#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\0";


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

    // vertex shader setup
    unsigned int vertexShader;
    {
        vertexShader = glCreateShader(GL_VERTEX_SHADER); 
        const std::string code = loadShaderFromFile("res/shaders/basic.vert");
        const char* src = code.c_str();
        glShaderSource(vertexShader, 1, &src, NULL);
        glCompileShader(vertexShader);
        //setup error messages
        int  success;
        char infoLog[512];
        // check for vertex compilation errors
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
        if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    }

    // fragment shader setup
    unsigned int fragmentShader;
    {
        fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
        glCompileShader(fragmentShader);
        //setup error messages
        int  success;
        char infoLog[512];
        // check for vertex compilation errors
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
        }
    }
    
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

    // setup an Vertex Array Object, Vertex Buffer Object, Element Buffer Object
    unsigned int VAO, VBO, EBO;
    {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);
        // bind VAO first, other stuff
        glBindVertexArray(VAO);
        // some vertices and stuff to work with
        float vertices[] = {
             0.5f,  0.5f, 0.0f,  // top right
             0.5f, -0.5f, 0.0f,  // bottom right
            -0.5f, -0.5f, 0.0f,  // bottom left
            -0.5f,  0.5f, 0.0f   // top left 
        };
        unsigned int indices[] = {
            0, 1, 3,   // first triangle
            1, 2, 3    // second triangle
        };
        // make VBO stuff
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        // make EBO stuff
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
        // debind the VAO, then other stuff
        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    // start render loop
    while (!glfwWindowShouldClose(window))
    {
        // user inputs
        processInput(window);

        // clear color
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // rendering stuff
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // check and call events. swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // destroy our things
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    // terminate
    glfwTerminate();
    return 0;
}