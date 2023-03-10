#ifndef INTEGRATE_H_INCLUDED
#define INTEGRATE_H_INCLUDED

#include "graphics/vector2.h"

namespace solstice {
struct Derivative {
    Vector2f dx, dv;
};

struct State {
    Vector2f x, v;
    virtual Vector2f Accelerate(float t);
    virtual Derivative Evaluate(double t, float dt, const Derivative& d);
    virtual Derivative Evaluate(double t, float dt);
};

void RK4(State* state, double t, float dt);
void Euler(State* state, float dt);
void Verlet(State* state, double t, float dt);

}

#endif // INTEGRATION_H_INCLUDED
