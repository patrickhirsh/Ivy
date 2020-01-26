#pragma once
#include "IvyPCH.h"

namespace _Ivy
{
	class IndexBuffer
	{
	public:
		static Ivy::Ref<IndexBuffer> Create(const GLuint* data, GLuint count);

	public:
		~IndexBuffer			();
		void Bind				() const;
		void Unbind				() const;
		inline GLuint GetCount	() const { return _count; }
	private:
		IndexBuffer				() {};
		GLuint					_id;
		GLuint					_count;
	};
}