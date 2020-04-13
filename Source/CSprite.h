#pragma once

#include "common.h"
#include <string>
#include <vector>

namespace TTC
{
	class CSprite
	{
	public:
		struct SpriteFrameInfo {
			float x, y, w, h, srcw, srch;
			int frames;
			float fps;
			std::string name;
			int displayListId;
		};

		CSprite(std::string filename, float width, float height, int columns=1, int rows=1, int numStates=1, int initialState=0, SpriteFrameInfo *frames=NULL);
		//CSprite(std::string filename, float width, float height, int numStates=1, int framesPerState=1, float fps=1, int initialState=0);
		CSprite(CSprite *other);
		~CSprite(void);

		inline GLuint GetTexId(){return mTexId;}
		void AdvanceFrame(float dt, bool loop=true);
		inline void ResetFrame(){mFrame=0;};

		inline void SetState(int state){mState = state;}
		inline int GetState(){return mState;}
		void Draw();

		float GetWidth(){return mWidth;}
		float GetHeight(){return mHeight;}

	protected:
		CSprite(){}

	private:
		GLuint mTexId;
		float mWidth;
		float mHeight;
		int mNumStates;

		int mState;
		float mFrame;
		SpriteFrameInfo *mFrames;

	};
}
