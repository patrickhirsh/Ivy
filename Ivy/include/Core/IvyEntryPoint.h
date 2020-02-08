#pragma once
#include "Core/IvyApplication.h"

namespace Ivy
{
	extern Application* CreateApplication(int argc, char* argv[]);
}

int main(int argc, char* argv[])
{
	auto IvyApp = Ivy::CreateApplication(argc, argv);
	auto Status = IvyApp->Run(argc, argv);
	delete IvyApp;
	return Status;
}