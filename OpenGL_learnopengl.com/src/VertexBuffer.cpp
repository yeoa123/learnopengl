#pragma once
#include "VertexBuffer.hpp"

// ----------------------- CONSTRUCTOR -----------------------
VertexBuffer::VertexBuffer(const void* data, unsigned int bytes)
{
	glCall(glGenBuffers(1, &m_RendererID));
	glCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
	glCall(glBufferData(GL_ARRAY_BUFFER, bytes, data, GL_STATIC_DRAW));
}

VertexBuffer::~VertexBuffer()
{
	glCall(glDeleteBuffers(1, &m_RendererID));
}

void VertexBuffer::bind() const
{
	glCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
}

void VertexBuffer::unbind() const
{
	glCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}
