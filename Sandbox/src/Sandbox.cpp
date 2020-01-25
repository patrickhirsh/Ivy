#include "Ivy.h"

int main(){
	auto window = Ivy::Window::Create("Hello World!", 1280, 720);
	auto teapot = Ivy::Object::Create(std::string("teapot.obj"));
	while (true) 
	{ 
		window->Draw(teapot);
		window->Update(); 
	}
	return 0;
}