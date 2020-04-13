#include "CPhysicsManager.h"
#include "CActor.h"

#include "CLog.h"

using namespace TTC;
using namespace std;

CPhysicsManager::CPhysicsManager()
{
	mPhysicsWorld = NULL;
}

CPhysicsManager::~CPhysicsManager(void)
{
	if (mPhysicsWorld)
		DestroyWorld();
}

void CPhysicsManager::SetupWorld(const Box &box)
{
	CLog::Get().Info("Setting up physics world");
	mBox = box;
	float x=mBox.x, y=mBox.y, w=mBox.w, h=mBox.h;
	float hext=0.5;
	mPhysicsTick = 0;
	//set up the physics world
	//make the AABB world twice as large as the region, for safety sake
	b2AABB worldAABB;
	worldAABB.lowerBound.Set(x - hext*w, y - hext*h);
	worldAABB.upperBound.Set(x + (1+hext)*w, y + (1+hext)*h);
	b2Vec2 gravity(0.0f, 0.f);
	bool doSleep = true;
	mPhysicsWorld = new b2World(worldAABB, gravity, doSleep);
	mPhysicsWorld->SetContactListener(this);

#ifdef _DEBUG
	mDebuggingEnabled = true;
	mPhysicsWorld->SetDebugDraw(&mDebugDraw);
#else
	mDebuggingEnabled = false;
	mPhysicsWorld->SetDebugDraw(NULL);
#endif

	mDebugDraw.SetFlags(~0);
	mDebugDraw.Reset();

	SetupWorldBoundaries();
}


void CPhysicsManager::DestroyWorld()
{
	CLog::Get().Info("Destroying physics world");
	if (mPhysicsWorld != NULL)
	{
		delete mPhysicsWorld;
		mPhysicsWorld = NULL;
	}
}

void CPhysicsManager::SetupWorldBoundaries()
{
	float x=mBox.x, y=mBox.y, w=mBox.w, h=mBox.h;

	//set up static bodies (walls)
	b2PolygonDef horiShapeCfg, vertShapeCfg;
	horiShapeCfg.SetAsBox(w, 0.5f);
	vertShapeCfg.SetAsBox(0.5f, h);
	//fixme horiShapeCfg.filter.categoryBits = CActor::ACTOR_STATIONARY;
	//fixme vertShapeCfg.filter.categoryBits = CActor::ACTOR_STATIONARY;
	horiShapeCfg.friction = 10.f;
	vertShapeCfg.friction = 10.f;

	b2BodyDef def;
	b2Body *body;

	//floor
	def.position.Set(x + 0.5f*w, y - 0.5f);
	body = mPhysicsWorld->CreateBody(&def);
	body->CreateShape(&horiShapeCfg);
	//roof
	def.position.Set(x + 0.5f*w, y + h + 0.5f);
	body = mPhysicsWorld->CreateBody(&def);
	body->CreateShape(&horiShapeCfg);

	//left
	def.position.Set(x - 0.5f, y + 0.5f*h);
	body = mPhysicsWorld->CreateBody(&def);
	body->CreateShape(&vertShapeCfg);
	//right
	def.position.Set(x + w + 0.5f, y + 0.5f*h);
	body = mPhysicsWorld->CreateBody(&def);
	body->CreateShape(&vertShapeCfg);
}

void CPhysicsManager::ResetDebug()
{
	mDebugDraw.Reset();
}

void CPhysicsManager::Update(float dt)
{
	mCollisions.clear();

	mPhysicsTick += dt;
	int numTicks = 0;
	while (mPhysicsTick >= PHYSICS_DT && numTicks < MAX_PHYSICS_TICKS) {
		if (mDebuggingEnabled)
			mDebugDraw.Reset();
		mPhysicsWorld->Step(PHYSICS_DT, 10);
		mPhysicsTick -= PHYSICS_DT;
		numTicks++;
	}
	if (numTicks == MAX_PHYSICS_TICKS) mPhysicsTick = 0;

	CollisionsMapCfg::iterator it;
	for (it=mCollisions.begin(); it != mCollisions.end(); it++)
	{
		if (it->second.actor1 != NULL)
			it->second.actor1->Collide(it->second.actor2, it->second, 1);
		if (it->second.actor2 != NULL)
			it->second.actor2->Collide(it->second.actor1, it->second, 2);
	}
}

void CPhysicsManager::RenderDebug()
{
	if (mDebuggingEnabled)
		mDebugDraw.Render();
}

void CPhysicsManager::ToggleDebug()
{
	mDebuggingEnabled = !mDebuggingEnabled;
	if (mDebuggingEnabled)
	{
		CLog::Get().Info("Enabling physics debug");
		mPhysicsWorld->SetDebugDraw(&mDebugDraw);
	}
	else
	{
		CLog::Get().Info("Disabling physics debug");
		mPhysicsWorld->SetDebugDraw(NULL);
	}
}

void CPhysicsManager::Add(const b2ContactPoint *point)
{
	b2Shape *shape1 = point->shape1;
	b2Shape *shape2 = point->shape2;

	CActor *actor1 = static_cast<CActor*>(shape1->GetUserData());
	CActor *actor2 = static_cast<CActor*>(shape2->GetUserData());
	ContactPoint c;
	c.actor1 = actor1;
	c.actor2 = actor2;
	c.normal = point->normal;
	mCollisions[point->id.key] = c;
}
