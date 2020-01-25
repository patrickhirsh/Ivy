#include "IvyPCH.h"
#include "VertexBuffer.h"

namespace _Ivy
{
	VertexBuffer::VertexBuffer(const void* data, GLuint size)
	{
		GL(glGenBuffers(1, &_bufferID));
		GL(glBindBuffer(GL_ARRAY_BUFFER, _bufferID));
		GL(glBufferData(GL_ARRAY_BUFFER, size, data, GL_DYNAMIC_DRAW));
	}

	VertexBuffer::~VertexBuffer()
	{
		GL(glDeleteBuffers(1, &_bufferID));
	}

	void VertexBuffer::Bind() const
	{
		GL(glBindBuffer(GL_ARRAY_BUFFER, _bufferID));
	}

	void VertexBuffer::UnBind() const
	{
		GL(glBindBuffer(GL_ARRAY_BUFFER, 0));
	}
}