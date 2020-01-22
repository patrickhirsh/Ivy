#include "Ivy.h"

int main()
{
	auto window = Ivy::Window::Create("Hello World!", 1280, 720);
	while (true) { window->Update(); }
	return 0;
}