#ifndef RENDERTILE_H_INCLUDED
#define RENDERTILE_H_INCLUDED

#include "backgroundtile.h"
#include "graphics/renderableobject.h"
#include "geometry/shapecreate.h"

#include "engine/resizableobject.h"

namespace solstice {
class RenderTile: public ResizableObject, public RenderableObject {
public:
    BackgroundTile2 mTile;
    Vector2f mPositionOffset;
    RenderTile();
    RenderTile(std::shared_ptr<IShapef> shape);
    RenderTile(BackgroundTile2 tile, int modelID, glm::mat4* transform);
    void Init(BackgroundTile2 tile, int modelID, glm::mat4* transform);

    void SetX(float x) {mTile.GetHBox()->SetX(x);}
    void SetY(float y) {mTile.GetHBox()->SetY(y);}
    void SetXY(float x, float y) {mTile.GetHBox()->SetPosition(Vector2f(x, y));}
    void SetXY(Vector2f xy) {mTile.GetHBox()->SetPosition(xy);}
    float GetX() {return mTile.GetHBox()->GetPosition().x;}
    float GetY() {return mTile.GetHBox()->GetPosition().y;}
    Vector2f GetPosition() {return mTile.GetHBox()->GetPosition();}
    void AddX(float x) {mTile.GetHBox()->AddX(x);}
    void AddY(float y) {mTile.GetHBox()->AddY(y);}
    void AddXY(float x, float y) {mTile.GetHBox()->AddXY(x, y);}
    void AddXY(Vector2f xy) {mTile.GetHBox()->AddXY(xy);}
    void SetAngle(float a) {mTile.GetHBox()->SetAngle(a);}
    void AddAngle(float a);
    void IncreaseWidth(int w);
    void IncreaseHeight(int h);
    void IncreaseSize(int height, int width);
    IShapef* GetShape() {return mTile.GetHBox().get();}
    Rectanglef GetHitRect();
    float GetAngle() {return mTile.GetHBox()->GetAngle();}


};
}

#endif // RENDERTILE_H_INCLUDED
