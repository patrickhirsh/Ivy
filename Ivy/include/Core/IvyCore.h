#pragma once
#include <cstring>
#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <string.h>
#include <vector>

namespace Ivy
{
	// Unique Pointer Implemetation
	template<typename T>
	using Scope = std::unique_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Scope<T> CreateScope(Args&& ... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	// Shared Pointer Implemetation
	template<typename T>
	using Ref = std::shared_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Ref<T> CreateRef(Args&& ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}

	// Weak Pointer Implemetation
	template<typename T>
	using WeakRef = std::weak_ptr<T>;
}


namespace _Ivy
{
	/* Filesystem */
	std::string GetRootDirectory();
	std::string GetResourceDirectory();

	/* OpenGL Error Handling */
	void GLClearError();
	bool GLStatus(const char* file, const char* function, const char* line);
}


/* MACROS */

// Filesystem Constants
#define MAX_PATH_LENGTH 1024

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
 x; ASSERT(_Ivy::GLStatus(__FILE__, #x, std::to_string(__LINE__).c_str())); } while(0)
#else
#define ASSERT(x)
#define GL(x) x
#endif

// Low-Level Data Manipulation
#define BIT(x) (1 << x)

// Logging
#ifdef _DEBUG
extern std::ofstream LOGSTREAM;
#define _LOG_META "[" << __FILE__ << "][line " << __LINE__ << "]"
#define _LOG_META_E(fi, fu, l) "[" << fi << "][" << fu << "][line " << l << "]"
#define _LOG(s) do { LOGSTREAM << s; std::cout << s; } while(0)
#define LOG_SYS(m) _LOG(" [SYSTEM] " << m << std::endl)
#define LOG_INFO(m) _LOG("          " << _LOG_META << " : " << m << std::endl)	// Ivy log info
#define LOG_WARN(m) _LOG("[WARNING] " << _LOG_META << " : " << m << std::endl)	// Ivy log warning
#define LOG_ERROR(m) _LOG(" [ERROR]  " << _LOG_META << " : " << m << std::endl) // Ivy log error
#define LOG_INFO_E(m, fi, fu, l) _LOG("          " << _LOG_META_E(fi, fu, l) << " : " << m << std::endl)	// Ivy log info (extend info)
#define LOG_WARN_E(m, fi, fu, l) _LOG("[WARNING] " << _LOG_META_E(fi, fu, l) << " : " << m << std::endl)	// Ivy log warning (extend info)
#define LOG_ERROR_E(m, fi, fu, l) _LOG(" [ERROR]  " << _LOG_META_E(fi, fu, l) << " : " << m << std::endl)	// Ivy log error (extend info)
#else
#define LOG_SYS(m) do { } while(0)
#define LOG_INFO(m) do { } while(0)
#define LOG_WARN(m) do { } while(0)
#define LOG_ERROR(m) do { } while(0)
#define LOG_INFO_E(m, fi, fu, l) do { } while(0)
#define LOG_WARN_E(m, fi, fu, l) do { } while(0)
#define LOG_ERROR_E(m, fi, fu, l) do { } while(0)
#endif