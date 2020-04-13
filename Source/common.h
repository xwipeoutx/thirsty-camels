#pragma once

//OpenGL
//NOTE: WE don't need SDL_opengl any more, indeed it gets screwy because of glew
#include "GL/glew.h"
#include <cg/cg.h>
#include <cg/cgGL.h>

//SDL
#ifdef _WIN32
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "SDL_mixer.h"
#include <hash_map>
#else
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_ttf.h"
#include "SDL/SDL_mixer.h"
#include <unordered_map>
#endif

//CEGUI
#include "CEGUI.h"
#include "CEGUIDefaultResourceProvider.h"
#include "RendererModules/OpenGLGUIRenderer/openglrenderer.h"

//Internal useful stuff - exceptions and log
#include "Exception.h"
#include "CLog.h"
#include "utils.h"