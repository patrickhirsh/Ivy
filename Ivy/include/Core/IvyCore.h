#pragma once
#include <cstring>
#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <string.h>
#include <vector>

#define IVY_VERSION "1.1.0"

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

// Exit Codes
#define IVY_OK 0
#define IVY_FATAL 1

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
#define LOG_INFO(m) _LOG("          " << _LOG_META << " : " << m << std::endl)	// Ivy log info
#define LOG_WARN(m) _LOG("[WARNING] " << _LOG_META << " : " << m << std::endl)	// Ivy log warning
#define LOG_ERROR(m) _LOG(" [ERROR]  " << _LOG_META << " : " << m << std::endl) // Ivy log error
#define LOG_INFO_E(m, fi, fu, l) _LOG("          " << _LOG_META_E(fi, fu, l) << " : " << m << std::endl)	// Ivy log info (extend info)
#define LOG_WARN_E(m, fi, fu, l) _LOG("[WARNING] " << _LOG_META_E(fi, fu, l) << " : " << m << std::endl)	// Ivy log warning (extend info)
#define LOG_ERROR_E(m, fi, fu, l) _LOG(" [ERROR]  " << _LOG_META_E(fi, fu, l) << " : " << m << std::endl)	// Ivy log error (extend info)
#else
#define LOG_INFO(m) do { } while(0)
#define LOG_WARN(m) do { } while(0)
#define LOG_ERROR(m) do { } while(0)
#define LOG_INFO_E(m, fi, fu, l) do { } while(0)
#define LOG_WARN_E(m, fi, fu, l) do { } while(0)
#define LOG_ERROR_E(m, fi, fu, l) do { } while(0)
#endif

// Sys Logging
#ifdef _DEBUG
#define LOG_SYS(m) _LOG(" [SYSTEM] " << m << std::endl)
#define LOG_INIT_SYS_HEADER(m) LOG_SYS("  <" << m << ">")
#define LOG_INIT_SYS_SUB_HEADER(m) LOG_SYS("    <" << m << ">")
#define LOG_INIT_SYS_ENTRY(m) LOG_SYS("    " << m)
#define LOG_INIT_SYS_SUB_ENTRY(m) LOG_SYS("      " << m)
#define LOG_INIT_SYS_BEGIN do { \
LOG_SYS("=============== Ivy Engine Startup ===============");\
LOG_SYS("Version: " << IVY_VERSION);\
} while(0);
#define LOG_INIT_SYS_END do { \
LOG_SYS("==================================================");\
} while(0);
#define LOG_SYSTEM_STARTUP() do { \
LOG_INIT_SYS_BEGIN \
LOG_INIT_SYS_HEADER("OpenGL"); \
LOG_INIT_SYS_ENTRY("Hardware Vendor:    " << (glGetString(GL_VENDOR) == GL_NONE ? "N/A" : (const char*)glGetString(GL_VENDOR))); \
LOG_INIT_SYS_ENTRY("Hardware Renderer:  " << (glGetString(GL_RENDERER) == GL_NONE ? "N/A" : (const char*)glGetString(GL_RENDERER))); \
LOG_INIT_SYS_ENTRY("OpenGL Version:     " << (glGetString(GL_VERSION) == GL_NONE ? "N/A" : (const char*)glGetString(GL_VERSION))); \
LOG_INIT_SYS_END } while(0)
#else
#define LOG_SYS(m) do { } while(0)
#define LOG_INIT_SYS_HEADER(m) do { } while(0)
#define LOG_INIT_SYS_SUB_HEADER(m) do { } while(0)
#define LOG_INIT_SYS_ENTRY(m) do { } while(0)
#define LOG_INIT_SYS_SUB_ENTRY(m) do { } while(0)
#define LOG_INIT_SYS_BEGIN
#define LOG_INIT_SYS_END
#define LOG_SYSTEM_STARTUP()
#endif