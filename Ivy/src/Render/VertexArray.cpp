#include "Core/IvyPCH.h"
#include "Render/VertexArray.h"

namespace _Ivy
{
	Ivy::Ref<VertexArray> VertexArray::Create()
	{
		Ivy::Ref<VertexArray> va = Ivy::Ref<VertexArray>(new VertexArray());
		GL(glGenVertexArrays(1, &va->_id));
		return va;
	}

	VertexArray::~VertexArray()
	{
		GL(glDeleteVertexArrays(1, &_id));
	}

	void VertexArray::SetVertexBuffer(Ivy::Ref<VertexBuffer> vb, const VertexBufferLayout& layout)
	{
		Bind();
		vb->Bind();
		const auto& elements = layout.GetElements();
		unsigned int offset = 0;
		for (unsigned int i = 0; i < elements.size(); i++)
		{
			const auto& element = elements[i];
			GL(glEnableVertexAttribArray(i));
			GL(glVertexAttribPointer(i, element.Count, element.Type, element.Normalized, 
				layout.GetStride(), (const void*)offset));

			offset += element.Count * VertexBufferLayoutElement::SizeOf(element.Type);
		}
	}

	void VertexArray::Bind() const
	{
		GL(glBindVertexArray(_id));
		GL(glEnableClientState(GL_VERTEX_ARRAY));
	}

	void VertexArray::Unbind() const
	{
		GL(glBindVertexArray(0));
		GL(glDisableClientState(GL_VERTEX_ARRAY));
	}
}