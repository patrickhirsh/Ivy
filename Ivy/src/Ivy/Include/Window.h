#pragma once
#include "IvyPCH.h"
#include "Object.h"
#include "Render.h"
#include "Event.h"

namespace Ivy
{
	class IVY_API Window
	{
	public:
		static Ivy::Ref<Window> Create(
			const char*							name,
			int									width,
			int									height);
	private:
		static std::vector<Ivy::Ref<Window>>*	_activeWindows;

	public:
		~Window									();
		void Update								();
		void Draw								(Ivy::Ref<Object> object);
	private:
		bool initWindow(
			const char*							name, 
			int									width, 
			int									height);

		/* Core systems */
		void initStartup						();
		void initRenderer						();
		void initEventDispatcher				();
		_Ivy::Render*							_render;
		_Ivy::EventDispatcher*					_event;

		/* Window */
		Window									() {};
		GLFWwindow*								_window;
		int										_width;
		int										_height;
	};
}