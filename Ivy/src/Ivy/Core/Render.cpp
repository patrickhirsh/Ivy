#include "IvyPCH.h"
#include "Render.h"

namespace _Ivy
{
	void Render::DrawRequest(Ivy::Ref<Ivy::Object> object)
	{
		_drawRequests.push_back(object);
	}

	void Render::ProcessRequests(GLFWwindow* window)
	{
		for (auto request : _drawRequests)
		{

		}
	}
}