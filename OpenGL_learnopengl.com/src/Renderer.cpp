#pragma once
#include "Renderer.hpp"


Renderer::Renderer()
{
}

Renderer::~Renderer()
{
}

void Renderer::draw(const VertexArray& v, const Shader& s, const IndexBuffer& i)
{
    s.bind();
    v.bind();
    i.bind();
    glCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0));
}

void Renderer::drawClearColor(float f0, float f1, float f2, float f3)
{
    glCall(glClearColor(f0, f1, f2, f3));
    glCall(glClear(GL_COLOR_BUFFER_BIT));
}
