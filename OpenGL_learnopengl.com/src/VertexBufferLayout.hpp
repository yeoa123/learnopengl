#pragma once
#include <vector>
#include "Renderer.hpp"

// stores: type, count, normalized
struct VertexBufferLayoutElements
{
	unsigned int type;
	unsigned int count;
	unsigned char normalized;

	// OpenGL types -> size in bytes
	static unsigned int getBytesOfType(unsigned int type)
	{
		switch (type)
		{
			case GL_FLOAT:			return sizeof(GLfloat);
			case GL_UNSIGNED_INT:	return sizeof(GLuint);
			case GL_UNSIGNED_BYTE:	return sizeof(GLbyte);
		}
		ASSERT(false);
		return 0;
	}
};

class VertexBufferLayout
{
private:
	std::vector<VertexBufferLayoutElements> m_Elements;
	unsigned int m_Stride;

public:
	VertexBufferLayout()
		: m_Stride(0){};
	~VertexBufferLayout(){};
	
	template<typename T>
	void push(unsigned int count)
	{
		static_assert(sizeof(T) == 0, "Type not implemented.");
	}

	template<>
	inline void push<float>(unsigned int count)
	{
		m_Elements.push_back({ GL_FLOAT, count, GL_FALSE });
		m_Stride += count * VertexBufferLayoutElements::getBytesOfType(GL_FLOAT);
	}

	template<>
	inline void push<unsigned int>(unsigned int count)
	{
		m_Elements.push_back({ GL_UNSIGNED_INT, count, GL_FALSE });
		m_Stride += count * VertexBufferLayoutElements::getBytesOfType(GL_UNSIGNED_INT);
	}

	template<>
	inline void push<unsigned char>(unsigned int count)
	{
		m_Elements.push_back({ GL_UNSIGNED_BYTE, count, GL_TRUE });
		m_Stride += count * VertexBufferLayoutElements::getBytesOfType(GL_UNSIGNED_BYTE);
	}

	inline std::vector<VertexBufferLayoutElements> const& getElements() const { return m_Elements; }
	inline unsigned int getStride() const { return m_Stride; }

};
