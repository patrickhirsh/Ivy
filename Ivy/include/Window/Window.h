#pragma once
#include "Core/IvyPCH.h"
#include "Event/Event.h"
#include "Render/Render.h"
#include "Entity/Entity.h"
#include "Entity/StaticMesh.h"

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
		bool IsActive							() const { return _active; }
		void RegisterEventCallback				(Ivy::EventCategory category, Ivy::EventCallback callback);
		void UnregisterEventCallback			(Ivy::EventCategory category, Ivy::EventCallback callback);
		
		void Tick								(Ivy::Ref<StaticMesh> object);

		void SetSceneTranslation				(float transX, float transY, float transZ);
		void SetSceneRotation					(float angleX, float angleY, float angleZ);
	
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