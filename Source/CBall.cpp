#include "CBall.h"

using namespace TTC;
using namespace FluidSolver;

CBall::CBall(Vector2 position, float radius)
: IVelocityPoller(position), mSpeedUp(1), mVelocity(0,0), mFluidVelocity(0,0), mRadius(radius)
{
	//call list
	mCallList = glGenLists(1);
	//GLUquadric *newQuadric = gluNewQuadric();
	const int numPoints = 16;
	glNewList(mCallList, GL_COMPILE);
	glColor4f(0,1,0, 1);
	glBegin(GL_TRIANGLE_FAN);
		for (int i=0; i < numPoints; i++) 
		{
			float v1 = radius * sin(6.28f * ((float)i)/numPoints);
			float v2 = radius * cos(6.28f * ((float)i)/numPoints);
			glVertex2f(v1, v2);
		}
	glEnd();
	glEndList();
	//gluDeleteQuadric(newQuadric);

	//add to physics world
	b2BodyDef bodyDef;
	bodyDef.position = b2Vec2( mPosition.x, mPosition.y );
	bodyDef.fixedRotation = false;
	bodyDef.linearDamping = 0.0f; //0.6f
	bodyDef.angularDamping = 0.0f;
	bodyDef.isBullet = false;
	mBody = CPhysicsManager::GetSingleton().GetPhysicsWorld()->CreateBody(&bodyDef);

	b2CircleDef shapeDef;
	shapeDef.density = 1.f;
	shapeDef.friction = 1.f;
	shapeDef.restitution = 0.0f; //0.6f
	shapeDef.radius = radius;
	mBody->CreateShape(&shapeDef);
	mBody->SetMassFromShapes();

	mBody->SetLinearVelocity(b2Vec2_zero);	
}

CBall::~CBall(void)
{
	CPhysicsManager::GetSingleton().GetPhysicsWorld()->DestroyBody(mBody);
	mBody = NULL;
	glDeleteLists(mCallList, 1);

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
		
		mBody->ApplyImpulse(velocityDeltab2, b2Vec2_zero);
		const b2Vec2 &pos = mBody->GetPosition();
		mPosition.x = pos.x;
		mPosition.y = pos.y;
	}
	return true;
}

void CBall::Draw()
{
	glTranslatef(mPosition.x, mPosition.y, 0);
	


	glCallList(mCallList);
	//GLUquadric *newQuadric = gluNewQuadric();
	//gluSphere(newQuadric, 0.2f, 16, 2);
	//gluDeleteQuadric(newQuadric);

	glTranslatef(-mPosition.x, -mPosition.y, 0);

}