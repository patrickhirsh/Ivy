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
		std::string SourcePath;

		// TODO: move a bunch of this into a material class
		bool						_loaded;
		std::string					_source;
		bool						_hasVertexPositions = false;
		bool						_hasVertexNormals = false;
		bool						_hasVertexTextures = false;
		std::vector<float>			_buffer;
		GLuint						_texture;
		unsigned char*				_textureData;
		int							_textureWidth;
		int							_textureHeight;
		int							_textureNRChannels;
		_Ivy::VertexBuffer			_vb;
		_Ivy::VertexArray			_va;
		_Ivy::VertexBufferLayout	_vbLayout;
	};
}