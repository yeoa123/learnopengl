// always include glad bevore glfw. it needs opengl headers.
#include <glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

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

int main(void)
{
    // glfw initialization stuff
    glfwInit();
    // set the version to 3.3, and use the core profile
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    // Window object creation
    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
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

    // start render loop
    while (!glfwWindowShouldClose(window))
    {
        // user inputs
        processInput(window);

        // rendering stuff
        // clear color
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // check and call events. swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // destroy our things

    // terminate
    glfwTerminate();
    return 0;
}