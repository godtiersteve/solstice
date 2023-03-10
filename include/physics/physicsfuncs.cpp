#include "physicsfuncs.h"
namespace solstice {
Vector2f CalcVelocity(RigidBody* body, float friction, float elasticity, Vector2f normal) {
    Vector2f velocity = body->GetPosition() - body->GetLastPosition();
    Vector2f normalVel = normal.Dot(velocity) * normal;
    Vector2f tangentVel = velocity - normalVel;
    float groundFriction = friction * fabs(normal.y);
    if(normal.x && normal.y > 0.25f) {
        groundFriction = 1.0f;
    }
    friction = std::max(body->GetFriction(), groundFriction);
    elasticity = std::max(body->GetElasticity(), elasticity);
    return tangentVel * -friction + normalVel * -(1 + elasticity);
}
}
