#pragma once

#ifdef IVY_BUILD_DLL
	#define IVY_API __declspec(dllexport)
#else
	#define IVY_API __declspec(dllimport)
#endif