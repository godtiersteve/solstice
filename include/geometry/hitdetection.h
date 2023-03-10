#ifndef HITDETECTION_H_INCLUDED
#define HITDETECTION_H_INCLUDED

#include "shape.h"
#include "hitbox.h"
#include "polygon.h"
#include "rectangle.h"
#include "circle.h"
#include "edge.h"
#include "collision.h"
#include "utility/sol_math.h"

#include <iostream>
#include <vector>
#include <cfloat>
#include <stdio.h>

namespace solstice {
enum CollisionLocation{COL_NONE = 0, COL_RIGHT = 1, COL_LEFT = 2, COL_TOP = 4, COL_BOTTOM = 8, COL_ALL = COL_RIGHT | COL_LEFT | COL_TOP | COL_BOTTOM};
Collision CheckCollision(IShapef* shape1, IShapef* shape2);
Collision CheckCollision(IShapef* shape, const HitBoxf& hbox);
bool RectRectCollision(Rectanglef& rect1, Rectanglef& rect2);
bool RectRectCollision2(Rectanglef rect1, Rectanglef rect2, Vector2f& mtv);
bool RectRectCollision2(Rectanglef rect1, Rectanglef rect2);
bool Raycast(Vector2f pos, Vector2f dir, Rectanglef& rect, float length);
bool RayIntersectInv(Vector2f pos, Vector2f dir, Rectanglef& rect, float maxLength = 0.0);
Collision CircleCollision(Circlef& circ1, Circlef& circ2);
Collision CirclePolyCollision(Circlef& circ, Polygonf& poly);
Collision CircleRectCollision(Circlef& circ, Rectanglef& rect);
Collision AABBCollision(Rectanglef& rect1, Rectanglef& rect2);
Collision SeparatingAxis(Rectanglef& rect1, Rectanglef& rect2);
void GetContactPoint(Collision& c, Rectanglef& rect1, Rectanglef& rect2);
std::vector<Collision> CheckCollisionv(const HitBoxf& hbox1, const HitBoxf& hbox2);
Collision CheckCollision(const HitBoxf& hbox1, const HitBoxf& hbox2);

Collision RectCollision(Rectanglef& rect1, Rectanglef& rect2);
bool PointRectCollision(Vector2f point, Rectanglef rect);

bool PolyCollision(Polygonf& poly1, Polygonf& poly2, Vector2f& mtv);
Collision PolyCollision(Polygonf& poly1, Polygonf& poly2);

bool CheckRange(float min, float max, float value);
bool CheckRange(Vector2f first, Vector2f second);
bool CheckProjection(Rectanglef& rect1, Rectanglef& rect2, Vector2f axis);
bool CheckProjection(Rectanglef& rect1, Rectanglef& rect2, Vector2f axis, float& distance);
bool CheckProjectionPoly(Polygonf& p1, Polygonf& p2, Vector2f axis, float& distance);
}
#endif // HITDETECTION_H_INCLUDED
