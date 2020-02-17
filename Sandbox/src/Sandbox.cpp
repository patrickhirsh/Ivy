#include "Sandbox.h"

bool shouldClose = false;
bool mb1Active = false;
bool mb2Active = false;

double mb1CursorX = 0;
double mb1CursorY = 0;
double mb2CursorX = 0;

double xModelRotation = -1.5;
double yModelRotation = 0;
double modelDistance = -50;

void InputCallback(Ivy::Event& Event)
{
	if (Event.GetEventType() == Ivy::EventType::E_KEY_PRESSED)
	{
		Ivy::EventKeyPressed e = Event.As<Ivy::EventKeyPressed>();
		if (e.GetKey() == GLFW_KEY_ESCAPE)
			shouldClose = true;
	}

	if (Event.GetEventType() == Ivy::EventType::E_MOUSEBUTTON_PRESSED)
	{
		Ivy::EventMouseButtonPressed e = Event.As<Ivy::EventMouseButtonPressed>();
		switch (e.GetMouseButton())
		{
		case 0:
			mb1Active = true;
			break;
		case 1:
			mb2Active = true;
			break;
		default:
			break;
		}
	}

	if (Event.GetEventType() == Ivy::EventType::E_MOUSEBUTTON_RELEASED)
	{
		Ivy::EventMouseButtonReleased e = Event.As<Ivy::EventMouseButtonReleased>();
		switch (e.GetMouseButton())
		{
		case 0:
			mb1Active = false;
			mb1CursorX = 0;
			mb1CursorY = 0;
			break;
		case 1:
			mb2Active = false;
			mb2CursorX = 0;
			break;
		default:
			break;
		}
	}

	if (Event.GetEventType() == Ivy::EventType::E_MOUSE_MOVED)
	{
		Ivy::EventMouseMoved e = Event.As<Ivy::EventMouseMoved>();
		if (mb1Active)
		{
			xModelRotation += (mb1CursorY - e.GetPosY()) * 0.005f;
			yModelRotation += (e.GetPosX() - mb1CursorX) * 0.005f;
		}
		if (mb2Active)
		{
			modelDistance += (e.GetPosX() - mb2CursorX) * 0.5f;
		}
		mb1CursorX = e.GetPosX();
		mb1CursorY = e.GetPosY();
		mb2CursorX = e.GetPosX();
	}
}

Sandbox::Sandbox(int argc, char* argv[])
{
	if (argc != 2) { printf("Please provide an argument with the path to a valid OBJ file, relative to 'Resource'\n"); Shutdown(); }
	else { _obj = argv[1]; }
	RegisterEventCallback(Ivy::EventCategory::C_INPUT, BIND_EVENT_FUNCTION(InputCallback));

	//_mesh = Ivy::StaticMesh::Create(_obj);
	//AddEntity(_mesh);
}

Sandbox::~Sandbox()
{
	
}

void Sandbox::Tick()
{
	SetSceneTranslation(0, 0, modelDistance);
	SetSceneRotation(xModelRotation, yModelRotation, 0);
}

Ivy::Application* Ivy::CreateApplication(int argc, char* argv[])
{
	return new Sandbox(argc, argv);
}
