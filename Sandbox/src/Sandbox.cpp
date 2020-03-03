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
	if (argc != 2) { printf("Please provide an argument with the path to a valid OBJ file, relative to 'Resource'\n"); }
	else { _obj = argv[1]; }
	RegisterEventCallback(Ivy::EventCategory::C_INPUT, BIND_EVENT_FUNCTION(InputCallback));

	/*
	// Cubemap
	auto transformCM = Ivy::Transform();
	auto meshCM = Ivy::Mesh();
	meshCM.SourceMeshPath = "cube.obj";
	auto cubemapCM = Ivy::Cubemap();
	cubemapCM.SourceTexturePathPosX = "cubemap/cubemap_posx.png";
	cubemapCM.SourceTexturePathNegX = "cubemap/cubemap_negx.png";
	cubemapCM.SourceTexturePathPosY = "cubemap/cubemap_posy.png";
	cubemapCM.SourceTexturePathNegY = "cubemap/cubemap_negy.png";
	cubemapCM.SourceTexturePathPosZ = "cubemap/cubemap_posz.png";
	cubemapCM.SourceTexturePathNegZ = "cubemap/cubemap_negz.png";
	_cubemap = CreateEntity();
	AddComponent<Ivy::Transform>(_cubemap, transformCM);
	AddComponent<Ivy::Mesh>(_cubemap, meshCM);
	AddComponent<Ivy::Cubemap>(_cubemap, cubemapCM);
	//*/

	/* Teapot
	auto transform = Ivy::Transform();
	transform.Position.Z = -50;
	auto mesh = Ivy::Mesh();
	mesh.SourceMeshPath = "teapot.obj";
	auto material = Ivy::Material();
	material.AlbedoPath = "bricks.png";
	//*/

	//* BHP
	auto transform = Ivy::Transform();
	transform.Position.Z = -50;
	auto mesh = Ivy::Mesh();
	mesh.SourceMeshPath = "BrowningHP\\BHP.obj";
	auto material = Ivy::Material();
	material.AlbedoPath = "BrowningHP\\BHPAlbedo.tga";
	material.NormalPath = "BrowningHP\\BHPNormal.tga";
	material.MetallicPath = "BrowningHP\\BHPMetallic.tga";
	material.RoughnessPath = "BrowningHP\\BHPRoughness.tga";
	//*/

	/* IceTool
	auto transform = Ivy::Transform();
	transform.Position.Z = -50;
	auto mesh = Ivy::Mesh();
	mesh.SourceMeshPath = "IceTool\\IceTool.obj";
	auto material = Ivy::Material();
	material.AlbedoPath = "IceTool\\ITAlbedo.png";
	material.NormalPath = "IceTool\\ITNormal.png";
	material.MetallicPath = "IceTool\\ITMetallic.png";
	material.RoughnessPath = "IceTool\\ITRoughness.png";
	//*/
	
	_object = CreateEntity();
	AddComponent<Ivy::Transform>(_object, transform);
	AddComponent<Ivy::Mesh>(_object, mesh);
	AddComponent<Ivy::Material>(_object, material);
}

Sandbox::~Sandbox()
{
	
}

void Sandbox::Tick()
{
	Ivy::Transform& transform = GetComponent<Ivy::Transform>(_object);
	transform.Position.Z = modelDistance;
	transform.Rotation.X = xModelRotation;
	transform.Rotation.Y = yModelRotation;
}

Ivy::Application* Ivy::CreateApplication(int argc, char* argv[])
{
	return new Sandbox(argc, argv);
}
