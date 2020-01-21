#pragma once
#include "IvyPCH.h"

namespace Ivy
{
	class IVY_API Window
	{
	public:
		static Ivy::Ref<Window> CreateWindow(
			const char*							name,
			int									width,
			int									height);
	private:
		static std::vector<Ivy::Ref<Window>>*	_activeWindows;

	public:
		~Window									();
		void Update								();
	private:
		Window									() {};

		GLFWwindow*								_window;
		int										_width;
		int										_height;
	};
}