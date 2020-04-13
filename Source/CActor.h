#pragma once

#include "CSprite.h"
#include "CPhysicsManager.h"

#include "common.h"

namespace TTC
{
	class CActor
	{
	public:
		enum ActorFlags {
			ACTOR_NONE = 0,
			ACTOR_ALL = ~0,
			ACTOR_PLAYER = 1 << 1,
			ACTOR_BALL = 1 << 2
		};

		CActor();
		virtual ~CActor(void);

		virtual void HandleEvent(const SDL_Event &e){}
		virtual bool Update(float dt){return true;}
		virtual void Draw(){}

		virtual void Collide(CActor *other, const CPhysicsManager::ContactPoint &cp, int pos){}

		inline bool GetDying(){return mDying;}
		inline void SetDying(bool dying){mDying=dying;}

		inline ActorFlags GetFlags() const {return mFlags;}
		inline void SetFlags(ActorFlags flags){mFlags = flags;}
		inline bool IsFlagged(ActorFlags flags) const {return (flags & mFlags) > 0;}
	protected:
		ActorFlags mFlags;
		bool mDying;

		b2Body *mBody;

	};
};
