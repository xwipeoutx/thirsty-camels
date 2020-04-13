#include "CBall.h"
#include "CSprite.h"
#include "CSpriteFactory.h"

using namespace TTC;
using namespace FluidSolver;

CBall::CBall(Vector2 position, float radius)
: IVelocityPoller(position), mSpeedUp(1), mVelocity(0,0), mFluidVelocity(0,0), mRadius(radius)
{
	SetFlags(PHYSICS_BALL);

	mSprite = CSpriteFactory::GetSingleton().Get("ball", "Cactus1");

	/*
	//call list
	mCallList = glGenLists(1);
	
	const int numPoints = 16;
	glNewList(mCallList, GL_COMPILE);
	glColor4f(0,1,0,1);
	glBegin(GL_TRIANGLE_FAN);
		for (int i=0; i < numPoints; i++) 
		{
			float v1 = radius * sin(6.28f * ((float)i)/numPoints);
			float v2 = radius * cos(6.28f * ((float)i)/numPoints);
			glVertex2f(v1, v2);
		}
	glEnd();
	glEndList();
	*/

	//add to physics world
	b2BodyDef bodyDef;
	bodyDef.position = b2Vec2( mPosition.x, mPosition.y );
	bodyDef.fixedRotation = false;
	bodyDef.linearDamping = 0.0f; //0.6f
	bodyDef.angularDamping = 0.9f;
	bodyDef.isBullet = false;
	mBody = CPhysicsManager::GetSingleton().GetPhysicsWorld()->CreateBody(&bodyDef);

	b2CircleDef shapeDef;
	shapeDef.density = 1.f;
	shapeDef.friction = 0.2f;
	shapeDef.restitution = 0.6f;
	shapeDef.radius = radius;
	shapeDef.userData = AsB2Data();
	shapeDef.filter.categoryBits = PHYSICS_BALL;
	shapeDef.filter.maskBits = PHYSICS_ALL;
	mBody->CreateShape(&shapeDef);
	mBody->SetMassFromShapes();

	mBody->SetLinearVelocity(b2Vec2_zero);
}

CBall::~CBall(void)
{
	CPhysicsManager::GetSingleton().GetPhysicsWorld()->DestroyBody(mBody);
	mBody = NULL;
	mSprite = NULL;
}

bool CBall::Update(float dt)
{
	//move object
	if (dt > 0)
	{
		b2Vec2 vel = mBody->GetLinearVelocity();
		mVelocity.x = vel.x; mVelocity.y = vel.y;

		Vector velocityDelta = (mFluidVelocity - mVelocity) * mSpeedUp * dt;
		b2Vec2 velocityDeltab2(velocityDelta.x, velocityDelta.y);
		
		//FIXME: Apply angular velocity based on curl (Fluid). Difficult. Do it later. Requires changes in fluid sim
		mBody->ApplyImpulse(velocityDeltab2, mBody->GetWorldPoint(b2Vec2_zero));
		
		const b2Vec2 &pos = mBody->GetPosition();
		mPosition.x = pos.x;
		mPosition.y = pos.y;
	}
	return true;
}

void CBall::Draw()
{
	glPushMatrix();
	glTranslatef(mPosition.x, mPosition.y, 0);
	glScalef(2*mRadius, 2*mRadius, 1);
	glRotatef(mBody->GetAngle()*180/3.14159f, 0, 0, 1);

	mSprite->Draw();

	glPopMatrix();

}

void CBall::Collide(IPhysicsObject *other, const CPhysicsManager::ContactPoint &cp, int pos)
{
	// FIXME anything?
}