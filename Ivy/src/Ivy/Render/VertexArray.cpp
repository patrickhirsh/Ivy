#include "IvyPCH.h"
#include "VertexArray.h"

namespace _Ivy
{
	VertexArray::VertexArray()
	{
		GL(glGenVertexArrays(1, &_id));
	}
	VertexArray::~VertexArray()
	{
		GL(glDeleteVertexArrays(1, &_id));
	}

	void VertexArray::SetVertexBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout)
	{
		Bind();
		vb.Bind();
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
	}

	void VertexArray::Unbind() const
	{
		GL(glBindVertexArray(0));
	}
}