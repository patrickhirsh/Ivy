#include "Core/IvyPCH.h"
#include "Window/Render/VertexBuffer.h"

namespace Ivy
{
	Ivy::Ref<VertexBuffer> VertexBuffer::Create(const void* data, GLuint size)
	{
		Ivy::Ref<VertexBuffer> vb = Ivy::Ref<VertexBuffer>(new VertexBuffer());
		GL(glGenBuffers(1, &vb->_id));
		GL(glBindBuffer(GL_ARRAY_BUFFER, vb->_id));
		GL(glBufferData(GL_ARRAY_BUFFER, size, data, GL_DYNAMIC_DRAW));
		return vb;
	}

	VertexBuffer::~VertexBuffer()
	{
		GL(glDeleteBuffers(1, &_id));
	}

	void VertexBuffer::Bind() const
	{
		GL(glBindBuffer(GL_ARRAY_BUFFER, _id));
	}

	void VertexBuffer::Unbind() const
	{
		GL(glBindBuffer(GL_ARRAY_BUFFER, 0));
	}
}