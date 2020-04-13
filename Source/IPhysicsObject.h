#pragma once

#include "CPhysicsManager.h"

#include "common.h"

namespace TTC
{
	class IPhysicsObject
	{
	public:
		enum PhysicsFlags {
			PHYSICS_NONE = 0,
			PHYSICS_ALL = ~0,
			PHYSICS_PLAYER = 1 << 1,
			PHYSICS_BALL = 1 << 2,
			PHYSICS_ZONE = 1 << 3,
			PHYSICS_STATIONARY = 1 << 3,
		};

		IPhysicsObject() 
			: mFlags(PHYSICS_NONE), mBody(NULL) {}

		virtual void Collide(IPhysicsObject *other, const CPhysicsManager::ContactPoint &cp, int pos)=0;

		inline PhysicsFlags GetFlags() const {return mFlags;}
		inline void SetFlags(PhysicsFlags flags){mFlags = flags;}
		inline bool IsFlagged(PhysicsFlags flags) const {return (flags & mFlags) > 0;}
	protected:
		PhysicsFlags mFlags;
		b2Body *mBody;

		void *AsB2Data()
		{
			return static_cast<void*>(static_cast<IPhysicsObject*>(this));
		}
	};
};