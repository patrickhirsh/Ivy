#include "Sandbox.h"

// state
bool shouldClose = false;
bool shouldDrawCubemap = false;

// mouse state
bool mb1Active = false;
bool mb2Active = false;
double mb1CursorX = 0;
double mb1CursorY = 0;
double mb2CursorX = 0;

// active model
Ivy::Entity _activeObject;
cy::Matrix4f modelRotation;
double modelDistance;

// models
Ivy::Entity _BHP;
Ivy::Entity _Rock;
Ivy::Entity _Car;
Ivy::Entity _Stone;

// cubemaps
Ivy::Entity _cubemap;

void InputCallback(Ivy::Event& Event)
{
	if (Event.GetEventType() == Ivy::EventType::E_KEY_PRESSED)
	{
		Ivy::EventKeyPressed e = Event.As<Ivy::EventKeyPressed>();
		if (e.GetKey() == GLFW_KEY_ESCAPE)
			shouldClose = true;
		if (e.GetKey() == GLFW_KEY_C)
			shouldDrawCubemap = !shouldDrawCubemap;

		// BHP
		if (e.GetKey() == GLFW_KEY_1)
		{
			_activeObject = _BHP;
			modelRotation = cy::Matrix4f::RotationXYZ(0, 0, 0);
			modelDistance = -1000;
		}

		/*

		// Rock
		if (e.GetKey() == GLFW_KEY_2)
		{
			_activeObject = _Rock;
			modelRotation = cy::Matrix4f::RotationXYZ(0, 0, 0);
			modelDistance = -80;
		}

		// Car
		if (e.GetKey() == GLFW_KEY_3)
		{
			_activeObject = _Car;
			modelRotation = cy::Matrix4f::RotationXYZ(0, 0, 0);
			modelDistance = -80;
		}

		// Stone
		if (e.GetKey() == GLFW_KEY_4)
		{
			_activeObject = _Stone;
			modelRotation = cy::Matrix4f::RotationXYZ(1.5, 1.5, 0);
			modelDistance = -60;
		}
		*/
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
			modelRotation *= cy::Matrix4f::RotationXYZ(
				(mb1CursorY - e.GetPosY()) * 0.005f, 
				(e.GetPosX() - mb1CursorX) * 0.005f,
				0);
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
	RegisterEventCallback(Ivy::EventCategory::C_INPUT, BIND_EVENT_FUNCTION(InputCallback));

	/*
	// Cubemap
	auto transformCM = Ivy::Transform();
	transformCM.Rotation = cy::Matrix4f::RotationXYZ(-50, 0, 0);
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

	//*
	// Cubemap2
	auto transformCM = Ivy::Transform();
	transformCM.Rotation = cy::Matrix4f::RotationXYZ(0, 0, 0);
	auto meshCM = Ivy::Mesh();
	meshCM.SourceMeshPath = "cube.obj";
	auto cubemapCM = Ivy::Cubemap();
	cubemapCM.SourceTexturePathPosX = "cubemap2/posx.jpg";
	cubemapCM.SourceTexturePathNegX = "cubemap2/negx.jpg";
	cubemapCM.SourceTexturePathPosY = "cubemap2/posy.jpg";
	cubemapCM.SourceTexturePathNegY = "cubemap2/negy.jpg";
	cubemapCM.SourceTexturePathPosZ = "cubemap2/posz.jpg";
	cubemapCM.SourceTexturePathNegZ = "cubemap2/negz.jpg";
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
	material.AlbedoPath = "brick.png";
	//*/

	/* IceTool
	auto transform = Ivy::Transform();
	auto mesh = Ivy::Mesh();
	mesh.SourceMeshPath = "IceTool\\IceTool.obj";
	auto material = Ivy::Material();
	material.AlbedoPath = "IceTool\\ITAlbedo.png";
	material.NormalPath = "IceTool\\ITNormal.png";
	material.MetallicPath = "IceTool\\ITMetallic.png";
	material.RoughnessPath = "IceTool\\ITRoughness.png";
	//*/

	//* BHP
	auto transformBHP = Ivy::Transform();
	auto meshBHP = Ivy::Mesh();
	meshBHP.SourceMeshPath = "BrowningHP\\BHP.obj";
	auto materialBHP = Ivy::Material();
	materialBHP.AlbedoPath = "BrowningHP\\BHPAlbedo.tga";
	materialBHP.NormalPath = "BrowningHP\\BHPNormal.tga";
	materialBHP.MetallicPath = "BrowningHP\\BHPMetallic.tga";
	materialBHP.RoughnessPath = "BrowningHP\\BHPRoughness.tga";

	_BHP = CreateEntity();
	AddComponent<Ivy::Transform>(_BHP, transformBHP);
	AddComponent<Ivy::Mesh>(_BHP, meshBHP);
	AddComponent<Ivy::Material>(_BHP, materialBHP);
	//*/

	/* Rock
	auto transformRock = Ivy::Transform();
	transformRock.Position.Z = 20;
	auto meshRock = Ivy::Mesh();
	meshRock.SourceMeshPath = "Rock\\Rock_1.obj";
	auto materialRock = Ivy::Material();
	materialRock.AlbedoPath = "Rock\\Rock_1_Albedo.jpg";
	materialRock.NormalPath = "Rock\\Rock_1_Normal.jpg";
	materialRock.MetallicPath = "Rock\\Rock_1_Specular.jpg";
	materialRock.RoughnessPath = "Rock\\Rock_1_Roughness.jpg";

	_Rock = CreateEntity();
	AddComponent<Ivy::Transform>(_Rock, transformRock);
	AddComponent<Ivy::Mesh>(_Rock, meshRock);
	AddComponent<Ivy::Material>(_Rock, materialRock);
	//*/

	/* Car
	auto transformCar = Ivy::Transform();
	transformCar.Position.Z = 20;
	auto meshCar = Ivy::Mesh();
	meshCar.SourceMeshPath = "Car\\falc_low.obj";
	auto materialCar = Ivy::Material();
	materialCar.AlbedoPath = "Car\\falc_Albedo.png";
	materialCar.NormalPath = "Car\\falc_Normal.png";
	materialCar.MetallicPath = "Car\\falc_MetallicSmoothness.png";
	materialCar.RoughnessPath = "Car\\falc_MetallicSmoothness.png";

	_Car = CreateEntity();
	AddComponent<Ivy::Transform>(_Car, transformCar);
	AddComponent<Ivy::Mesh>(_Car, meshCar);
	AddComponent<Ivy::Material>(_Car, materialCar);
	//*/

	/* Stone
	auto transformStone = Ivy::Transform();
	transformStone.Position.Z = 20;
	auto meshStone = Ivy::Mesh();
	meshStone.SourceMeshPath = "Stone\\stone.obj";
	auto materialStone = Ivy::Material();
	materialStone.AlbedoPath = "Stone\\stone_albedo.jpg";
	materialStone.NormalPath = "Stone\\stone_normal.jpg";
	materialStone.MetallicPath = "Stone\\stone_specular.jpg";
	materialStone.RoughnessPath = "Stone\\stone_roughness.jpg";

	_Stone = CreateEntity();
	AddComponent<Ivy::Transform>(_Stone, transformStone);
	AddComponent<Ivy::Mesh>(_Stone, meshStone);
	AddComponent<Ivy::Material>(_Stone, materialStone);
	//*/

	_activeObject = _BHP;
	modelRotation = cy::Matrix4f::RotationXYZ(0, 0, 0);
	modelDistance = -1000;
}

Sandbox::~Sandbox()
{
	
}

void Sandbox::Tick()
{
	// Super hacky and temporary. Clear all visibility, then set our current active as visible
	Ivy::Transform& transformBHP = GetComponent<Ivy::Transform>(_BHP);
	transformBHP.IsVisible = false;
	/*
	Ivy::Transform& transformRock = GetComponent<Ivy::Transform>(_Rock);
	transformRock.IsVisible = false;
	Ivy::Transform& transformCar = GetComponent<Ivy::Transform>(_Car);
	transformCar.IsVisible = false;
	Ivy::Transform& transformStone = GetComponent<Ivy::Transform>(_Stone);
	transformStone.IsVisible = false;
	*/

	Ivy::Transform& transformACTIVE = GetComponent<Ivy::Transform>(_activeObject);
	transformACTIVE.IsVisible = true;

	// Object movement
	Ivy::Transform& transform = GetComponent<Ivy::Transform>(_activeObject);
	transform.Position.Z = modelDistance;
	transform.Rotation = modelRotation;

	// Cubemap
	Ivy::Transform& transformCM = GetComponent<Ivy::Transform>(_cubemap);
	transformCM.IsVisible = shouldDrawCubemap;

}

Ivy::Application* Ivy::CreateApplication(int argc, char* argv[])
{
	return new Sandbox(argc, argv);
}
