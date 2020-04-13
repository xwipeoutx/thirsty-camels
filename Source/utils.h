#pragma once

#include "common.h"
#include <string>
#include <sstream>
#include <vector>

namespace TTC
{
	SDL_Surface *ImageLoad( std::string filename );

	GLuint SDL_GL_SurfaceToTexture(SDL_Surface * surface);

	inline void checkGLError(const char *label)
	{
		GLenum errCode;
		const GLubyte *errStr;
		if ((errCode = glGetError()) != GL_NO_ERROR)
		{
			errStr = gluErrorString(errCode);
			exit(1);
		}
	}

	template <class _T>
	std::string ToString(const _T &val)
	{
		std::ostringstream str;
		str << val;
		return str.str();
	}


};