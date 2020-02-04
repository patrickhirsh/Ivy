#include "Core/IvyPCH.h"
#include "Window/Render/IndexBuffer.h"

namespace Ivy
{
	Ivy::Ref<IndexBuffer> IndexBuffer::Create(const GLuint* data, GLuint count)
	{
		Ivy::Ref<IndexBuffer> ib = Ivy::Ref<IndexBuffer>(new IndexBuffer());
		ib->_count = count;
		GL(glGenBuffers(1, &ib->_id));
		GL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ib->_id));
		GL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(GLuint), data, GL_DYNAMIC_DRAW));
		return ib;
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