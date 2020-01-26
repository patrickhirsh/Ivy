#pragma once
#include "IvyPCH.h"

namespace _Ivy
{
	class VertexBuffer
	{
	public:
		VertexBuffer			(const void* data, GLuint size);
		~VertexBuffer			();
		void Bind				() const;
		void Unbind				() const;
	private:
		GLuint					_id;
	};
}