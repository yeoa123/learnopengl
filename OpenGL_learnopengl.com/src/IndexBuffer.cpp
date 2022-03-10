// ----------------------- INCLUDES -----------------------
#pragma once
#include "IndexBuffer.hpp"
#include "utility.hpp"


// ----------------------- CONSTRUCTOR -----------------------
IndexBuffer::IndexBuffer(const unsigned* data, unsigned int count)
	:m_Count(count)
{
	glCall(glGenBuffers(1, &m_RendererID));
	glCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
	glCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count*sizeof(unsigned int), data, GL_STATIC_DRAW));
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
