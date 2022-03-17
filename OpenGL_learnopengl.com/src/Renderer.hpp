#pragma once
#include "ErrorHandling.hpp"
#include "VertexArray.hpp"
#include "IndexBuffer.hpp"
#include "Shader.hpp"


class Renderer
{
public:
	Renderer();
	~Renderer();

	void draw(const VertexArray& v, const Shader& s, const IndexBuffer& i);
	void drawClearColor(float f0, float f1, float f2, float f3);

private:
    
};
