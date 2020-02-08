#pragma once
#include "Core/IvyPCH.h"
#include "Render/Renderer.h"
#include "Event/Event.h"


int main(int argc, char* argv[]);

namespace Ivy
{
	class IVY_API Application
	{
	public:
		Application						();
		virtual ~Application			();
		
		virtual void Tick				() = 0;
		void RegisterEventCallback		(EventCategory category, EventCallback callback);
		void UnregisterEventCallback	(EventCategory category, EventCallback callback);
		void Shutdown					() { _active = false; }
	
		// TODO: Remove this and implement entity-component system
	public:
		void AddEntity(Ivy::Ref<StaticMesh> mesh) { _renderer->DrawRequest(mesh); }
		void SetSceneTranslation(float transX, float transY, float transZ);
		void SetSceneRotation(float angleX, float angleY, float angleZ);

	private:
		bool Run						(int argc, char* argv[]);

		bool							_active = true;
		GLFWwindow*						_window;
		_Ivy::Renderer*					_renderer;

		bool initEngine					();
		bool initGLFW					();
		bool initGLEW					();
		bool initRenderer				();

		void obtainWindowContext		();

		friend int ::main(int argc, char* argv[]);
	};
}


