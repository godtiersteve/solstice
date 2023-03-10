#ifndef SPRING_H_INCLUDED
#define SPRING_H_INCLUDED

#include "graphics/vector2.h"
#include "physics/integration.h"

namespace solstice {
struct Spring : public State {
    Vector2f position;
    float height, targetHeight;
    float lastHeight;
    float maxHeight, minHeight;
    float width;
    float acceleration, speed;
    float k, dampening;
    Spring* attach;
    Spring();
    Spring(Vector2f pos, float h, float w);
    Vector2f Accelerate(float t);
    void AddHeight(float h);
    void Update(double t, float dt, float kk, float damping);
    void Update(double t, float dt);
    void Update2(float k, float damping);
    void Update2() {Update2(k, dampening);}
    void SetTargetHeight(float h);
    void Pull(Spring& spring, float spread);
    Derivative Evaluate(double t, float dt, const Derivative& d);
    float HeightDifference(Spring& spring, float spread, float dt = 1.0f);
    void SetHeight(Spring& spring, float spread);
};

typedef Spring WaterSpring;

}

#endif // SPRING_H_INCLUDED
