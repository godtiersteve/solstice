#include "spring.h"

namespace solstice {
    Spring::Spring() {
        height = 0;
        lastHeight = 0;
        targetHeight = 0;
        acceleration = 0;
        speed = 0;
        maxHeight = 0;
        minHeight = 0;
        k = 0.0125f;
        dampening = 0.01f;
        attach = NULL;
    }
    Spring::Spring(Vector2f pos, float h, float w) {
        position = pos;
        height = h;
        lastHeight = h;
        width = w;
        targetHeight = height;
        speed = 0;
        acceleration = 0;
        maxHeight = 0;
        minHeight = 0;
        k = 0.025f;
        dampening = 0.01f;
        attach = NULL;
    }
    void Spring::AddHeight(float h) {
        height+= h;
    }
    void Spring::Update2(float k, float damping) {
        float x = height - targetHeight;
        acceleration = -k * x - damping * speed;
        speed+= acceleration;
        height+= speed;
        if(maxHeight && height > maxHeight) {
            height = maxHeight;
        }
        if(minHeight && height < minHeight) {
            height = minHeight;
        }
    }
    void Spring::Update(double t, float dt) {
        Update(t, dt, k, dampening);
    }
    void Spring::Update(double t, float dt, float kk, float damping) {
        x.y = height;
        v.y = speed;
        this->k = kk;
        this->dampening = damping;
        RK4(this, t, dt);
        //Euler(this, dt);
        height = x.y;
        speed = v.y;
        if(height < minHeight) {
            //height = minHeight;
            //speed = fabs(speed) * 0.5;
        }
        if(height > maxHeight) {
            //height = maxHeight;
            //speed = -fabs(speed) * 0.5;
        }
        //float acceleration = -1.0f / this->k * height - speed * dampening;
        //speed+= acceleration * dt;
        //height+= speed * dt;
        //x.y = height;
        //v.y = speed;

        //float a = -k * x.y - dampening * v.y;
        //speed+= a * dt;
        //height+= speed * dt;
        //x.y = height;
        //v.y = speed;
        /*
        float x = height - targetHeight;
        acceleration = -k * x;
        speed+= acceleration;
        height+= speed;
        if(maxHeight && height > maxHeight) {
            height = maxHeight;
        }
        if(minHeight && height < minHeight) {
            height = minHeight;
        }
        */
        /*
        float x = height - targetHeight;
        if(x) {
            float prevAcc = acceleration;
            acceleration = -k * x - speed * damping * tstep;
            //acceleration = -k * x - damping * tstep * tstep;
            speed+= 0.5f * (acceleration + prevAcc) * tstep;
            float newHeight = height + speed * tstep + 0.5f * prevAcc * tstep * tstep;
            height = newHeight;
        }
        */
    }
    Derivative Spring::Evaluate(double t, float dt, const Derivative& d) {
        Spring spring;
        spring = *this;
        spring.x = x + d.dx * dt;
        spring.v = v + d.dv * dt;
        Derivative output;
        output.dx = spring.v;
        output.dv = spring.Accelerate(t + dt);
        return output;
    }
    Vector2f Spring::Accelerate(float t) {
        return  (-k * (x - Vector2f(0, targetHeight)) - v * dampening);
    }
    void Spring::SetTargetHeight(float h) {
        height = h;
        targetHeight = h;
    }
    void Spring::Pull(Spring& spring, float spread) {
        float delta = (height - spring.height) * spread;
        spring.speed+= delta;
        spring.height+= delta;
    }
    float Spring::HeightDifference(Spring& spring, float spread, float tstep) {
        float h = spring.height - height;
        return h * spread;
    }
    void Spring::SetHeight(Spring& spring, float spread) {
        float delta = spread * (height - spring.height);
        spring.height+= delta;
    }
}
