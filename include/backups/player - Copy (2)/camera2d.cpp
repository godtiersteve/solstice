#include "camera2d.h"

namespace solstice {
CameraBoundary::CameraBoundary() {
    horiLimit = 0;
    vertLimit = 0;
    offsetSlowStep = Vector2f(0, 5);
    velocityRange = Vector2f(0, 10);
    offsetMax = Vector2f(0, 25);
    offsetMin = Vector2f(0, 25);
}
CameraBoundary::CameraBoundary(Rectanglef r) {
    *this = r;
    horiLimit = 0;
    vertLimit = 0;
    offsetSlowStep = Vector2f(0, 5);
    velocityRange = Vector2f(0, 10);
    offsetMax = Vector2f(0, 25);
    offsetMin = Vector2f(0, 25);
}
Camera::Camera() {
    stepSize = 1.0f;
    xOffset = 0;
    yOffset = 0;
}
void Camera::SetPosition(Vector2f pos) {
    position = GetBoundedPosition(pos);
    position.x = -position.x;
    position.y = -position.y;
}
void Camera::MoveTo(Vector2f pos) {
    pos.x+= xOffset;
    pos.y+= yOffset;
    Vector2f dest = GetBoundedPosition(pos);
    position.x = -position.x;
    position.y = -position.y;
    if(position != dest) {
        position = position - (position - dest) / (float)stepSize;
    }
    position = -position;
}
Vector2f Camera::GetBoundedPosition(Vector2f pos) {
    float leftLimit = mBoundary.GetLeft() + mBoundary.horiLimit;
    if(pos.x < mBoundary.GetLeft() + mBoundary.horiLimit) {
        pos.x = mBoundary.GetLeft() + mBoundary.horiLimit;
    }
    else if(pos.x > mBoundary.GetRight() - mBoundary.horiLimit) {
        pos.x = mBoundary.GetRight() - mBoundary.horiLimit;
    }
    if(pos.y < mBoundary.GetBottom() + mBoundary.vertLimit) {
        pos.y = mBoundary.GetBottom() + mBoundary.vertLimit;
    }
    else if(pos.y > mBoundary.GetTop() - mBoundary.vertLimit) {
        pos.y = mBoundary.GetTop() - mBoundary.vertLimit;
    }
    return pos;
}
void Camera::MoveTo(Vector2f velocity, Vector2f pos, float dt) {
    float xDist = position.x - pos.x;
    float yDist = position.y - pos.y;
    if(fabs(xDist) < 10) {
        xDist*= stepSize;
    }
    if(fabs(yDist) < 10) {
        yDist*= stepSize;
    }
    pos = position - Vector2f(xDist + xOffset, yDist + yOffset) + Vector2f(xOffset, yOffset);
    Vector2f dest = GetBoundedPosition(pos);
    position.x = -position.x;
    position.y = -position.y;
    if(position != dest) {
        position = position - (position - dest) * stepSize;
    }
    position = -position;
}
}
