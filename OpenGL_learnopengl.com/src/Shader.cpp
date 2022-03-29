#pragma once
#include "Shader.hpp"



// load shader from file location
std::string Shader::readFile(const std::string& filepath)
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
unsigned int Shader::compileShaderFromFile(const std::string& filepath)
{
    unsigned int shader;
    // read shader sourcecode from file and compile shader
    std::string code = readFile(filepath);
    GLchar const* files[] = { code.c_str() };
    GLint lengths[] = { (GLint)code.size() };
    // create shader
    std::string shadertype;
    if (filepath.find(".vert") != std::string::npos)
    {
        glCall(shader = glCreateShader(GL_VERTEX_SHADER));
        shadertype = "VERTEX";
    }
    else if (filepath.find(".frag") != std::string::npos)
    {
        glCall(shader = glCreateShader(GL_FRAGMENT_SHADER));
        shadertype = "FRAGMENT";
    }
    else if (filepath.find(".comp") != std::string::npos)
    {
        glCall(shader = glCreateShader(GL_COMPUTE_SHADER));
        shadertype = "COMPUTE";
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


Shader::Shader(const std::string& vertexfilepath, const std::string& fragmentfilepath)
{
	unsigned int vs = compileShaderFromFile(vertexfilepath);
    unsigned int fs = compileShaderFromFile(fragmentfilepath);
    glCall(m_RendererID = glCreateProgram());
    glCall(glAttachShader(m_RendererID, vs));
    glCall(glAttachShader(m_RendererID, fs));
    glCall(glLinkProgram(m_RendererID));
    //setup error messages
    int  success;
    char infoLog[512];
    // shader programm errors
    glCall(glGetProgramiv(m_RendererID, GL_LINK_STATUS, &success));
    if (!success) {
        glCall(glGetProgramInfoLog(m_RendererID, 512, NULL, infoLog)) ;
    }
    glCall(glDeleteShader(fs));
    glCall(glDeleteShader(vs));


}

Shader::Shader(const std::string& computefilepath)
{
    unsigned int c = compileShaderFromFile(computefilepath);
    glCall(m_RendererID = glCreateProgram());
    glCall(glAttachShader(m_RendererID, c));
    glCall(glLinkProgram(m_RendererID));
    //setup error messages
    int  success;
    char infoLog[512];
    // shader programm errors
    glCall(glGetProgramiv(m_RendererID, GL_LINK_STATUS, &success));
    if (!success) {
        glCall(glGetProgramInfoLog(m_RendererID, 512, NULL, infoLog));
    }
    glCall(glDeleteShader(c));

}

Shader::~Shader()
{
    glCall(glDeleteProgram(m_RendererID));
}

void Shader::bind() const
{
    glCall(glUseProgram(m_RendererID));
}

void Shader::unbind() const
{
    glCall(glUseProgram(0));
}

void Shader::setUniform4f(const std::string& name, float f0, float f1, float f2, float f3)
{
    bind();
    glCall(int location = glGetUniformLocation(m_RendererID, name.c_str()));
    if (location == -1)
        std::cout << "ERROR::SHADER::UNIFORM_NOT_EXISTING\n" << std::endl;
    glCall(glUniform4f(location, f0, f1, f2, f3));
}
