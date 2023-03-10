#ifndef CAMERA2D_H_INCLUDED
#define CAMERA2D_H_INCLUDED

#include "graphics/vector2.h"
#include "geometry/rectangle.h"
#include <algorithm>
namespace solstice {
struct CameraBoundary : public Rectanglef {
    Vector2f offsetMax, offsetMin, offsetStep, velocityRange, offsetSlowStep;
    float horiLimit, vertLimit;
    CameraBoundary();
    CameraBoundary(Rectanglef r);
    CameraBoundary& operator=(Rectanglef r) {SetXYHW(r.GetPosition().x, r.GetPosition().y, r.GetHeight(), r.GetWidth()); return *this;}
    void Init(Rectanglef r) {*this = r;}
};
struct Camera {
    Vector2f position;
    Vector2f origin;
    float stepSize;
    float xOffset, yOffset;
    CameraBoundary mBoundary;
    Camera();
    void SetPosition(Vector2f pos);
    void MoveTo(Vector2f pos);
    void MoveTo(Vector2f velocity, Vector2f pos, float dt = 1.0);
    Vector2f GetBoundedPosition(Vector2f pos);
    void Bind(CameraBoundary boundary) {mBoundary = boundary;}
    CameraBoundary& GetBoundary() {return mBoundary;}
};
}

#endif // CAMERA2D_H_INCLUDED
