#pragma once
#include "IvyPCH.h"

namespace _Ivy
{
	/* Represents an element in a VertexBufferLayout */
	struct VertexBufferLayoutElement
	{
		GLuint	Type;
		GLuint	Count;
		GLuint	Normalized;

		static unsigned int SizeOf(GLuint type)
		{
			switch (type)
			{
				case GL_FLOAT:			return 4;
				case GL_UNSIGNED_INT:	return 4;
			}
			ASSERT(false);
			return 0;
		}
	};

	/* Represents the layout of elements within a VertexBuffer */
	class VertexBufferLayout
	{
	public:
		VertexBufferLayout()
			: _stride(0) {}

		inline const std::vector<VertexBufferLayoutElement>& GetElements() const { return _elements; }
		inline GLuint GetStride() const { return _stride; }

		template<typename T>
		void Push(GLuint count)
		{
			static_assert(false);
		}

		template<>
		void Push<float>(GLuint count)
		{
			_elements.push_back({ GL_FLOAT, count, GL_FALSE });
			_stride += count * sizeof(float);
		}

		template<>
		void Push<unsigned int>(GLuint count)
		{
			_elements.push_back({ GL_UNSIGNED_INT, count, GL_FALSE });
			_stride += count * sizeof(unsigned int);
		}

	private:
		std::vector<VertexBufferLayoutElement> _elements;
		GLuint _stride;
	};
}