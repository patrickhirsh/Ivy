#pragma once
#include "Core/IvyPCH.h"
#include "Data/Object.h"


namespace _Ivy
{
	class Render
	{
	public:
		Render									() {};
		~Render									() {};
		void DrawRequest						(Ivy::Ref<Ivy::Object> object);
		void ProcessRequests					(GLFWwindow* window);

		void SetSceneTranslation				(const cy::Vec3f& translation);
		void SetSceneRotation					(float angleX, float angleY, float angleZ);

	private:
		std::vector<Ivy::Ref<Ivy::Object>>		_drawRequests;

		cy::Matrix4f							_sceneTranslation = cy::Matrix4f::Identity();
		cy::Matrix4f							_sceneRotation = cy::Matrix4f::Identity();
		cy::Matrix4f							_projection;
		cy::Matrix4f							_model;
	};
}