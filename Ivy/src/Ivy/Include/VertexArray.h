#pragma once
#include "IvyPCH.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

namespace _Ivy
{
	class VertexArray
	{
	public:
		VertexArray();
		~VertexArray();
		void SetVertexBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);
		void Bind() const;
		void Unbind() const;
	private:

		GLuint _id;
	};
}