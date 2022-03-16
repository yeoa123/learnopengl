#pragma once
#include "VertexArray.hpp"

VertexArray::VertexArray()
{
	glCall(glGenVertexArrays(1, &m_RendererID));
}

VertexArray::~VertexArray()
{
	glCall(glDeleteVertexArrays(1, &m_RendererID));
}

void VertexArray::addBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout)
{
	// first bind the VA, then the VB
	bind();
	vb.bind();
	// get the different elements of the layout ( example: 3 float for position + 1 int for other stuff + ... )
	const auto& elements = layout.getElements();
	unsigned int offset = 0;
	// loop over every element -> tell GL the Attributes of the bound VB 
	for (unsigned int i = 0; i < elements.size(); i++)
	{
		const auto& element = elements[i];
		glCall(glEnableVertexAttribArray(i));
		glCall(glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.getStride(), (const void*)offset));
		// calculate the offset (=bytes to hop) to get to the next element
		offset += element.count * VertexBufferLayoutElements::getBytesOfType(element.type);
	}
}

void VertexArray::bind() const
{
	glCall(glBindVertexArray(m_RendererID));
}

void VertexArray::unbind() const
{
	glCall(glBindVertexArray(0));
}
