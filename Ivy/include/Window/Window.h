#pragma once
#include "Core/IvyPCH.h"

namespace _Ivy
{
	class Window
	{
	public:
		static Ivy::Ref<Window> Create(
			int width,
			int height,
			std::string name);

	public:
		~Window						() {};
		bool IsActive				() const { return _active; }
		void BeginFrame				();
		void EndFrame				();
		void SetCurrentContext		();

		std::tuple<GLuint, GLuint> GetDimensions();

	private:
		Window						() {};
		bool						_active;
		GLFWwindow*					_window;
	};
}