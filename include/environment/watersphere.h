#ifndef WATERSPHERE_H_INCLUDED
#define WATERSPHERE_H_INCLUDED

#include "physics/spring.h"
#include "graphics/quaddata.h"
#include "geometry/polygon.h"
#include "environment/room.h"
#include "physics/movableobject.h"
#include <vector>

using std::vector;
namespace solstice {
    class WaterSphere : public WaterBody, public MovableObject {
    protected:
        Polygonf mPolyHitBox;
    public:
        WaterSphere();
        WaterSphere(Vector2f pos, int size, int numSprings);
        void InitPolyHitBox(Vector2f pos, int size, int numSprings);
        virtual Polygonf GetPolygon() {return mPolyHitBox;}
        virtual float CheckCollision(Rectanglef rect);
        virtual void Splash(Rectanglef rect, float force = 0);
        virtual void Ripple(float force);

        virtual void SetX(float x) {mPolyHitBox.SetX(x); mLastPos.x = x;}
        virtual void SetY(float y) {mPolyHitBox.SetY(y); mLastPos.y = y;}
        virtual void SetXY(float x, float y) {mPolyHitBox.SetPosition(Vector2f(x, y)); mLastPos.x = x; mLastPos.y = y;}
        virtual void SetXY(Vector2f xy) {mPolyHitBox.SetPosition(xy); mLastPos = xy;}
        virtual float GetX() {return mPolyHitBox.GetX();}
        virtual float GetY() {return mPolyHitBox.GetY();}
        virtual void IncreaseWidth(int w);
        virtual void IncreaseHeight(int h);
        virtual void IncreaseSize(int height, int width);
        virtual void AddX(float x) {mPolyHitBox.AddX(x); mLastPos.x+= x;}
        virtual void AddY(float y) {mPolyHitBox.AddY(y); mLastPos.y+= y;}
        virtual void AddXY(float x, float y) {mPolyHitBox.AddXY(Vector2f(x, y)); mLastPos.x+= x; mLastPos.y+=y;}
        virtual void AddXY(Vector2f xy) {mPolyHitBox.AddXY(xy); mLastPos+= xy;}
        virtual void SetAngle(float a) {}
        virtual void AddAngle(float a) {}
        virtual float GetAngle() {return 0;}
        virtual IShapef* GetShape() {return &mPolyHitBox;}
        virtual Rectanglef GetHitRect() {return mPolyHitBox.GetAABB();}
        virtual Vector2f GetPosition() {return mPolyHitBox.GetPosition();}

        virtual void Move(Vector2f distance) {AddXY(distance);}
        virtual void MoveTo(Vector2f pos) {WaterBody::SetPosition(pos);}
        virtual void Update(double dt);
        virtual IShapef* GetHitShapef() {return &mPolyHitBox;}
    };
}

#endif // WATERSPHERE_H_INCLUDED
