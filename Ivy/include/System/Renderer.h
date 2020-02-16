#pragma once
#include "Core/IvyPCH.h"
#include "System/System.h"
#include "Window/Window.h"



namespace _Ivy
{
	class Renderer : public System
	{
	public:
		void Update(Ivy::Ref<Window> window);
		Renderer() {};
		~Renderer() {};
	};
}