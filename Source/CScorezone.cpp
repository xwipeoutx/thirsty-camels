#include "CScorezone.h"
#include "CGameBoard.h"
#include "CPhysicsManager.h"

#include "CBall.h"

using namespace TTC;

CScorezone::CScorezone()
: IBoardFeature()
{
	SetFlags(PHYSICS_ZONE);

	b2BodyDef bodyDef;
	bodyDef.position.Set(0, 0);
	mBody = CPhysicsManager::GetSingleton().GetPhysicsWorld()->CreateBody(&bodyDef);

	b2PolygonDef shapeDef;
	shapeDef.SetAsBox(8, 1);
	shapeDef.filter.categoryBits = PHYSICS_ZONE;
	shapeDef.filter.maskBits = PHYSICS_BALL;
	shapeDef.userData = AsB2Data();
	shapeDef.isSensor = true;
	mBody->CreateShape(&shapeDef);
}

CScorezone::~CScorezone(void)
{
	CPhysicsManager::GetSingleton().GetPhysicsWorld()->DestroyBody(mBody);
	mBody = NULL;
}

void CScorezone::Collide(TTC::IPhysicsObject *other, const TTC::CPhysicsManager::ContactPoint &cp, int pos)
{
	CBall *ball = dynamic_cast<CBall*>(other);
	if (ball != NULL && !ball->GetDying())
	{
		GetGameBoard()->AddScore(1);
		ball->SetDying(true);
	}
}

bool CScorezone::Update(float dt)
{
	return true;
}
