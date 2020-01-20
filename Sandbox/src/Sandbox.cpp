#include "Ivy.h"

class Sandbox : public Ivy::Application
{
public:
	Sandbox()
	{

	}
	~Sandbox()
	{

	}
};

Ivy::Application* Ivy::CreateApplication()
{
	return new Sandbox();
}