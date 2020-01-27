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
		Window									(const char* name, int width, int height);
		~Window									();
		void Update								();
		void Draw								(Ivy::Ref<Object> object);
		bool IsActive							() const { return _active; }
		void RegisterEventCallback				(Ivy::EventCategory category, Ivy::EventCallback callback);
		void UnregisterEventCallback			(Ivy::EventCategory category, Ivy::EventCallback callback);
	
	private:
		void initWindow							(const char* name, int width, int height);
		void initStartup						();
		void initRenderer						();
		void initEventDispatcher				();
		void makeCurrentContext					();
		void setInactive						();	

		GLFWwindow*								_window;
		_Ivy::Render*							_render;
		_Ivy::EventDispatcher*					_event;
		bool									_active = true;
		int										_xPos, _yPos;
		int										_width, _height;
	};
}