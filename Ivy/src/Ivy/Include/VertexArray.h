#pragma once
#include "IvyPCH.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

namespace _Ivy
{
	class VertexArray
	{
	public:
		static Ivy::Ref<VertexArray> Create();

	public:
		~VertexArray			();
		void SetVertexBuffer	(Ivy::Ref<VertexBuffer> vb, const VertexBufferLayout& layout);
		void Bind				() const;
		void Unbind				() const;
	private:
		VertexArray				() {};
		GLuint					_id;
	};
}