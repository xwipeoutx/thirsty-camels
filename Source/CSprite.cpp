#include "CSprite.h"

#include "utils.h"
#include "common.h"

using namespace TTC;

CSprite::CSprite(std::string filename, 
			  float width, float height, int columns, int rows,
			  int numStates, int initialState, SpriteFrameInfo *frames)
: mWidth(width), mHeight(height), 
mNumStates(numStates),  mState(initialState), mFrame(0)
{
	//Create a surface for the image
	SDL_Surface *surface = TTC::ImageLoad(filename);
	if (surface == NULL) throw("Could not load sprite image");
	mTexId = SDL_GL_SurfaceToTexture(surface);
	SDL_FreeSurface(surface);

	GLuint numFrames=0;

	if (frames == NULL)
	{
		mFrames = new SpriteFrameInfo[1];
		mFrames[0].fps = 0;
		mFrames[0].frames = 1;
		mFrames[0].x = 0;
		mFrames[0].y = 0;
		mFrames[0].w = width;
		mFrames[0].h = height;
		mFrames[0].srcw = 1;
		mFrames[0].srch = 1;
		mFrames[0].name = std::string("Static");
		numFrames = 1;
	}
	else
	{
		mFrames = new SpriteFrameInfo[numStates];
		for (int i=0; i<numStates; i++)
		{
			mFrames[i].fps = frames[i].fps;
			mFrames[i].frames = frames[i].frames;
			//mFrames[i].index = frames[i].index;
			mFrames[i].name = frames[i].name;
			//mFrames[i].startFrame = frames[i].startFrame;

			mFrames[i].x = frames[i].x;
			mFrames[i].y = frames[i].y;
			mFrames[i].w = frames[i].w;
			mFrames[i].h = frames[i].h;
			mFrames[i].srcw = frames[i].srcw;
			mFrames[i].srch = frames[i].srch;
			numFrames += mFrames[i].frames;
		}
	}

	//generate display lists
	int displayListId = glGenLists(numFrames);

	//float dx = 1.f / columns;
	//float dy = 1.f / rows;

	//float xd = 0.5f*mWidth;
	//float yd = 0.5f*mHeight;

	for (int i=0; i<numStates; i++)
	{
		mFrames[i].displayListId = displayListId;
		//int ix = mFrames[i].startFrame % columns;
		//int iy = mFrames[i].startFrame / columns;
		float x = mFrames[i].x;
		float y = mFrames[i].y;
		float w = mFrames[i].srcw;
		float h = mFrames[i].srch;
		float xd = 0.5f * mFrames[i].w;
		float yd = 0.5f * mFrames[i].h;


		for (int j=0; j<mFrames[i].frames; j++)
		{
			glNewList(displayListId++, GL_COMPILE);
			{
				glBegin(GL_QUADS);
				{
					glTexCoord2f(x, 1-y-h);
					glVertex3f( -xd, -yd, 0 );

					glTexCoord2f(x+w,1-y-h);
					glVertex3f(  xd,  -yd, 0 ); 

					glTexCoord2f(x+w,1-y);
					glVertex3f(  xd,   yd, 0 ); 

					glTexCoord2f(x,1-y);
					glVertex3f( -xd,  yd, 0 ); 
				}
				glEnd(); 
			}
			glEndList();
			checkGLError("Creating List");

			x = x + w;
			if (x+w > 1.f) 
			{
				x = 0;
				y = y + h;
				if (y+h > 1.f)
					y = 0;
			}
		}
	}
}

CSprite::CSprite(CSprite *other)
{
	mTexId = other->mTexId;
	mWidth = other->mWidth;
	mHeight = other->mHeight;
	mNumStates = other->mNumStates;
	mFrame = other->mFrame;
	mState = other->mState;
	mFrames = other->mFrames;
}

CSprite::~CSprite(void)
{
	return;
	/*FIXME: Check Sprite Allocations
	std::map<std::string, std::pair<int, GLuint> >::iterator it;
	it = sSpriteAllocations.find(mFilename);
	if (it != sSpriteAllocations.end())
	{
		if(--it->second.first == 0)
		{
			//delete the texture and iterator
			glDeleteTextures(1, &mTexId);
			sSpriteAllocations.erase(it);
		}
	}
	*/
}


void CSprite::AdvanceFrame(float dt, bool loop)
{
	mFrame += dt*mFrames[mState].fps;
	if  (loop)
	{
		if (mFrame > mFrames[mState].frames-0.01f) mFrame = 0;
		else if ( mFrame < 0) mFrame = (float)mFrames[mState].frames-0.01f;
	}
	else
	{
		if (mFrame > mFrames[mState].frames-0.01f) mFrame = (float)mFrames[mState].frames-0.01f;
		else if ( mFrame < 0) mFrame = 0;
	}
}

void CSprite::Draw()
{
	glColor3f(1,1,1);
	glBindTexture(GL_TEXTURE_2D,mTexId);

	GLuint offset = mFrames[mState].displayListId + (GLuint)(mFrame);

	glCallList(offset);
}