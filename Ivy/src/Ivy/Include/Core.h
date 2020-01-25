#pragma once
#include <memory>
#include <sstream>
#include <string.h>
#include <vector>

#define IVY_PATH __FILE__

/* IVY PUBLIC */
namespace Ivy
{
	/* Typedefs */
	template<typename T>
	using Scope = std::unique_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Scope<T> CreateScope(Args&& ... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using Ref = std::shared_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Ref<T> CreateRef(Args&& ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}
}


/* IVY INTERNAL */
namespace _Ivy
{
	/* Filesystem */
	std::string GetRootDirectory();
	std::string GetResourceDirectory();

	/* OpenGL Error Handling */
	void GLClearError();
	bool GLCheckError(const char* file, const char* function, const char* line);
}


/* MACROS */

// DLL Export
#ifdef IVY_BUILD_DLL
#define IVY_API __declspec(dllexport)
#else
#define IVY_API __declspec(dllimport)
#endif

// OpenGL Error Handling
#ifdef _DEBUG
#define ASSERT(x) do { if (!(x)) __debugbreak(); } while(0)
#define GL(x) do { _Ivy::GLClearError();\
 x; ASSERT(_Ivy::GLCheckError(__FILE__, #x, std::to_string(__LINE__).c_str())); } while(0)
#else
#define ASSERT(x) x
#define GL(x) x
#endif