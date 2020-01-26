#pragma once
#include "IvyPCH.h"
#include "Object.h"
#include "Resource.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "VertexArray.h"
#include "IndexBuffer.h"

namespace _Ivy
{
	class Render
	{
	public:
		Render									() {};
		~Render									() {};
		void DrawRequest						(Ivy::Ref<Ivy::Object> object);
		void ProcessRequests					(GLFWwindow* window);

	private:
		std::vector<Ivy::Ref<Ivy::Object>>		_drawRequests;
	};
}