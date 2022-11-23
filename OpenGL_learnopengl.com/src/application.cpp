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
void inline framebuffer_size_callback(GLFWwindow* window, int new_width, int new_height)
{
    // resize event
    glViewport(0, 0, new_width, new_width);
}
// processing keyboard inputs
void inline processInput(GLFWwindow* window)
{
    // close window if <ESC> pressed
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

unsigned int width, height;



int main(void)
{
    // glfw initialization stuff
    {
        glfwInit();
        // set the version to 3.3, and use the core profile
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    }

    // Window object creation
    GLFWwindow* window;
    {
        width = 1000;
        height = 500;
        window = glfwCreateWindow(width, height, "LearnOpenGL", NULL, NULL);
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
        glCall(glViewport(0, 0, width, height));
        // activate callback function on resize event
        glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    }

    // begin opengl context
    {
        // construct shader program with vertex and fragment shader
        Shader s("res/shaders/basic.vert", "res/shaders/basic.frag");

        // some vertices and stuff to work with
        unsigned int indices[] = {
            0, 2, 1,
            0, 3, 2
        };
        float vertices[] = {
            // positions    // colors           // tex-coords
            -1.0f, -1.0f,   0.0f, 1.0f, 0.0f,   0.0f, 0.0f,         // bottom left
            -1.0f,  1.0f,   0.0f, 0.0f, 1.0f,   0.0f, 1.0f,         // top left
             1.0f,  1.0f,   0.0f, 0.0f, 1.0f,   1.0f, 1.0f,         // top right
             1.0f, -1.0f,   1.0f, 0.0f, 0.0f,   1.0f, 0.0f          // bottom right
        };

        // construct VertexArray
        VertexArray va;
        // construct VertexBuffer with the positions of the rectangle
        VertexBuffer vb(vertices, sizeof(vertices));
        // construct IndexBuffer with the indices of the rectangle stuff
        IndexBuffer ib(indices, sizeof(indices));
        // construct the layout
        VertexBufferLayout layout;
        {
            // layout the attribute of one vertex: 3 floats for position, 3 floats for colors
            layout.push<float>(2);
            layout.push<float>(3);
            layout.push<float>(2);
            // bind the Buffer with its layout to this VertexArray
            va.addBuffer(vb, layout);
        }
        // construct renderer
        Renderer rnd;


        // get specs of graphics card
        {
            // print maximum size if total work group we can give glDispatchCompute()
            int work_grp_cnt[3];
            glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 0, &work_grp_cnt[0]);
            glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 1, &work_grp_cnt[1]);
            glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 2, &work_grp_cnt[2]);
            printf("max global (total) work group counts x:%i y:%i z:%i\n", work_grp_cnt[0], work_grp_cnt[1], work_grp_cnt[2]);

            // local work group sizes = jobs. this is defined in the shader itself with "layout .."
            int work_grp_size[3];
            glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 0, &work_grp_size[0]);
            glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 1, &work_grp_size[1]);
            glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 2, &work_grp_size[2]);
            printf("max local (in one shader) work group sizes x:%i y:%i z:%i\n", work_grp_size[0], work_grp_size[1], work_grp_size[2]);

            // maximum numebr of Workgroups that a local workgroup in the shader that is allowed (32x32 job in one local workgroup, thend the product cannot exeed ..)
            int work_grp_inv;
            glGetIntegerv(GL_MAX_COMPUTE_WORK_GROUP_INVOCATIONS, &work_grp_inv);
            printf("max local work group invocations %i\n", work_grp_inv);
        }

        // dimensions of the image
        int tex_w = width, tex_h = height;
        GLuint tex_output;
        {
            glCall(glGenTextures(1, &tex_output));
            glCall(glActiveTexture(GL_TEXTURE0));
            glCall(glBindTexture(GL_TEXTURE_2D, tex_output));
            glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
            glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
            glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR))
            glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
            glCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, tex_w, tex_h, 0, GL_RGBA, GL_FLOAT, NULL));
            glCall(glBindImageTexture(0, tex_output, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F));
        }

        GLuint tex_output_buffer;
        {
            glCall(glGenTextures(1, &tex_output_buffer));
            glCall(glActiveTexture(GL_TEXTURE1));
            glCall(glBindTexture(GL_TEXTURE_2D, tex_output_buffer));
            glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
            glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
            glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
            glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
            glCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, tex_w, tex_h, 0, GL_RGBA, GL_FLOAT, NULL));
            glCall(glBindImageTexture(1, tex_output_buffer, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F));
        }

        GLuint positions;
        unsigned int number = 1000;
        {
            glCall(glGenTextures(1, &positions));
            glCall(glActiveTexture(GL_TEXTURE2));
            glCall(glBindTexture(GL_TEXTURE_1D, positions));
            glCall(glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
            glCall(glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
            glCall(glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
            glCall(glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
            glCall(glTexImage1D(GL_TEXTURE_1D, 0, GL_RG16F, number, 0, GL_RG, GL_FLOAT, NULL));
            glCall(glBindImageTexture(2, positions, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F));
        }


        Shader calc("res/shaders/calc.comp");
        Shader copy("res/shaders/copy.comp");
        Shader setup_map("res/shaders/setup_map.comp");
        Shader setup_ants("res/shaders/setup_ants.comp");
        // Shader update_ants("res/shaders/update_ants.comp");


        // setup the double buffering texture
        {
            glCall(glBindImageTexture(1, tex_output_buffer, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F));
            setup_map.bind();
            glCall(glDispatchCompute((GLuint)tex_w, (GLuint)tex_h, 1));
        }
        glCall(glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT));

        // setup the positions
        {
            glCall(glBindImageTexture(2, positions, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RG16F));
            setup_ants.bind();
            glCall(glDispatchCompute(number, 1, 1));
        }
        glCall(glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT));

      
        GLfloat deltaTime = 0.0f, lastFrame = 0.0f;

        // start render loop
        while (!glfwWindowShouldClose(window))
        {
            // Set frame time
            GLfloat currentFrame = glfwGetTime();
            while (deltaTime < 0.5f)
            {
                currentFrame = glfwGetTime();
                deltaTime = currentFrame - lastFrame;
            }
            lastFrame = currentFrame;

            // user inputs
            processInput(window);
            
            // take the values from buffer, calculate, store in the output image
            {
                calc.bind();
                glCall(glDispatchCompute((GLuint)tex_w, (GLuint)tex_h, 1));
            }
            glCall(glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT));

            // copy the output image into the buffer for next frame use
            {
                copy.bind();
                glCall(glDispatchCompute((GLuint)tex_w, (GLuint)tex_h, 1));
            }
            glCall(glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT));

            // make background color
            rnd.drawClearColor(0.15f, 0.2f, 0.3f, 1.0f);

            {
                glClear(GL_COLOR_BUFFER_BIT);
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, tex_output);
                rnd.draw(va, s, ib);
            }

            // check and call events. swap buffers
            glfwSwapBuffers(window);
            glfwPollEvents();
        }
        
    }
    // terminate
    glfwTerminate();
    return 0;
}