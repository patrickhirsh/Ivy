#pragma once
#include "Core/IvyPCH.h"
#include "Render/VertexArray.h"
#include "Render/VertexBuffer.h"

namespace _Ivy
{
	using ComponentType = unsigned char;
	const ComponentType MAX_COMPONENTS = 32;
	using ComponentSignature = std::bitset<MAX_COMPONENTS>;
}


namespace Ivy
{
	class IVY_API Component
	{
		
	};

	class IVY_API Transform : public Component
	{
	public:

		class IVY_API Position
		{
		public:
			float X, Y, Z;
		};

		class IVY_API Rotation
		{
		public:
			float X, Y, Z;
		};

		Position Position;
		Rotation Rotation;
	};

	class IVY_API Mesh : public Component
	{
	public:
		bool						Loaded = false;
		std::string					SourceMeshPath;
		bool						HasVertexPositions = false;
		bool						HasVertexNormals = false;
		bool						HasVertexTextures = false;
		GLuint						VBOSize;
		GLuint						VBO;
		GLuint						VAO;
	};

	class IVY_API Material : public Component
	{
	public:
		bool						Loaded = false;
		std::string					SourceTexturePath;
		unsigned char*				TextureData;
		int							TextureWidth;
		int							TextureHeight;
		int							TextureNRChannels;
		GLuint						TBO;
		float						Metallic;
		float						Roughness;
		float						AO;
	};

	class IVY_API Cubemap : public Component
	{
	public:
		bool						Loaded = false;
		std::string					SourceTexturePathPosX;
		std::string					SourceTexturePathNegX;
		std::string					SourceTexturePathPosY;
		std::string					SourceTexturePathNegY;
		std::string					SourceTexturePathPosZ;
		std::string					SourceTexturePathNegZ;
		std::vector<unsigned char*> TextureData;
		int							TextureWidth;
		int							TextureHeight;
		GLuint						TBO;
	};
}