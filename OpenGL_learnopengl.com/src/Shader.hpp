#pragma once
#include <string>
#pragma once
#include <fstream>
#include <sstream>
#include <iostream>
#include "ErrorHandling.hpp"

class Shader
{
private:

public:
	unsigned int m_RendererID;
	Shader(const std::string& vertexfilepath, const std::string& fragmentfilepath);
	Shader(const std::string& computefilepath);
	~Shader();

	void bind() const;
	void unbind() const;

	void setUniform4f(const std::string& name, float f0, float f1, float f2, float f3);

private:
	std::string readFile(const std::string& filepath);
	unsigned int compileShaderFromFile(const std::string& filepath);
};
