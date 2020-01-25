#include "IvyPCH.h"
#include "Core.h"

namespace _Ivy
{
	/* Filesystem */

	std::string GetRootDirectory()
	{
		std::string root;
		std::stringstream path(IVY_PATH);
		std::string token;
		std::vector<std::string> tokens;

		while (std::getline(path, token, '\\'))
			tokens.push_back(token);

		for (int i = 0; i < ((int)tokens.size() - 4); i++)
			root += tokens[i] + "\\";

		return root;
	}

	std::string GetResourceDirectory()
	{
		return GetRootDirectory() + "resource\\";
	}


	/* OpenGL Error Handling */

	void GLClearError()
	{
		while (glGetError() != GL_NO_ERROR);
	}

	bool GLCheckError(const char* file, const char* function, const char* line)
	{
		bool error = false;
		while (GLenum e = glGetError())
		{
			LOG_ERROR_E("[OpenGL " << e << "]", file, function, line);
			error = true;
		}
		return error;
	}
}