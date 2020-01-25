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
		void UnBind				() const;
	private:
		GLuint					_bufferID;
	};
}