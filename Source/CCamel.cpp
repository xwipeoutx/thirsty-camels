#include "CCamel.h"
#include "CSprite.h"
#include "CSpriteFactory.h"

#include "CGameBoard.h"
#include "CBall.h"

using namespace TTC;
using namespace FluidSolver;

CCamel::CCamel(Vector2 position, int direction, float scale, SDLKey key)
: mPosition(position), mDirection(direction), mScale(scale), mKey(key), mScore(0), mPower(1), mIsDrinking(false)
{
	SetFlags(PHYSICS_PLAYER);

	mSprite = CSpriteFactory::GetSingleton().Get("player", "Cactus1");
	if (mSprite == NULL)
	{
		mSprite = CSpriteFactory::GetSingleton().Load("player", "Camel1", "Camel1.png", 1, 1);
	}

	//add to physics world
	b2BodyDef bodyDef;
	bodyDef.position = b2Vec2( mPosition.x, mPosition.y );
	bodyDef.fixedRotation = false;
	bodyDef.linearDamping = 0.0f; //0.6f
	bodyDef.angularDamping = 0.9f;
	bodyDef.isBullet = false;
	mBody = CPhysicsManager::GetSingleton().GetPhysicsWorld()->CreateBody(&bodyDef);

	b2PolygonDef shapeDef;
	shapeDef.SetAsBox(2*scale, 2*scale);
	shapeDef.isSensor = true;
	shapeDef.density = 0.f;
	shapeDef.friction = 0.0f;
	shapeDef.restitution = 0.0f;
	shapeDef.userData = AsB2Data();
	shapeDef.filter.categoryBits = PHYSICS_PLAYER;
	shapeDef.filter.maskBits = PHYSICS_BALL;
	mBody->CreateShape(&shapeDef);
	mBody->SetMassFromShapes();

	mBody->SetLinearVelocity(b2Vec2_zero);
}

CCamel::~CCamel(void)
{
	CPhysicsManager::GetSingleton().GetPhysicsWorld()->DestroyBody(mBody);
	mBody = NULL;
}

void CCamel::HandleEvent(const SDL_Event &e)
{
	if (e.key.keysym.sym == mKey)
	{
		mIsDrinking = e.type == SDL_KEYDOWN;
	}
}

bool CCamel::Update(float dt)
{
	if (mPower > 0 && mIsDrinking && dt > 0)
	{
		float d = mPower * dt;
		GetGameBoard()->GetFluid()->Inject(mPosition.x, mPosition.y, mPower*dt*50, (1-mPower)*dt*50, 0, mScale);
		GetGameBoard()->GetFluid()->Perturb(mPosition.x, mPosition.y, (float)20*d*mDirection, 0, mScale);

		mPower -= 0.1f*dt;
	}
	else if (!mIsDrinking && dt > 0 && mPower < 1)
	{
		mPower += dt;
	}
	return true;
}

void CCamel::Draw()
{
	glPushMatrix();
	glTranslatef(mPosition.x, mPosition.y, 0);
	glScalef(4*mScale, 4*mScale, 1);
	if (mDirection < 1) glScalef(-1, 1, 1);

	mSprite->Draw();

	glPopMatrix();

}

void CCamel::Collide(IPhysicsObject *other, const CPhysicsManager::ContactPoint &cp, int pos)
{
	CBall *ball = dynamic_cast<CBall*>(other);
	if (ball != NULL && !ball->GetDying())
	{
		mScore++;
		ball->SetDying(true);
	}
}