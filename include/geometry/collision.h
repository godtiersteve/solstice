#ifndef COLLISION_H
#define COLLISION_H

#include "graphics/vector2.h"
#include "rectangle.h"

class BasePlayer2;
namespace solstice {
struct Collision {
    Vector2f mtv, normal;
    Vector2f contactPoint;
    Edgef e1, e2, clipEdge, minEdge;
    bool hasCollision;
    Collision(float mintrans = 0);
    Collision(bool col);
    operator bool()  {return hasCollision;}
    bool Empty() {return !hasCollision;}
    void GetEdges(Rectanglef& rect1, Rectanglef& rect2);
    Vector2f GetContactPoint(Rectanglef& rect1, Rectanglef& rect2);
    //ResolvePlayer(BasePlayer2* player, RigidBody* body);
};
std::vector<Vector2f> Clip(Vector2f v1, Vector2f v2, Vector2f n, float o);
//bool CheckAndResolvePlayer(BasePlayer2* player, RigidBody* body);

}

#endif // COLLISION_H
