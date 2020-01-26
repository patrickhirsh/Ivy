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
		static int CompileShader(unsigned int type, const std::string& source);
		static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);

	public:
		Render									() {};
		~Render									() {};
		void DrawRequest						(Ivy::Ref<Ivy::Object> object);
		void ProcessRequests					(GLFWwindow* window);

	private:
		std::vector<Ivy::Ref<Ivy::Object>>		_drawRequests;
	};
}