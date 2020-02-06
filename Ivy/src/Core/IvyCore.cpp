#include "Core/IvyPCH.h"

#ifdef _DEBUG
std::ofstream LOGSTREAM = std::ofstream("Ivy.log", std::ofstream::out);
#endif

namespace _Ivy
{
	std::string GetRootDirectory()
	{
		// get path to current executable
		WCHAR raw[MAX_PATH_LENGTH];
		GetModuleFileName(NULL, raw, MAX_PATH_LENGTH);
		std::wstring wstr(raw);
		std::stringstream path(std::string(wstr.begin(), wstr.end()));

		// tokenize path
		std::string token;
		std::vector<std::string> tokens;
		while (std::getline(path, token, '\\'))
			tokens.push_back(token);

		// remove filename from path
		std::string root;
		for (int i = 0; i < ((int)tokens.size() - 1); i++)
			root += tokens[i] + "\\";
		
		return root;
	}

	std::string GetResourceDirectory()
	{
		return GetRootDirectory() + "Resource\\";
	}


	/* OpenGL Error Handling */

	void GLClearError()
	{
		while (glGetError() != GL_NO_ERROR);
	}

	bool GLStatus(const char* file, const char* function, const char* line)
	{
		bool status = true;
		while (GLenum e = glGetError())
		{
			LOG_ERROR_E("[OpenGL " << e << "]", file, function, line);
			status = false;
		}
		return status;
	}
}