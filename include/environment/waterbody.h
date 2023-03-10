#ifndef WATERBODY_H_INCLUDED
#define WATERBODY_H_INCLUDED

#include "watersurface.h"
#include "environment/backgroundtile.h"
#include "graphics/3d/splashmesh.h"
#include "player/camera2d.h"
#include "engine/resizableobject.h"

namespace solstice {
enum WaterType {
    WATER_FRESH,
    WATER_SALT,
    WATER_LAVA
};
class WaterBody : public ResizableObject {
protected:
    virtual void UpdateSprings(double dt) {mSplashMesh.Update(0, dt);}
public:
    WaterSurface mSurface;
    Color mWaterColor;
    int mZSplashPos;
    SplashMesh mSplashMesh;
    CameraBoundary mCamBoundary;
    Rectanglef mHitBox;
    WaterType mWaterType;
    float mSurfaceHeight;
    WaterBody();
    WaterBody(WaterSurface surface, Rectanglef hitbox);
    WaterBody(Vector2f origin, float height, float width, float springHeight, float numSprings);

    void Init(WaterSurface surface, Rectanglef hitbox);
    void Init(Vector2f origin, float height, float width, float springHeight, float numSprings);
    WaterSurface& GetSurface()                              {return mSurface;}
    Rectanglef& GetHitBox()                                 {return mHitBox;}
    Color& GetColor()                                       {return mWaterColor;}
    Vector2f GetPosition()                                  {return mHitBox.GetPosition();}
    void SetSplashMesh(SplashMesh m)                        {mSplashMesh = m;}
    SplashMesh& GetWaterMesh()                              {return mSplashMesh;}
    SplashMesh* GetWaterMeshPtr()                           {return &mSplashMesh;}
    float GetTop()                                          {return mHitBox.GetTop();}
    CameraBoundary GetCamBoundary()                         {return mCamBoundary;}
    void SetCamBoundary(CameraBoundary b)                   {mCamBoundary = b;}
    void SetZSplashPos(int i)                               {mZSplashPos = i;}
    int GetZSplashPos()                                     {return mZSplashPos;}
    Rectanglef GetSurfaceRect()                             {return Rectanglef(mHitBox.GetTopLeft() - Vector2f(0, mSurfaceHeight * 0.5), mSurfaceHeight, mHitBox.GetWidth());}

    void SetColor(Color c)      {mWaterColor = c; mSurface.GetColor() = c;}
    void SetPosition(Vector2f vt);

    void Update(double dt = 1.0f);
    virtual Polygonf GetPolygon();
    virtual float CheckCollision(Rectanglef rect);
    virtual float CheckCollision(Circlef circ);
    virtual float CheckCollision(IShapef* shape);
    bool CheckSurfaceCollision(Rectanglef& rect);
    virtual void Splash(Rectanglef rect, float force = 0);
    virtual void Ripple(float force);
    virtual void Draw(ShaderProgram& shader)                                     {mSplashMesh.Draw(shader);}
    virtual void Draw(ShaderProgram& shader, TextureArray& textures)             {mSplashMesh.Draw(shader, textures);}
    std::vector<Rectanglef> GetRects();

    virtual void SetX(float x) {mHitBox.SetX(x);}
    virtual void SetY(float y) {mHitBox.SetY(y);}
    virtual void SetXY(float x, float y) {mHitBox.SetPosition(Vector2f(x, y));}
    virtual void SetXY(Vector2f xy) {mHitBox.SetPosition(xy);}
    virtual float GetX() {return mHitBox.GetX();}
    virtual float GetY() {return mHitBox.GetY();}
    virtual void IncreaseWidth(int w);
    virtual void IncreaseHeight(int h);
    virtual void IncreaseSize(int height, int width);
    virtual void AddX(float x) {mHitBox.AddX(x);}
    virtual void AddY(float y) {mHitBox.AddY(y);}
    virtual void AddXY(float x, float y) {mHitBox.AddXY(Vector2f(x, y));}
    virtual void AddXY(Vector2f xy) {mHitBox.AddXY(xy);}
    virtual void SetAngle(float a) {mHitBox.SetAngle(a);}
    virtual void AddAngle(float a) {mHitBox.SetAngle(mHitBox.GetAngle() + a);}
    virtual float GetAngle() {return 0;}
    virtual IShapef* GetShape() {return &mHitBox;}
    virtual Rectanglef GetHitRect() {return mHitBox;}
};
}

#endif // WATERBODY_H_INCLUDED
