#include "CSpriteFactory.h"

#include "utils.h"
#include "CConfig.h"
#include "CLog.h"

using namespace TTC;

CSpriteFactory::CSpriteFactory(void)
{
}

CSpriteFactory::~CSpriteFactory(void)
{
}


CSprite *CSpriteFactory::Load(std::string type, std::string name, std::string filename,
					   float width, float height, int columns, int rows,
//					   int numStates, int framesPerState, float fps, int initialState)
						int numStates, int initialState, CSprite::SpriteFrameInfo *states)
{
 	CLog::Get().Debug("Finding Sprite: " + type + "/" + name);
	//check it doesn't already exist
	//std::map<std::string, CSprite* >::iterator it;
	SpritesMapCfg::iterator it;
	std::string key = type + "," + name;
	it = mSprites.find(key);
	if (it != mSprites.end())
		return it->second;

	std::string realFilename = TTC_CFG_GET_STR("spritePath") + filename;
	CLog::Get().Debug("  Sprite not cached - loading from " + realFilename);
	//CSprite *sprite = new CSprite(filename, width, height, numStates, framesPerState, fps, initialState);
	CSprite *sprite = new CSprite(realFilename, width, height, columns, rows, numStates, initialState, states);
	mSprites[key] = sprite;
	return sprite;
}

/* FIXME: I will probably need to eventually externalise frame information
CSprite *CSpriteFactory::Load(std::string type, std::string name, ticpp::Element *spriteNode)
{
	//check it doesn't already exist
	SpritesMapCfg::iterator it;
	std::string key = type + "," + name;
	it = mSprites.find(key);
	if (it != mSprites.end())
		return it->second;

	//load it
	float w=0, h=0;
	int columns, rows, initialState;
	float fps=0;
	ticpp::Element *el;

	//dimensions (in all sprites)
	w = spriteNode->GetAttribute<float>("width");
	h = spriteNode->GetAttribute<float>("height");
	el = spriteNode->FirstChildElement("image");
	std::string filename(el->GetAttribute("filename"));

	//animation (not in all sprites)
	ticpp::Element *animation = spriteNode->FirstChildElement("animation",false);
	if (animation)
	{
		columns = animation->GetAttribute<int>("columns");
		rows = animation->GetAttribute<int>("rows");
		initialState = animation->GetAttribute<int>("initialstate");

		//TODO: count number of states (surely there's a function for this?)
		int numStates = 0;
		ticpp::Iterator<ticpp::Element> state("state");
		for(state=state.begin(animation); state != state.end(); ++state)
		{
			numStates++;
		}

		CSprite::SpriteFrameInfo *states = new CSprite::SpriteFrameInfo[numStates];
		int index;
		for(state=state.begin(animation); state != state.end(); ++state)
		{
			index = state->GetAttribute<int>("index");
			if (index >= numStates)
			{
				throw(Exception("Index must be less than the number of states: " + type + "/" + name,__FILE__, __LINE__));
			}

			//states[index].index = index;
			states[index].x = state->GetAttribute<float>("x");
			states[index].y = state->GetAttribute<float>("y");
			states[index].w = state->GetAttribute<float>("w");
			states[index].h = state->GetAttribute<float>("h");
			states[index].srcw = state->GetAttribute<float>("srcw");
			states[index].srch = state->GetAttribute<float>("srch");
			states[index].frames = state->GetAttribute<int>("frames");
			states[index].fps = state->GetAttribute<float>("fps");
			states[index].name = state->GetText();
		}

		return Load(type, name, filename, w, h, columns, rows, numStates, initialState, states);
		//return Load(name, filename, w, h, rows, columns, fps, initialState);
	}
	else
	{
		//load without animation
		return Load(type, name, filename, w, h);
	}
}
*/

CSprite *CSpriteFactory::Get(std::string type, std::string name)
{
	//check it doesn't already exist
	SpritesMapCfg::iterator it;
	std::string key = type + "," + name;
	it = mSprites.find(key);
	if (it == mSprites.end())
		return NULL;

	//make a copy
	return new CSprite(it->second);
}

void CSpriteFactory::Clear()
{
	SpritesMapCfg::iterator it = mSprites.begin();
	while (it != mSprites.end())
	{
		delete it->second;
		++it;
	}
	mSprites.clear();
}