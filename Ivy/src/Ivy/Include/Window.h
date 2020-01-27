#pragma once
#include "IvyPCH.h"
#include "Object.h"
#include "Render.h"
#include "Event.h"

#define IVY_ACTIVE_WINDOW_ONLY \
	if (_active == false) { return; }

namespace Ivy
{
	class IVY_API Window
	{
	private:
		static std::vector<Window*>	_activeWindows;

	public:
		Window(
			const char*							name,
			int									width,
			int									height);

		~Window									();
		void Update								();
		bool IsActive							() const { return _active; }
		void Draw								(Ivy::Ref<Object> object);
	
	private:
		void initWindow(
			const char*							name, 
			int									width, 
			int									height);

		/* Core Systems */
		void initStartup						();
		void initRenderer						();
		void initEventDispatcher				();
		_Ivy::Render*							_render;
		_Ivy::EventDispatcher*					_event;

		void makeCurrentContext					();
		void setInactive						();						
		GLFWwindow*								_window;
		bool									_active = true;
		int										_xPos, _yPos;
		int										_width, _height;
	};
}