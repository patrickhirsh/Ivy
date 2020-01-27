#include "Ivy.h"


void TestCallback(Ivy::Event& Event)
{
	if (Event.GetEventType() == Ivy::EventType::KEY_PRESSED)
	{
		Ivy::EventKeyPressed e = (*(Ivy::EventKeyPressed*)&Event);
		printf("%d\n", e.GetKey());
	}
}

int main()
{
	auto window = Ivy::Window("Hello World!", 1280, 720);
	auto teapot = Ivy::Object::Create(std::string("teapot.obj"));
	
	window.RegisterEventCallback(Ivy::EventCategory::KEYBOARD, BIND_EVENT_FUNCTION(TestCallback));

	while (window.IsActive())
	{ 
		window.Draw(teapot);
		window.Update();
	}

	return 0;
}