#include "Ivy.h"

int main(){
	auto window = Ivy::Window("Hello World!", 1280, 720);
	auto teapot = Ivy::Object::Create(std::string("teapot.obj"));
	
	while (window.IsActive())
	{ 
		window.Draw(teapot);
		window.Update();
	}

	return 0;
}