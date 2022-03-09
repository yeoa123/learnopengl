// ----------------------- INCLUDES -----------------------
#include "utility.hpp"


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
        glBindVertexArray(VAO);

        // make error to see what happens GL_UNSIGNED_INT -> GL_INT
        glCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0));
        

        glUniform2f(vertexColorLocation, 8*pulse, pulse);

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