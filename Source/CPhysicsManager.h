#pragma once

#include "CPhysicsDebugDraw.h"
#include "CSingleton.h"
#include "Box2D.h"
#include "common.h"
#include "structures.h"

namespace TTC
{
	class CActor;

	const float PHYSICS_DT = 0.02f;
	const int MAX_PHYSICS_TICKS = 5;
	class CPhysicsManager : public b2ContactListener, public CSingleton<CPhysicsManager>
	{
	public:
		struct ContactPoint
		{
			CActor *actor1, *actor2;
			b2Vec2 normal;
		};
		CPhysicsManager(void);
		~CPhysicsManager(void);

		//FIXME: Better structure
		void SetupWorld(const Box &box);
		void DestroyWorld();

		//physics stuff
		b2World *GetPhysicsWorld(){return mPhysicsWorld;}

		//contact listener
		void Add(const b2ContactPoint *point);

		void ResetDebug();
		void Update(float dt);
		void RenderDebug();

		void ToggleDebug();

	protected:
#ifdef _WIN32
		typedef stdext::hash_map<uint32, ContactPoint > CollisionsMapCfg;
#else
		typedef std::unordered_map<uint32, ContactPoint > CollisionsMapCfg;
#endif

		CollisionsMapCfg mCollisions;

		void SetupWorldBoundaries();

		float mPhysicsTick;
		b2World *mPhysicsWorld;
		CPhysicsDebugDraw mDebugDraw;
		bool mDebuggingEnabled;

		Box mBox;
	};
};
