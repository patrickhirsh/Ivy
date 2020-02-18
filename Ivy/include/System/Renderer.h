#pragma once
#include "Core/IvyPCH.h"
#include "System/System.h"
#include "Window/Window.h"
#include "Component/Component.h"



namespace _Ivy
{
	class Renderer : public System
	{
	public:
		void Update(Ivy::Ref<Window> window, Ivy::Ref<ECS> ecs);
		Renderer() {};
		~Renderer() {};

	private:
		void BindVAO(GLuint vao);
		void BindVBO(GLuint vbo);
		void BindTexture(GLuint tbo, int TextureWidth, int TextureHeight, unsigned char* TextureData);
		void UnbindAll();
		void LoadMesh(Ivy::Mesh& mesh);
	};
}