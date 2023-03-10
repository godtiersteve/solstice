#include "watersphere.h"
namespace solstice  {

WaterSphere::WaterSphere() {
    InitPolyHitBox(0, 160, 16);
}

WaterSphere::WaterSphere(Vector2f pos, int size, int numSprings) {
    InitPolyHitBox(pos, size, numSprings);
}

void WaterSphere::InitPolyHitBox(Vector2f pos, int size, int numSprings) {
    mPolyHitBox = Polygonf(pos, numSprings, size);
    mLastPos = pos;
}
void WaterSphere::Update(double dt) {
    Vector2f vel = GetPosition() - mLastPos;
    Vector2f pos = GetPosition() + mAppliedImpulse;
    AddXY(vel + mAppliedImpulse + mAppliedForce * float(dt * dt));
    mLastPos = pos;
    mAppliedImpulse = 0;
    mAppliedForce = 0;
    UpdateSprings(dt);
}
float WaterSphere::CheckCollision(Rectanglef rect) {
    float depth = 0;
    Collision c = solstice::CheckCollision(&mPolyHitBox, &rect);
    if(c) {
        if(fabs(c.mtv.y) >= rect.GetHeight() * 0.5) {
            depth = mPolyHitBox.GetAABB().GetTop() - rect.GetBottom();
        }
        else {
            Circlef circle(mPolyHitBox.GetPosition(), mPolyHitBox.GetSize() * 0.5);
            Vector2f top = circle.GetClosestPoint(rect.GetBotCenter());
            depth = top.y - rect.GetBottom();
        }
        return depth;
    }
    else {
        return 0;
    }
}

void WaterSphere::Splash(Rectanglef rect, float force) {

}

void WaterSphere::Ripple(float force) {

}

void WaterSphere::IncreaseWidth(int w) {
    mPolyHitBox.SetSize(w);
}

void WaterSphere::IncreaseHeight(int h) {
    mPolyHitBox.SetSize(h);
}

void WaterSphere::IncreaseSize(int height, int width) {
    mPolyHitBox.SetSize(height);
}

}
