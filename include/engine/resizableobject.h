#ifndef RESIZABLEOBJECT_H_INCLUDED
#define RESIZABLEOBJECT_H_INCLUDED

#include "graphics/vector2.h"
#include "geometry/shape.h"
#include "geometry/rectangle.h"

namespace solstice {
    class ResizableObject {
    public:
        std::string mName;
        virtual void SetX(float x) = 0;
        virtual void SetY(float y) = 0;
        virtual void SetXY(float x, float y) = 0;
        virtual void SetXY(Vector2f xy) = 0;
        virtual float GetX() = 0;
        virtual float GetY() = 0;
        virtual Vector2f GetPosition() = 0;
        virtual void IncreaseWidth(int w) = 0;
        virtual void IncreaseHeight(int h) = 0;
        virtual void IncreaseSize(int height, int width) = 0;
        virtual void AddX(float x) = 0;
        virtual void AddY(float y) = 0;
        virtual void AddXY(float x, float y) = 0;
        virtual void AddXY(Vector2f xy) = 0;
        virtual void SetAngle(float a) = 0;
        virtual void AddAngle(float a) = 0;
        virtual float GetAngle() =  0;
        virtual void SetPosition(Vector2f pos) {SetXY(pos);}
        virtual IShapef* GetShape() {return nullptr;}
        virtual Rectanglef GetHitRect() = 0;
    };
}


#endif // LEVELEDITOROBJECT_H_INCLUDED
