#pragma once


class IndexBuffer
{
private:
	// the unsigned int thing for OpenGL
	unsigned int m_RendererID;
	// number of uints (indeces) in this buffer
	unsigned int m_Count;

public:
	// data | sizeof(data)
	IndexBuffer(const unsigned* data, unsigned int count);
	~IndexBuffer();

	void bind() const;
	void unbind() const;

	// get number of uints (indeces) in this buffer
	inline unsigned int getCount() const { return m_Count; };
};
