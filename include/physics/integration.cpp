#include "integration.h"
namespace solstice {
Derivative State::Evaluate(double t, float dt) {
    Derivative output;
    output.dv = x;
    output.dx = Accelerate(t + dt);
    return output;
}
Vector2f State::Accelerate(float t) {
   const float k = 15.0f;
    const float b = 0.1f;
    return -k * x - b * v;
}
Derivative State::Evaluate(double t, float dt, const Derivative& d) {
    State state;
    state.x = x + d.dx * dt;
    state.v = v + d.dv * dt;

    Derivative output;
    output.dx = state.v;
    output.dv = state.Accelerate(t + dt);
    return output;
}
void RK4(State* state, double t, float dt) {
    float rate = 1.0f;
    Derivative a, b, c, d;
    a = state->Evaluate(t, 0.0f, Derivative());
    b = state->Evaluate(t, dt * rate * 0.5f, a);
    c = state->Evaluate(t, dt * rate * 0.5f, b);
    d = state->Evaluate(t, dt * rate, c);

    float coeff = rate / 6.0f;
    Vector2f dxdt = coeff * (a.dx + (2.0f * (b.dx + c.dx) + d.dx));
    Vector2f dvdt = coeff * (a.dv + (2.0f * (b.dv + c.dv) + d.dv));

    state->x = state->x + dxdt * dt;
    state->v = state->v + dvdt * dt;
}

void Euler(State* state, float dt) {
    state->v+= state->Accelerate(0) * dt;
    state->x+= state->v * dt;
}
void Verlet(State* state, double t, float dt) {
    Vector2f lastAccel = state->Accelerate(t);
    Vector2f newPos = state->x + state->v * dt + 0.5f * lastAccel * dt * dt;
    state->v+= 0.5f * (state->Accelerate(t + dt) + lastAccel) * dt;
    state->x = newPos;
}
}
