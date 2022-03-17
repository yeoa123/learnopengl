#pragma once
#include <string>
#pragma once
#include <fstream>
#include <sstream>
#include <iostream>
#include "Renderer.hpp"

class Shader
{
private:
	unsigned int m_RendererID;

public:
	Shader(const std::string& vertexfilepath, const std::string& fragmentfilepath);
	~Shader();

	void bind() const;
	void unbind() const;

	void setUniform4f(const std::string& name, float f0, float f1, float f2, float f3);

private:
	std::string readFile(const std::string& filepath);
	unsigned int compileShaderFromFile(const std::string& filepath);
};
