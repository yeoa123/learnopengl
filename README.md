# Error Checking but advanced
If anybody is interested in glDebugMessageCallback:
Take a look into docs.gl and the glfw documentation. Or if you don't have time:

/* to load the function pointer for glDebugMessageCallback and 'enable' debugging in your current glfw context
put the following before glfwCreateWindow: */
 glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);

/* as the name suggests, glDebugMessageCallback takes a callback function, here is one posibility
in CPP, the signature might look way different, please consult google. Also, on windows, there are some
issues where you have to mark sth. in the signature with __std or something  */
void GLAPIENTRY errorOccurredGL(GLenum source,
                                GLenum type,
                                GLuint id,
                                GLenum severity,
                                GLsizei length,
                                const GLchar* message,
                                const void* userParam)
{
  printf("Message from OpenGL:\nSource: 0x%x\nType: 0x%x\n"
         "Id: 0x%x\nSeverity: 0x%x\n", source, type, id, severity);
  printf("%s\n", message);
  exit(-1); // shut down the program gracefully (it does cleanup stuff too)
// without exit(), OpenGL will constantly print the error message... make sure to kill your program.
}

// Now, just befor entering the main loop, put:
  glEnable(GL_DEBUG_OUTPUT);
  glDebugMessageCallback(errorOccurredGL, NULL);