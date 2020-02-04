#pragma once
#include "Core/IvyPCH.h"

namespace Ivy
{
	class VertexBuffer
	{
	public:
		static Ivy::Ref<VertexBuffer> Create(const void* data, GLuint size);

	public:
		~VertexBuffer		();
		void Bind			() const;
		void Unbind			() const;
	private:
		VertexBuffer		() {};
		GLuint				_id;
	};
}