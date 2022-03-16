#pragma once
#include "IndexBuffer.hpp"

IndexBuffer::IndexBuffer(const unsigned* data, unsigned int bytes)
	:m_Count(bytes/sizeof(unsigned int))
{
	glCall(glGenBuffers(1, &m_RendererID));
	glCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
	glCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, bytes, data, GL_STATIC_DRAW));
}

IndexBuffer::~IndexBuffer()
{
	glCall(glDeleteBuffers(1, &m_RendererID));
}

void IndexBuffer::bind() const
{
	glCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
}

void IndexBuffer::unbind() const
{
	glCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}
