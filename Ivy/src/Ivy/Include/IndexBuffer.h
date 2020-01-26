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
		void Unbind				() const;
		inline GLuint GetCount	() const { return _count; }
	private:
		GLuint					_id;
		GLuint					_count;
	};
}