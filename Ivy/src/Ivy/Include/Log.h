#pragma once
#include <cstring>
#include <fstream>
#include <iostream>

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
#define LOG_INFO(m) do { } while(0)
#define LOG_WARN(m) do { } while(0)
#define LOG_ERROR(m) do { } while(0)
#define LOG_INFO_E(m, fi, fu, l) do { } while(0)
#define LOG_WARN_E(m, fi, fu, l) do { } while(0)
#define LOG_ERROR_E(m, fi, fu, l) do { } while(0)
#endif