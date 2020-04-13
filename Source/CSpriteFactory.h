#pragma once

#include "CSingleton.h"
#include "CSprite.h"

#include "common.h"

namespace TTC
{

	class CSpriteFactory : public CSingleton<CSpriteFactory>
	{
	public:
		CSpriteFactory(void);
		~CSpriteFactory(void);

		CSprite *Load(std::string type, std::string name, std::string filename, float width, float height, int columns=1, int rows=1, int numStates=1, int initialState=0, CSprite::SpriteFrameInfo *spritesInfo=NULL);
		//CSprite *Load(std::string name, std::string filename, float width, float height, int numStates=1, int framesPerState=1, float fps=1, int initialState=0);
		CSprite *Get(std::string type, std::string name);

		void Clear();
	private:

#ifdef _WIN32
		typedef stdext::hash_map<std::string, CSprite* > SpritesMapCfg;
#else
		typedef std::unordered_map<std::string, CSprite* > SpritesMapCfg;
#endif
		SpritesMapCfg mSprites;
	};
};
