#include "IvyPCH.h"
#include "IndexBuffer.h"

namespace _Ivy
{
	IndexBuffer::IndexBuffer(const GLuint* data, GLuint count)
		: _count(count)
	{
		GL(glGenBuffers(1, &_id));
		GL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _id));
		GL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(GLuint), data, GL_DYNAMIC_DRAW));
	}

	IndexBuffer::~IndexBuffer()
	{
		GL(glDeleteBuffers(1, &_id));
	}

	void IndexBuffer::Bind() const
	{
		GL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _id));
	}

	void IndexBuffer::Unbind() const
	{
		GL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
	}
}