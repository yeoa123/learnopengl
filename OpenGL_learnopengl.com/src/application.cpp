// ----------------------- INCLUDES -----------------------
// always include glad bevore glfw. it needs opengl headers.
#include <glad.h>
#include <GLFW/glfw3.h>

// my own classes
#include "Renderer.hpp"
#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"
#include "VertexArray.hpp"
#include "VertexBufferLayout.hpp"
#include "Shader.hpp"


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
        Shader s("res/shaders/basic.vert", "res/shaders/basic.frag");

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
            
            s.setUniform4f("uniformVariable", pulse, pulse, pulse, pulse);
            

            // rendering stuff          
            s.bind();
            va.bind();
            ib.bind();

            // make error to see what happens GL_UNSIGNED_INT -> GL_INT
            glCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0));


            

            // check and call events. swap buffers
            glfwSwapBuffers(window);
            glfwPollEvents();
        }
    }
    // terminate
    glfwTerminate();
    return 0;
}