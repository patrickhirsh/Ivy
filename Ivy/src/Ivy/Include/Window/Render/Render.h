#pragma once
#include "Core/IvyPCH.h"
#include "Object/Object.h"
#include "Object/Renderable/StaticMesh.h"


namespace Ivy
{
	class Render
	{
		template<typename T>
		using DrawRequestQueue = std::unordered_map<std::string, std::vector<Ivy::Ref<T>>>;

	public:
		Render									() {};
		~Render									() {};
		void DrawRequest						(Ivy::Ref<Ivy::StaticMesh> object);
		void ProcessRequests					(GLFWwindow* window);

		void SetSceneTranslation				(const cy::Vec3f& translation);
		void SetSceneRotation					(float angleX, float angleY, float angleZ);

	private:
		DrawRequestQueue<Ivy::StaticMesh>		_staticMeshDrawRequests;

		cy::Matrix4f							_sceneTranslation = cy::Matrix4f::Identity();
		cy::Matrix4f							_sceneRotation = cy::Matrix4f::Identity();
		cy::Matrix4f							_projection;
		cy::Matrix4f							_model;
		cy::Matrix4f							_view;
	};
}