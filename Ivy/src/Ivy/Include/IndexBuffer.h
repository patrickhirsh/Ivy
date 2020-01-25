#pragma once
#include "IvyPCH.h"

namespace _Ivy
{
	class IndexBuffer
	{
	public:
		IndexBuffer				(const GLuint* data, GLuint count);
		~IndexBuffer			();
		void Bind				() const;
		void UnBind				() const;
		inline GLuint GetCount	() const { return _count; }
	private:
		GLuint					_bufferID;
		GLuint					_count;
	};
}