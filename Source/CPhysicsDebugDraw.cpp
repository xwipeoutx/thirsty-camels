#include "CPhysicsDebugDraw.h"
#include "common.h"

using namespace TTC;

CPhysicsDebugDraw::CPhysicsDebugDraw(void)
{
}

CPhysicsDebugDraw::~CPhysicsDebugDraw(void)
{
}

void CPhysicsDebugDraw::DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
{
	std::list<b2Vec2> verts;
	for (int i=0; i<vertexCount; i++)
	{
		b2Vec2 vert = vertices[i];
		verts.push_back(vert);
	}
	DebugPoly poly = {verts, color};
	mPolygons.push_back(poly);
	return;
}

void CPhysicsDebugDraw::DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
{
	std::list<b2Vec2> verts;
	for (int i=0; i<vertexCount; i++)
	{
		b2Vec2 vert = vertices[i];
		verts.push_back(vert);
	}
	DebugPoly poly = {verts, color};
	mSolidPolygons.push_back(poly);
	return;
}

void CPhysicsDebugDraw::DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color)
{
	DebugCircle c = {center, radius, color};
	mCircles.push_back(c);
	return;
}

void CPhysicsDebugDraw::DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color)
{
	DebugCircle c = {center, radius, color};
	mSolidCircles.push_back(c);
	return;
}

void CPhysicsDebugDraw::DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color)
{
	DebugSegment s = {p1, p2, color};
	mSegments.push_back(s);
	return;
}

void CPhysicsDebugDraw::DrawXForm(const b2XForm& xf)
{
	mXForms.push_back(xf);
	return;
}

void CPhysicsDebugDraw::Render()
{
	glDisable(GL_TEXTURE_2D);
	RenderPolygons();
	RenderSolidPolygons();
	RenderCircles();
	RenderSolidCircles();
	RenderSegments();
	RenderXForms();
	glEnable(GL_TEXTURE_2D);

}
void CPhysicsDebugDraw::Reset()
{
	mPolygons.clear();
	mSolidPolygons.clear();
	mCircles.clear();
	mSolidCircles.clear();
	mSegments.clear();
	mXForms.clear();
}
void CPhysicsDebugDraw::RenderPolygons()
{
	std::list<DebugPoly>::iterator it;

	for (it=mPolygons.begin(); it != mPolygons.end(); ++it)
	{
		glColor3f(it->color.r, it->color.g, it->color.b);
		glBegin(GL_LINE_LOOP);
		std::list<b2Vec2>::iterator it2;
		for (it2 = it->vertices.begin(); it2 != it->vertices.end(); ++it2)
		{
			glVertex3f(it2->x, it2->y, 0);
		}
		glEnd();
	}
}
void CPhysicsDebugDraw::RenderSolidPolygons()
{
	std::list<DebugPoly>::iterator it;

	for (it=mSolidPolygons.begin(); it != mSolidPolygons.end(); ++it)
	{
		glColor3f(it->color.r, it->color.g, it->color.b);
		glBegin(GL_TRIANGLE_FAN);
		std::list<b2Vec2>::iterator it2;
		for (it2 = it->vertices.begin(); it2 != it->vertices.end(); ++it2)
		{
			glVertex3f(it2->x, it2->y, 0);
		}
		glEnd();
	}
}
void CPhysicsDebugDraw::RenderCircles()
{
	std::list<DebugCircle>::iterator it;
	const float32 k_segments = 16.0f;
	const float32 k_increment = 2.0f * b2_pi / k_segments;

	for (it = mCircles.begin(); it != mCircles.end(); ++it)
	{
		float32 theta = 0.0f;
		glColor3f(it->color.r, it->color.g, it->color.b);
		glBegin(GL_LINE_LOOP);
		for (int32 i = 0; i < k_segments; ++i)
		{
			b2Vec2 v = it->center + it->radius * b2Vec2(cosf(theta), sinf(theta));
			glVertex2f(v.x, v.y);
			theta += k_increment;
		}
		glEnd();
	}
}
void CPhysicsDebugDraw::RenderSolidCircles()
{
	std::list<DebugCircle>::iterator it;
	const float32 k_segments = 16.0f;
	const float32 k_increment = 2.0f * b2_pi / k_segments;

	for (it = mSolidCircles.begin(); it != mSolidCircles.end(); ++it)
	{
		float32 theta = 0.0f;
		glColor3f(it->color.r, it->color.g, it->color.b);
		glBegin(GL_TRIANGLE_FAN);
		for (int32 i = 0; i < k_segments; ++i)
		{
			b2Vec2 v = it->center + it->radius * b2Vec2(cosf(theta), sinf(theta));
			glVertex2f(v.x, v.y);
			theta += k_increment;
		}
		glEnd();
	}
}
void CPhysicsDebugDraw::RenderSegments()
{
	std::list<DebugSegment>::iterator it;

	for (it = mSegments.begin(); it != mSegments.end(); ++it)
	{
		glColor3f(it->color.r, it->color.g, it->color.b);
		glBegin(GL_LINE_STRIP);
			glVertex3f(it->p1.x, it->p1.y, 0);
			glVertex3f(it->p2.x, it->p2.y, 0);
		glEnd();
	}
}
void CPhysicsDebugDraw::RenderXForms()
{
	std::list<b2XForm>::iterator it;
	for (it=mXForms.begin(); it != mXForms.end(); ++it)
	{
		const b2XForm &xf = *it;
		b2Vec2 p1 = xf.position, p2;
		const float32 k_axisScale = 0.4f;
		glBegin(GL_LINES);

			glColor3f(1.0f, 0.0f, 0.0f);
			glVertex2f(p1.x, p1.y);
			p2 = p1 + k_axisScale * xf.R.col1;
			glVertex2f(p2.x, p2.y);

			glColor3f(0.0f, 1.0f, 0.0f);
			glVertex2f(p1.x, p1.y);
			p2 = p1 + k_axisScale * xf.R.col2;
			glVertex2f(p2.x, p2.y);

		glEnd();
	}
}
