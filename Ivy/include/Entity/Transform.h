#pragma once
#include "Core/IvyPCH.h"

namespace Ivy
{
	class IVY_API Transform
	{
	public:
		/* X, Y, & Z position in world space */
		class IVY_API Position
		{
		public:
			Position()
				: X(0), Y(0), Z(0) {}
			Position(float x, float y, float z)
				: X(x), Y(y), Z(z) {}
			float X, Y, Z;
		};
		/* Rotation about the X, Y, & Z axis in world space */
		class IVY_API Rotation
		{
		public:
			Rotation()
				: X(0), Y(0), Z(0) {}
			Rotation(float x, float y, float z)
				: X(x), Y(y), Z(z) {}
			float X, Y, Z;
		};
		Transform()
			: Position(), Rotation() {}
		Transform(Position position, Rotation rotation)
			: Position(position), Rotation(rotation) { }
		void SetPosition(float x, float y, float z) { Position.X = x; Position.Y = y; Position.Z = z; }
		void SetRotation(float x, float y, float z) { Rotation.X = x; Rotation.Y = y; Rotation.Z = z; }
		Position Position;
		Rotation Rotation;
	};
}