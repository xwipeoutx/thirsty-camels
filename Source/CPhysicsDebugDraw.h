#pragma once
#include "Box2D.h"
#include <list>

namespace TTC
{
	class CPhysicsDebugDraw : public b2DebugDraw
	{
	public:
		CPhysicsDebugDraw(void);
		~CPhysicsDebugDraw(void);

		/// Draw a closed polygon provided in CCW order.
		virtual void DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color);

		/// Draw a solid closed polygon provided in CCW order.
		virtual void DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color);

		/// Draw a circle.
		virtual void DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color);

		/// Draw a solid circle.
		virtual void DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color);

		/// Draw a line segment.
		virtual void DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color);

		/// Draw a transform. Choose your own length scale.
		/// @param xf a transform.
		virtual void DrawXForm(const b2XForm& xf);

		void Draw();
		void Reset();
	private:
		void RenderPolygons();
		void RenderSolidPolygons();
		void RenderCircles();
		void RenderSolidCircles();
		void RenderSegments();
		void RenderXForms();


		struct DebugPoly {
			std::list<b2Vec2> vertices;
			b2Color color;
		};

		struct DebugCircle {
			b2Vec2 center;
			float32 radius;
			b2Color color;
		};

		struct DebugSegment {
			b2Vec2 p1;
			b2Vec2 p2;
			b2Color color;
		};

		std::list<DebugPoly> mPolygons;
		std::list<DebugPoly> mSolidPolygons;

		std::list<DebugCircle> mCircles;
		std::list<DebugCircle> mSolidCircles;

		std::list<DebugSegment> mSegments;
		std::list<b2XForm> mXForms;


	};
};
