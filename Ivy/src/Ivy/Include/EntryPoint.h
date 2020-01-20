#pragma once
#include "Application.h"

#ifdef IVY_PLATFORM_WINDOWS

extern Ivy::Application* Ivy::CreateApplication();

int main(int argc, char** argv)
{
	auto application = Ivy::CreateApplication();
	application->Run();
	delete application;
}

#endif
