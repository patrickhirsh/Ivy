#pragma once
#include "Core/IvyPCH.h"
#include "Window/Event.h"
#include "Render/Render.h"
#include "Object/Object.h"
#include "Object/Renderable/StaticMesh.h"

namespace Ivy
{
	class Window
	{
	public:
		Window									(GLFWwindow* window);
		~Window									();
		void Update								();
		bool IsActive							() const { return _active; }	
		void RegisterEventCallback				(Ivy::EventCategory category, Ivy::EventCallback callback);
		void UnregisterEventCallback			(Ivy::EventCategory category, Ivy::EventCallback callback);
		
		void Tick								(Ivy::Ref<StaticMesh> object);

		void SetSceneTranslation				(float transX, float transY, float transZ);
		void SetSceneRotation					(float angleX, float angleY, float angleZ);
		void MakeCurrentContext					();
	
	private:
		void initRenderer						();
		void initEventDispatcher				();

		bool									_active;
		GLFWwindow*								_window;
		Ivy::Render*							_render;
		Ivy::EventDispatcher*					_event;
		int										_xPos, _yPos;
		int										_width, _height;
	};
}