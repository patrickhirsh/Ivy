#pragma once
#include "Ivy.h"

class Sandbox : public Ivy::Application
{
public:
	Sandbox(int argc, char* argv[]);
	~Sandbox();
	void Tick();
};