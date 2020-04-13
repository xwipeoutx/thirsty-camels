#pragma once

//fixme #include "CSprite.h"
#include "IPhysicsObject.h"

namespace TTC
{
	class CActor : public IPhysicsObject
	{
	public:

		CActor();
		virtual ~CActor(void);

		virtual void HandleEvent(const SDL_Event &e){}
		virtual bool Update(float dt){return true;}
		virtual void Draw(){}

		inline bool GetDying(){return mDying;}
		inline void SetDying(bool dying){mDying=dying;}
	protected:
		bool mDying;

	};
};
