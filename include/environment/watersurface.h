#ifndef WATERSURFACE_H_INCLUDED
#define WATERSURFACE_H_INCLUDED

#include "geometry/rectangle.h"
#include "physics/spring.h"
#include "graphics/color.h"
#include "geometry/geometry.h"
#include <vector>
#include <string>

using std::vector;
using std::string;

//TODO : Lock the first and last springs to base position
namespace solstice {

class WaterSurface {
private:
    vector<WaterSpring> mWaterSurface;
    float mSpringWidth, mSpringHeight;
    float mSpread;
    float mDamping, mSpringConstant;
    Color mWaterColor;
    int mTextureID;
    string mTextureRef;
    Vector2f mPosition;
    Polygonf mPolygon;
    bool mRipples;
    void GeneratePolygon();
    void UpdatePolygon();
public:
    WaterSurface(int springWidth = 10, float height = 100, float width = 100);
    void Init(int springWidth = 10, float height = 100, float width = 100);
    void ReinitSurface(Vector2f position, float height, float width);

    float GetSpringWidth()              {return mSpringWidth;}
    float GetSpringHeight()             {return mSpringHeight;}
    float GetSpread()                   {return mSpread;}
    Color& GetColor()                   {return mWaterColor;}
    int GetTexutreID()                  {return mTextureID;}
    string GetTextureRef()              {return mTextureRef;}
    Vector2f GetPosition()              {return mPosition;}
    float GetDamping()                  {return mDamping;}
    float GetSpringConstant()           {return mSpringConstant;}
    bool GetRipples()                   {return mRipples;}
    vector<WaterSpring>& GetSprings()   {return mWaterSurface;}
    void SetSpringWidth(float width);
    void SetSpringHeight(float height);
    void SetPosition(Vector2f position);
    void SetSpread(float spread)            {mSpread = spread;}
    void SetTextureID(int id)               {mTextureID = id;}
    void SetTextureRef(string ref)          {mTextureRef = ref;}
    void SetDamping(float damping)          {mDamping = damping;}
    void SetSpringConstant(float k)         {mSpringConstant = k;}
    void SetRipples(bool r)                 {mRipples = r;}

    void Update();
    void Splash(Rectanglef hitbox, float force = 0);
    void Ripple(float force, int i = 0);
    Polygonf& GetPolygon();
    Polygonf GetSurfacePoly();
    vector<Rectanglef> GetRects(Vector2f origin = 0);
};
}

#endif // WATERSURFACE_H_INCLUDED
