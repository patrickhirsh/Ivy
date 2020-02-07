#pragma once
#include "Core/IvyApplication.h"

namespace Ivy
{
	extern Application* CreateApplication();
}

int main(int argc, char* argv[])
{
	auto IvyApp = Ivy::CreateApplication();
	IvyApp->Run();
	delete IvyApp;
}